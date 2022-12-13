#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TimedAction.h>


//Definicoes
#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

//Constantes e variaveis
const char* ssid = STASSID;
const char* password = STAPSK;

const char* mqtt_server = "10.0.0.101";
const char *mqtt_username = "aluno";
const char *mqtt_password = "@luno*123";

WiFiClient espClient;
PubSubClient client(espClient);

int tempo = 5;

void publishTopic(){
  float valor = analogRead(A0)*(3.3/1023.0);
  char valorConv[4];
  sprintf(valorConv, "%.1f", valor);
  client.publish("VOLTAGE", valorConv);
  
  int d0Value = digitalRead(D0);
  char d0valueConv[1];
  sprintf(d0valueConv, "%d", d0Value);
  client.publish("D0", d0valueConv);

  int d1Value = digitalRead(D1);
  char d1valueConv[1];
  sprintf(d1valueConv, "%d", d1Value);
  client.publish("D1", d1valueConv);
}

TimedAction proc_aux = TimedAction(2000, publishTopic);

void callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic,"TIME") == 0){
    char aux[10];
    
    for (int i=0;i<length;i++) {
      aux[i] = (char)payload[i];
    }
    tempo = atoi(aux);
    proc_aux.setInterval(tempo*1000);
  }
}

void connectWifi_OTA(){
  Serial.begin(9600);
  delay(10);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();}

  ArduinoOTA.setHostname("ESP-10.0.0.108");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect_MQTT(){
  Serial.print("Attempting MQTT connection...");
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
    Serial.println("connected");
    client.publish("outTopic", "hello world");
    client.subscribe("TIME");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(2000);
  }
}

void setup() {
  connectWifi_OTA();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect_MQTT();
  }
  proc_aux.check();
  client.loop();
}