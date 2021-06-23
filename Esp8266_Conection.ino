
#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>



/*
#define sensor1 D2
#define Ptrig1 D8

void setup() {                
  Serial.begin (9600);
  pinMode(sensor1, INPUT);
  pinMode(Ptrig1, OUTPUT);

  void loop() {
  
  digitalWrite(Ptrig1, LOW);
  delayMicroseconds(2);

  digitalWrite(Ptrig1, HIGH);  
  delayMicroseconds(10);
  
  duracion = pulseIn(sensor1, HIGH);
  distancia = (duracion/2) / 29;         
  
  if (distancia >= 500 || distancia <= 0){  
    Serial.println("---");                 
  }
  else {
    Serial.print(distancia);          
    Serial.println("cm");             
    digitalWrite(13, 0);               
    digitalWrite(4, 1); 
  } 
  
   if (distancia <= 10 && distancia >= 1){
    digitalWrite(13, 1);                    
    digitalWrite(4, 0); 
    Serial.println("Alarma.......");        
  }
  delay(400);

*/

//==============================================
//             CONFIGURACIÓN WIFI 
//==============================================
const char* ssid = "ARRIS-EFF2"; //Nombre de la red
const char* password =  "GearS2255"; //Contraseña de la red



//==============================================
//             CONFIGURACIÓN MQTT
//==============================================
const char *mqtt_server = "192.168.0.8"; // Insertar la dirección IP o la dirección del servidor MQTT
const int mqtt_port = 1883;
const char *root_topic_subscribe = "/topico/input"; //Insertar el topico de entrada
const char *root_topic_publish = "/topico/output";  //Insertar el topico de salida
const char *mqtt_user = "";
const char *mqtt_pass = "";


//==============================================
//                  GLOBALES   
//==============================================
WiFiClient espClient;
PubSubClient client(espClient);
char msg[25];
String temperatura = "25 °C";


//==============================================
//                 F U N C I O N E S 
//==============================================
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }

  if (client.connected()){
    Serial.print("cliente conectado");
  //  String str = "Temperatura = " + String(temperatura);
  //  str.toCharArray(msg,25);
   //  client.publish(root_topic_publish,msg);
    delay(10000);
  }
  client.loop();
}

//==============================================
//              CONEXION WIFI      
//==============================================
void setup_wifi(){
  delay(10);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

//*****************************
//***    CONEXION MQTT      ***
//*****************************

void reconnect() {

  while (!client.connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "ESP-UV_";
    clientId += String(random(0xffff), HEX) ;
    // Intentamos conectar
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      // Nos suscribimos

      if(client.subscribe(root_topic_subscribe)){
        Serial.println("Suscripcion ok");
      }else{
        Serial.println("fallo Suscripciión");
      }
    } else {
      Serial.print("falló :( con error ===> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
  }
}


//==============================================
//                 CALLBACK        
//==============================================

void callback(char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print("Se recibió mensaje:  ");
  Serial.print(topic);
  Serial.println("");
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje  ==>  " + incoming + "\n\n");
  client.publish(root_topic_publish,"30° C");

}
