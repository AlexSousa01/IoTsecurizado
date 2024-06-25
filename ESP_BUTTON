#include <PubSubClient.h> // Lib MQTT
#include "WiFiClientSecure.h" // Lib para soportar TLS
#include "credentials.h" // Certificado y wifi_pwd

const char* ssid = wifi_ssid; // SSID de la red WiFi
const char* password = wifi_pwd; // Contrasena de la red Wi-Fi
const char* mqttServer = mqtt_server_IP; // IP servidor MQTT
const int mqttPort = mqtt_server_port;
const char* mqttUser = esp_user_button;
const char* mqttPassword = esp_password_button;

const char* ca_cert =  CA_CRT;
const char* bk_cert =  BK_CRT;
const char* bk_key =  BK_KEY;

// Inicializar cliente Wifi seguro y cliente MQTT
WiFiClientSecure espClientSecure;
PubSubClient client(espClientSecure);

const int pinButton = 21; // PIN donde se ubica el botón
bool buttonPressed = false; // Flag para habilitar la Interrupción

void setup() {
 Serial.begin(115200); // Comunicacion serie con el PC
 delay(10);
 Serial.println('\n');

 WiFi.begin(ssid, password); // Conectar WiFi
 Serial.print("Connecting to ");
 Serial.print(ssid); Serial.println(" ...");
 int i = 0;
 while (WiFi.status() != WL_CONNECTED) { // Esperar conexion
  delay(1000);
  Serial.print(++i); Serial.print(',');
 }
 Serial.println('\n');
 Serial.println("Connection established!"); 
 Serial.print("IP address:\t");
 Serial.println(WiFi.localIP()); // Imprimir IP del ESP

 espClientSecure.setCACert(ca_cert); // Establecer certificado
 espClientSecure.setCertificate(bk_cert);  // Certificados para verificar el servidor
 espClientSecure.setPrivateKey(bk_key);
 espClientSecure.setInsecure();

 client.setServer(mqttServer, mqttPort);
 while (!client.connected()) {
  Serial.println("Connecting to MQTT...");
  if (client.connect("", mqttUser, mqttPassword)) {
   Serial.println("connected");
  } 
  else {
   Serial.print("failed \n");
   delay(2000);
  }
 }
 pinMode(pinButton, INPUT_PULLUP); // Activamos resistencia interna (Pull up)
 attachInterrupt(digitalPinToInterrupt(pinButton), buttonInterrupt, FALLING);
}

void buttonInterrupt() {
  buttonPressed = true; // Establece el indicador de pulsación del botón en verdadero
}

void loop() {
  client.loop();
  delay(10);
  if (buttonPressed) {
    // Publica un mensaje MQTT indicando que el botón se ha presionado
    client.publish("casa/sala", "button");
    buttonPressed = false; // Reinicia el indicador de pulsación del botón
    delay(500); // Esperar para volver a pulsar
  }
}
