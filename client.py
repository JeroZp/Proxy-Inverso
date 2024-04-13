import socket
import sys
import datetime
import os

# Función para enviar una solicitud HTTP
def send_request(url, port, method):
    # Verifica si la URL comienza con "http://"
    if url.startswith("http://"):
        url = url[7:]  # Elimina "http://"

    # Extrae el nombre del host de la URL
    host = url.split("/")[0]
    
    # Crea un socket TCP/IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Conecta el socket al servidor remoto en el puerto especificado
    server_address = (host, port)
    sock.connect(server_address)

    try:
        # Forma la solicitud HTTP
        request = f"{method} / HTTP/1.1\r\nHost: {host}\r\nConnection: close\r\n\r\n"

        # Envía la solicitud al servidor
        sock.sendall(request.encode())

        # Lee la respuesta del servidor
        response = b""
        while True:
            data = sock.recv(1024)
            if not data:
                break
            response += data
        
        # Guarda la respuesta en un archivo si es HTML o cualquier otro tipo de archivo
        if "Content-Type: text/html" in response.decode() or "Content-Type: image/" in response.decode() or "Content-Type: application/" in response.decode():
            save_file(response)
        
        # Retorna la respuesta recibida del servidor
        return response.decode(errors='ignore')  # Agregamos el manejo de errores 'ignore'
    
    finally:
        # Cierra el socket
        sock.close()

# Función para guardar el contenido en un archivo
def save_file(content):
    filename = f"response_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
    with open(filename, "wb") as f:
        f.write(content)

# Función para escribir en el archivo de registro
def log_request(response):
    with open(log_file, "a") as log:
        log.write(f"{datetime.datetime.now().date()} {datetime.datetime.now().time()} {request_method} {response}\n")

# Función para limpiar el caché
def flush_cache():
    # Elimina todos los archivos de respuesta en el directorio actual
    for filename in os.listdir():
        if filename.startswith("response_"):
            os.remove(filename)
    print("Caché eliminado.")

# Función principal
if _name_ == "_main_":
    # Verifica si se proporcionan los argumentos necesarios
    if len(sys.argv) != 4:
        print("Uso: httpclient <log_file> <url or IP> <port>")
        sys.exit(1)

    # Obtiene los argumentos de la línea de comandos
    log_file = sys.argv[1]
    url = sys.argv[2]
    port = int(sys.argv[3])

    # Muestra un mensaje de inicio
    print("HTTP Client iniciado.")

    # Bucle principal
    while True:
        # Solicita al usuario que ingrese un método HTTP
        request_method = input("Ingrese el método HTTP (GET, HEAD, FLUSH): ").upper()
        
        # Verifica si el método es válido
        if request_method not in ["GET", "HEAD", "FLUSH"]:
            print("Método HTTP no válido. Inténtelo de nuevo.")
            continue
        
        # Si el método es FLUSH, borra el caché
        if request_method == "FLUSH":
            flush_cache()
            continue
        
        # Envía la solicitud HTTP al servidor y recibe la respuesta
        response = send_request(url, port, request_method)

        # Guarda la solicitud en el archivo de registro
        log_request(response)

        # Muestra la respuesta del servidor
        print("Respuesta del servidor:")
        print(response)
