# Proxy inverso con APACHE y Balanceo de carga
## Integrantes:
- Moisés Arrieta Hernández
- Jerónimo Pérez Baquero
- Juan José Vélez Orozco

- https://www.youtube.com/watch?v=ovehuhW2mwY

## Introducción
Un proxy inverso es un servidor que se sitúa entre los clientes y los servidores web. Actúa como un intermediario para las solicitudes de los clientes que buscan recursos de uno o varios servidores. Apache es un servidor web de código abierto que puede ser configurado para actuar como un proxy inverso.

El uso de un proxy inverso tiene varias ventajas, como la capacidad de equilibrar la carga entre varios servidores back-end, proporcionar una capa adicional de abstracción y control para garantizar la fluidez del tráfico entre los clientes y los servidores, y ofrecer seguridad adicional.

## Desarrollo
### Generalidades
Para este proyecto se implementó en C un proxy inverso con balanceador de carga, dicho proxy soportara peticiones HTTP/1.1, además se implementó en este proxy un mecanismo de balanceo de carga y de cache, lo que permitirá una mayor fluidez en términos de velocidad de respuesta.

El balanceo de carga permite establecer la conexión de un URL con diversos servidores en una red privada, lo que ofrece la posibilidad de repartir las solicitudes entrantes por varios servidores y el caché 
para aumentar la velocidad de los servicios del servidor, ofrece una función con la que se pueden guardar las respuestas del servidor de manera temporal. Este caché hace que el servidor proxy sea capaz de responder por sí mismo de manera parcial o total a las peticiones recurrentes.

### Apache 
Se necesitan seguir los siguientes pasos para el proceso de instalación:
1. Instalación de apache: En primer lugar, necesitarás tener Apache instalado en tu servidor. En sistemas basados en Debian:
   ```C
     sudo apt-get update
     sudo apt-get install apache2
   ```
2. Habilitar módulos de proxy: Apache utiliza módulos para proporcionar funcionalidades adicionales. Para que Apache funcione como un proxy inverso, necesitarás habilitar los módulos proxy, proxy_http y proxy_balancer.
   ```
   sudo a2enmod proxy
   sudo a2enmod proxy_http
   sudo a2enmod proxy_balancer
   sudo a2enmod lbmethod_byrequests
   sudo systemctl restart apache2
   ```
3. Configurar el proxy inverso: Ahora necesitarás configurar Apache para que actúe como un proxy inverso. Esto se hace modificando el archivo de configuración de Apache. En este archivo, puedes especificar a qué servidores back-end debe reenviar las solicitudes Apache.
   
4. Reiniciar Apache: Por último, necesitarás reiniciar Apache para que los cambios surtan efecto:
   ```
   sudo systemctl restart apache2
   ```
### Sockets


### Aspectos Logrados y No logrados
Aspectos Logrados:
- Desarrollamos el cliente HTTP capaz de realizar peticiones HTTP/1.1, registrar las solicitudes y almacenar las respuestas. Implementamos la capacidad de solicitar recursos web utilizando los métodos GET y HEAD, y también incorporamos la funcionalidad para borrar la caché de la aplicación utilizando el método FLUSH.
- El servidor HTTP Proxy + Balanceador de Carga fue implementado en lenguaje C y logró cumplir con los requisitos funcionales de interceptar las solicitudes HTTP/1.1 y enviarlas a un conjunto de servidores web operativos en el backend, devolviendo las respuestas al cliente a través del servidor proxy.
- Desplegamos tres servidores web que ofrecen una página web idéntica.

Aspectos No Logrados:
- No logramos subir el proxy a la instancia de AWS, lo que impidió que el cliente pudiera comunicarse con los servidores y recibir respuestas. A pesar de los múltiples intentos, encontramos dificultades al intentar subir el proxy, lo que afectó la integración completa del sistema. Aunque cada componente (cliente, servidores y proxy) funcionó de manera independiente, la integración completa se vio obstaculizada por este problema.


### Conclusiones

El proyecto representó una valiosa oportunidad para explorar y aplicar conceptos clave en el desarrollo de aplicaciones web y redes. A través de la implementación exitosa de un cliente HTTP, servidores web y un proxy inverso con balanceador de carga, pudimos profundizar nuestra comprensión de temas como la gestión de solicitudes, el enrutamiento de tráfico y la distribución de carga. Si bien enfrentamos obstáculos al intentar subir el servidor proxy a AWS, esta experiencia nos enseñó importantes lecciones sobre la resolución de problemas, la depuración y la importancia de la planificación adecuada en el despliegue de infraestructura. Además, el trabajo en equipo y la colaboración fueron fundamentales para superar desafíos y avanzar en el proyecto. En última instancia, aunque no alcanzamos todos nuestros objetivos, el conocimiento adquirido y las habilidades desarrolladas durante este proyecto son invaluables y nos preparan para futuros proyectos y desafíos en el campo de la informática y las redes.



## Referencias
https://www.ionos.es/digitalguide/servidores/know-how/que-es-un-servidor-proxy-inverso/#:~:text=Balanceo%20de%20carga%3A%20un%20servidor,solicitudes%20entrantes%20por%20varios%20servidores.
https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-recv-receive-data-socket
