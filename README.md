# Proxy inverso con APACHE y Balanceo de carga
## Integrantes:
- Moisés Arrieta Hernández
- Jerónimo Pérez Baquero
- Juan José Vélez

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

## Referencias
https://www.ionos.es/digitalguide/servidores/know-how/que-es-un-servidor-proxy-inverso/#:~:text=Balanceo%20de%20carga%3A%20un%20servidor,solicitudes%20entrantes%20por%20varios%20servidores.
https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-recv-receive-data-socket
