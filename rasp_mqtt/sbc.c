#include <wiringPi.h>
#include <lcd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

#define RS 25
#define E 1
#define DB4 12
#define DB5 16
#define DB6 20
#define DB7 21

#define MQTT_ADDRESS "tcp://10.0.0.101"
#define USERNAME "aluno"
#define PASSWORD "@luno*123"
#define CLIENTID "MQTTCClientID"

#define MQTT_PUBLISH_TOPIC "SBC/voltage"
#define MQTT_SUBSCRIBE_TOPIC "LEDS/voltage"

MQTTClient client;
int lcd;

void publish(MQTTClient client, char* topic, char* payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;

    printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName, payload);

    publish(client, MQTT_PUBLISH_TOPIC, payload);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main(){
    
    wiringPiSetup();
    lcd = lcdInit (2, 16, 4, RS, E, DB4, DB5, DB6, DB7, 0, 0, 0, 0);

    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    rc = MQTTClient_connect(client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS){
        printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
        exit(-1);}
    MQTTClient_subscribe(client, MQTT_SUBSCRIBE_TOPIC, 0);

    while(1){
    }
}