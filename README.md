# IoTsecurizado
Securización de comunicaciones MQTT en un entorno real.

Se trata de la securización en domótica incluyendo en el protocolo MQTT las siguientes medidas de seguridad:
- Comunicaciones cifradas.
- Lista de acceso.
- Comprobación de certificados.

En este caso se ha creado un servidor Mosquitto en una máquina Linux y se han utilizado tres ESP32:
- Uno conectado a un LED.
- Otro con función de botón para encender y apagar el LED.
- El último conectado a un sensor de presencia para automatizar el funcionamiento del LED.

Cada ESP32 debe contener el programa "credentials.h" con las credenciales necesarias.
Cada ESP32 debe también contener "certs.h" con los certificados del cliente y del servidor.
