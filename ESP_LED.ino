#include <PubSubClient.h> // Lib MQTT
#include "WiFiClientSecure.h" // Lib para soportar TLS
#include "credentials.h" // Certificado y wifi_pwd

const char* ssid = wifi_ssid; // SSID de la red WiFi
const char* password = wifi_pwd; // Contrasena de la red Wi-Fi
const char* mqttServer = mqtt_server_IP; // IP servidor MQTT
const int mqttPort = mqtt_server_port;
const char* mqttUser = esp_user_led;
const char* mqttPassword = esp_password_led;

const char* ca_cert =  CA_CRT;
const char* bk_cert =  BK_CRT;
const char* bk_key =  BK_KEY;

// Inicializar cliente Wifi seguro y cliente MQTT
WiFiClientSecure espClientSecure;
PubSubClient client(espClientSecure);

const int pinLED = 33;
int stateLED = 0;

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
 client.subscribe("casa/sala");
 client.setCallback(callback); // callback al recibir un mensaje
 pinMode(pinLED, OUTPUT); // Establecer PIN en modo OUTPUT
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.println("Mensaje recibido en [casa/sala]");
 if((char)payload[0] == 'b') {
  if (stateLED == 0) {
   digitalWrite(pinLED, HIGH); // Encender LED (botón y estaba apagado)
   Serial.println("LED encendido");
   stateLED = 1;
  }
  else {
   digitalWrite(pinLED, LOW); // Apagar LED (botón y estaba encendido)
   Serial.println("LED apagado");
   stateLED = 0;
  }
 }
 else if((char)payload[0] == 'p') {
  digitalWrite(pinLED, HIGH); // Encender LED (hay alguien)
  Serial.println("LED encendido");
  stateLED = 1;
 }
 else {
  digitalWrite(pinLED, LOW); // Apagar LED (no hay nadie)
  Serial.println("LED apagado");
  stateLED = 0;
 }
 sendState();
}

void sendState() {
 client.publish("casa/sala/estado", String(stateLED).c_str());
 Serial.println("Mensaje enviado: "); Serial.println(stateLED);
}

void loop() {
  client.loop();
  delay(10);
 }
