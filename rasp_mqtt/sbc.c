#include <wiringPi.h>
#include <lcd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>
#include <pthread.h>
#include <time.h>

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
char voltage[10] = "0";
char d0[10] = "0";
char d1[10] = "0";
char d2[10] = "0";
char d3[10] = "0";
char d4[10] = "0";
char d5[10] = "0";
char d6[10] = "0";
int tempo = 5;
int flag = 0;
int screen = 0;

void write_1line(char linha1[]);
void write_2line(char linha1[], char linha2[]);

void publish(MQTTClient client, char* topic, char* payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);


/*
1 - sensor analógico
2 - sensores digitais
3 - sensores digitais p2
3 - sensores digitais p3
4 - LED
5 - configuração de tempo
6 - status da conexão
*/


void *screen_manager() {
    int b1 = 0, b2 = 0, b3 = 0;
    while(1){
        b1 = digitalRead(BUTTON_1);
        b2 = digitalRead(BUTTON_2);
        b3 = digitalRead(BUTTON_3);
        
        if(b1 == LOW) {
            delay(200);
            if (screen <= 4){
                screen++;
            }
            else{
                screen = 1;
            }
            if (screen == 1){
                lcdClear(lcd);
                lcdPuts(lcd, "   Sensor A0    ");
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd, "      %sV     ", voltage);
            }
            else if (screen == 2){
                lcdClear(lcd);
                lcdPuts(lcd, " Sensores D0-D1 ");
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd, "  D0-%s  D1-%s", d0, d1);
            }
            else if (screen == 3){
                lcdClear(lcd);
                lcdPuts(lcd, " Sensores D2-D3 ");
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd, "  D2-%s  D3-%s", d2, d3);
            }
            else if (screen == 4){
                lcdClear(lcd);
                lcdPuts(lcd, " Sensores D4-D6 ");
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd, "D4-%s D5-%s D6-%s", d4, d5, d6);
            }
            else if (screen == 5){
                lcdClear(lcd);
                lcdPuts(lcd, "Intervalo (s)");
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd, "  %d", tempo);
            }
        }

        if (screen == 5){
            
            if (b2 == LOW){
                delay(200);
                if (tempo <= 55){
                    tempo = tempo + 5;
                    lcdClear(lcd);
                    lcdPuts(lcd, "Intervalo (s)");
                    lcdPosition(lcd,0,1);
                    lcdPrintf(lcd, "  %d", tempo);
                }
                else{
                    tempo = 5;
                    lcdClear(lcd);
                    lcdPuts(lcd, "Intervalo (s)");
                    lcdPosition(lcd,0,1);
                    lcdPrintf(lcd, "  %d", tempo);
                }
            }
            
            else if(b3 == LOW){
                delay(200);
                char timeConv[10];
                sprintf(timeConv, "%d", tempo);
                publish(client, "TIME", timeConv);
            }
        }

        else if(flag == 1 && screen == 1){
            lcdClear(lcd);
            lcdPuts(lcd, "   Sensor A0    ");
            lcdPosition(lcd,0,1);
            lcdPrintf(lcd, "      %sV     ", voltage);
            flag = 0;
        }
        else if(flag == 1 && screen == 2){
            lcdClear(lcd);
            lcdPuts(lcd, " Sensores D0-D1 ");
            lcdPosition(lcd,0,1);
            lcdPrintf(lcd, "  D0-%s  D1-%s", d0, d1);
            flag = 0;
        }
        else if(flag == 1 && screen == 3){
            lcdClear(lcd);
            lcdPuts(lcd, " Sensores D2-D3 ");
            lcdPosition(lcd,0,1);
            lcdPrintf(lcd, "  D2-%s  D3-%s", d2, d3);
            flag = 0;
        }
        else if(flag == 1 && screen == 4){
            lcdClear(lcd);
            lcdPuts(lcd, " Sensores D4-D6 ");
            lcdPosition(lcd,0,1);
            lcdPrintf(lcd, "D4-%s D5-%s D6-%s", d4, d5, d6);
            flag = 0;
        }
    }
    pthread_exit(NULL);
}

int main(){
    
    wiringPiSetup();
    lcd = lcdInit (2, 16, 4, RS, E, DB4, DB5, DB6, DB7, 0, 0, 0, 0);
    
    pinMode(BUTTON_1,INPUT);
	pinMode(BUTTON_2,INPUT);
	pinMode(BUTTON_3,INPUT);

    pthread_t interrupt;
    pthread_create(&interrupt, NULL, screen_manager, NULL);

    write_2line("    IoT_MQTT    ", "    MI - SD     ");

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
    MQTTClient_subscribe(client, "D2", 2);
    MQTTClient_subscribe(client, "D3", 2);
    MQTTClient_subscribe(client, "D4", 2);
    MQTTClient_subscribe(client, "D5", 2);
    MQTTClient_subscribe(client, "D6", 2);
    MQTTClient_subscribe(client, "SBC/TIME", 2);

    while(1){
        

/*
        if(screen == 5){
            if(isPressed(BUTTON_2)) {
                
                if (tempo <= 55){
                    tempo = tempo + 5;
                }else{
                    tempo = 5;
                }
                printf("%d", tempo);
            }else if (isPressed(BUTTON_3)){
                //publish(client, "TIME", payload);
                //publish(client, "SBC/TIME", payload);
                printf("%d", tempo);
            }
        }
        */
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
    /*
    if(strcmp(topicName, "SBC/TIME") == 0){
        publish(client, "TIME", payload);
        strcpy(time, payload);
    }
    */
    if(strcmp(topicName, "SBC/LED") == 0){
        publish(client, "LED", payload);
        strcpy(voltage, payload);
    }
    else if(strcmp(topicName, "VOLTAGE") == 0){
        publish(client, "SBC/VOLTAGE", payload);
        strcpy(voltage, payload);
    }
    else if(strcmp(topicName, "D0") == 0){
        publish(client, "SBC/D0", payload);
        strcpy(d0, payload);
    }
    else if(strcmp(topicName, "D1") == 0){
        publish(client, "SBC/D1", payload);
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