#define Serial Serial
#define RadarSerial Serial1
#define Radar_RX_PIN 32 // Conectar al PIN 33 del ESP32
#define Radar_TX_PIN 33 // Conectar al PIN 32 del ESP32

#include <ld2410.h>
#include <PubSubClient.h> // Lib MQTT
#include "WiFiClientSecure.h" // Lib para soportar TLS
#include "credentials.h" // Certificado y wifi_pwd

const char* ssid = wifi_ssid; // SSID de la red WiFi
const char* password = wifi_pwd; // Contrasena de la red Wi-Fi
const char* mqttServer = mqtt_server_IP; // IP servidor MQTT
const int mqttPort = mqtt_server_port;
const char* mqttUser = esp_user_sensor;
const char* mqttPassword = esp_password_sensor;

const char* ca_cert =  CA_CRT;
const char* bk_cert =  BK_CRT;
const char* bk_key =  BK_KEY;

// Inicializar cliente Wifi seguro y cliente MQTT
WiFiClientSecure espClientSecure;
PubSubClient client(espClientSecure);

ld2410 radar; // Inicializar sensor y variables
uint32_t lastReading = 0;
bool presence = false;

void setup() {
 Serial.begin(115200); // Comunicacion serie con el PC
 RadarSerial.begin(256000, SERIAL_8N1, Radar_RX_PIN, Radar_TX_PIN); // UART para monitorizar el sensor
 delay(500);

 int j = 0; // Conectar sensor
 Serial.print(F("LD2410 radar sensor initialising: "));
  while (!radar.begin(RadarSerial)) {
   delay(1000);
   Serial.print(++j); Serial.print(',');
  }
  Serial.println(F("connected"));

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
}

void loop() {
  client.loop();
  delay(10);
  radar.read();
  if(radar.isConnected() && millis() - lastReading > 100)  // Verificar cada 100 ms
  {
    lastReading = millis();
    if(radar.presenceDetected()) {
      if (!presence) {
        client.publish("casa/sala", "presence");
        Serial.println("Cambio a Presencia");
        presence = true;
      }
    }
    else {
      if (presence) {
      client.publish("casa/sala", "no presence");
      Serial.println("Cambio a No presencia");
      presence = false;
      }
    }
  }
}
