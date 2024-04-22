#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>  // Para waitpid() y asociados
#include <unistd.h>    // Para fork() y close()
#include <stdlib.h>    // Para exit()

#define SERVER_PORT "8080"
#define BUFFER_SIZE 4096

char server1[] = "54.208.141.145";
char server2[] = "54.163.140.203";
char server3[] = "44.202.119.47";
#define APACHE_SERVER "127.0.0.1"
#define APACHE_PORT "80"

// Function prototypes
int create_proxy_socket();
void accept_client_connections(int proxy_sockfd);
void handle_client_request(int client_sockfd);
int connect_to_apache_server();
void forward_to_apache(int apache_sockfd, const char* request);
void receive_from_apache_and_forward_to_client(int apache_sockfd, int client_sockfd);

int main() {
    int proxy_sockfd = create_proxy_socket();

    if (proxy_sockfd < 0) {
        perror("Could not create proxy socket");
        exit(EXIT_FAILURE);
    }
    
    accept_client_connections(proxy_sockfd);
    close(proxy_sockfd);
    return 0;
}

int create_proxy_socket()
{
    struct addrinfo hints, *res;

    // first, load up the address structs with getaddrinfo():

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // use IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if (getaddrinfo(APACHE_SERVER, APACHE_PORT, &hints, &res) != 0)
    {
        perror("GET ADDRES INFO");
        return -1;
    }

    // define the socket file descriptor to listen for incomming connections

    int sockfd;

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd < 0)
    {
        perror("SOCKET");
        return -1;
    }

    // bind the socket file descriptor to the port

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0)
    {
        perror("BINDING");
        return -1;
    }

    // define the size of the queue to listen for incomming connections

    if (listen(sockfd, 100) != 0)
    {
        perror("LISTENING");
        return -1;
    }

    freeaddrinfo(res);

    return sockfd;
}

void accept_client_connections(int proxy_sockfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd;

    while (1) {
        client_sockfd = accept(proxy_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd < 0) {
            perror("Error al aceptar la conexión del cliente");
            continue;  // Continuar con el siguiente intento de conexión
        }

        // Crear un proceso hijo para manejar la conexión
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error al crear el proceso hijo");
            close(client_sockfd);  // Cerrar el socket del cliente en caso de fallo
        } else if (pid == 0) {
            // Estamos en el proceso hijo
            close(proxy_sockfd);  // El hijo no necesita el socket de escucha
            handle_client_request(client_sockfd);  // Manejar la solicitud del cliente
            exit(0);  // Terminar el proceso hijo después de manejar la solicitud
        } else {
            // Estamos en el proceso padre, que sigue aceptando nuevas conexiones
            close(client_sockfd);  // El padre cierra el socket del cliente, el hijo lo manejará
        }
    }
}


void handle_client_request(int client_sockfd)
{
    char client_request[BUFFER_SIZE];
    
    // Receive the message from the client
    ssize_t received = recv(client_sockfd, client_request, BUFFER_SIZE, 0);
    if (received < 0) {
        perror("Failed to receive message from client");
        close(client_sockfd);
        return;
    }
    
    // Translate the client message if necessary
    // For a simple proxy, this step might just be passing the request along
    
    // Connect to the Apache server
    int apache_sockfd = connect_to_apache_server();
    if (apache_sockfd < 0) {
        perror("Could not connect to Apache server");
        close(client_sockfd);
        return;
    }
    
    // Forward the message to Apache
    forward_to_apache(apache_sockfd, client_request);
    
    // Receive the response from Apache and send it back to the client
    receive_from_apache_and_forward_to_client(apache_sockfd, client_sockfd);
    
    // Close the connection with the client
    close(client_sockfd);
}

int connect_to_apache_server()
{
    int sockfd;
    struct sockaddr_in apache_addr;

    // Crear el socket TCP (SOCK_STREAM) para conectarse al servidor Apache
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("No se pudo crear el socket para conectar con Apache");
        return -1;
    }

    // Configurar la estructura sockaddr_in para la dirección del servidor Apache
    memset(&apache_addr, 0, sizeof(apache_addr));
    apache_addr.sin_family = AF_INET;
    apache_addr.sin_port = htons(80);

    // Convertir APACHE_SERVER a su representación binaria y almacenarla
    if (inet_pton(AF_INET, APACHE_SERVER, &apache_addr.sin_addr) <= 0) {
        perror("Dirección IP del servidor Apache inválida o error");
        close(sockfd);
        return -1;
    }

    // Conectar el socket con el servidor Apache
    if (connect(sockfd, (struct sockaddr *)&apache_addr, sizeof(apache_addr)) < 0) {
        perror("Error al conectar con el servidor Apache");
        close(sockfd);
        return -1;
    }

    return sockfd; // El socket está conectado, retornar su descriptor
}


void forward_to_apache(int apache_sockfd, const char* request)
{
    ssize_t sent_bytes, total_sent = 0;
    size_t request_length = strlen(request);  // Asume que 'request' es una cadena terminada en nulo

    // Enviar la solicitud al servidor Apache, manejando posibles interrupciones por señales
    while (total_sent < request_length) {
        sent_bytes = send(apache_sockfd, request + total_sent, request_length - total_sent, 0);
        if (sent_bytes < 0) {
            perror("Error al enviar la solicitud al servidor Apache");
            return;
        }
        total_sent += sent_bytes;
    }
}

void receive_from_apache_and_forward_to_client(int apache_sockfd, int client_sockfd)
{
    char server_response[BUFFER_SIZE];
    
    // Receive the response from Apache
    ssize_t received = recv(apache_sockfd, server_response, BUFFER_SIZE, 0);
    if (received > 0) {
        // Forward the response to the client
        send(client_sockfd, server_response, received, 0);
    }
    
    // Close the Apache server connection
    close(apache_sockfd);
}
