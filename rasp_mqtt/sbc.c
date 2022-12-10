#include <wiringPi.h>
#include <lcd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

#define RS 13
#define E 18
#define DB4 21
#define DB5 24
#define DB6 26
#define DB7 27

#define BUTTON_1 19
#define BUTTON_2 23
#define BUTTON_3 25

#define MQTT_ADDRESS "tcp://10.0.0.101"
#define USERNAME "aluno"
#define PASSWORD "@luno*123"
#define CLIENTID "MQTTCClientID"
#define QOS 0

MQTTClient client;
int lcd;
char voltage[10];
char d0[10];
char d1[10];
char time[10] = "5";
int flag = 0;

void write_1line(char linha1[]);
void write_2line(char linha1[], char linha2[]);

void publish(MQTTClient client, char* topic, char* payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);

int main(){
    
    wiringPiSetup();
    lcd = lcdInit (2, 16, 4, RS, E, DB4, DB5, DB6, DB7, 0, 0, 0, 0);
    
    pinMode(BUTTON_1,INPUT);
	pinMode(BUTTON_2,INPUT);
	pinMode(BUTTON_3,INPUT);

    write_2line("IoT_MQTT", "MI - SD");

    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 60;
	conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    rc = MQTTClient_connect(client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS){
        printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
        exit(-1);}
    MQTTClient_subscribe(client, "VOLTAGE", 2);
    MQTTClient_subscribe(client, "D0", 2);
    MQTTClient_subscribe(client, "D1", 2);
    MQTTClient_subscribe(client, "SBC/TIME", 2);

    while(1){
        if(flag == 1){
            lcdClear(lcd);
            lcdPrintf(lcd, "Intervalo: %ss", time);
            lcdPosition(lcd,0,1);
            lcdPrintf(lcd, "A0-%s D0-%s D1-%s", voltage, d0, d1);
            flag = 0;
        }
    }
}

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;

    if(strcmp(topicName, "SBC/TIME") == 0){
        publish(client, "TIME", payload);
        strcpy(time, payload);
    }
    else if(strcmp(topicName, "SBC/LED") == 0){
        publish(client, "LED", payload);
        strcpy(voltage, payload);
    }
    else if(strcmp(topicName, "VOLTAGE") == 0){
//        publish(client, "SBC/VOLTAGE", payload);
        strcpy(voltage, payload);
    }
    else if(strcmp(topicName, "D0") == 0){
//        publish(client, "SBC/D0", payload);
        strcpy(d0, payload);
    }
    else if(strcmp(topicName, "D1") == 0){
//        publish(client, "SBC/D1", payload);
        strcpy(d1, payload);
        flag = 1;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void write_1line(char linha1[]) {
	lcdHome(lcd);
	lcdPuts(lcd, linha1);
	lcdPosition(lcd,0,1);
	lcdPuts(lcd, "                ");
}

void write_2line(char linha1[], char linha2[]) {
	lcdHome(lcd);
	lcdPuts(lcd, linha1);
	lcdPosition(lcd,0,1);
	lcdPuts(lcd, linha2);
}