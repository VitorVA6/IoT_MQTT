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
char led[10] = "OFF";
int tempo = 5;
int status_con = 2;

int flag = 0;
int screen = 0;

void write_1line(char linha1[]);
void write_2line(char linha1[], char linha2[]);

void publish(MQTTClient client, char* topic, char* payload);
int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);

void screen_1(){
    lcdClear(lcd);
    lcdPuts(lcd, "   Sensor A0    ");
    lcdPosition(lcd,0,1);
    lcdPrintf(lcd, "      %sV     ", voltage);
}

void screen_2(){
    lcdClear(lcd);
    lcdPuts(lcd, " Sensores D0-D1 ");
    lcdPosition(lcd,0,1);
    lcdPrintf(lcd, "  D0-%s  D1-%s", d0, d1);
}

void screen_3(){
    lcdClear(lcd);
    lcdPuts(lcd, " Sensores D2-D3 ");
    lcdPosition(lcd,0,1);
    lcdPrintf(lcd, "  D2-%s  D3-%s", d2, d3);
}

void screen_4(){
    lcdClear(lcd);
    lcdPuts(lcd, " Sensores D4-D6 ");
    lcdPosition(lcd,0,1);
    lcdPrintf(lcd, "D4-%s D5-%s D6-%s", d4, d5, d6);
}

void screen_5(){
    lcdClear(lcd);
    lcdPuts(lcd, "      LED       ");
    lcdPosition(lcd,0,1);
    lcdPrintf(lcd, "        %s", led);
}

void screen_6(){
    lcdClear(lcd);
    lcdPuts(lcd, "Intervalo (s)");
    lcdPosition(lcd,0,1);
    lcdPrintf(lcd, "  %d", tempo);
}

void screen_7(){
    if(status_con == 1 || status_con == 2){
        lcdClear(lcd);
        lcdPuts(lcd, "   Connection   ");
        lcdPosition(lcd,0,1);
        lcdPuts(lcd, "     ONLINE     ");
    }    
    else{
        lcdClear(lcd);
        lcdPuts(lcd, "   Connection   ");
        lcdPosition(lcd,0,1);
        lcdPuts(lcd, "    OFFLINE     ");
    }
}

void *screen_manager() {
    int b1 = 0, b2 = 0, b3 = 0;
    while(1){
        b1 = digitalRead(BUTTON_1);
        b2 = digitalRead(BUTTON_2);
        b3 = digitalRead(BUTTON_3);
        
        if(b1 == LOW) {
            delay(200);
            if (screen <= 6){
                screen++;
            }
            else{
                screen = 1;
            }
            if (screen == 1){
                screen_1();
            }
            else if (screen == 2){
                screen_2();
            }
            else if (screen == 3){
                screen_3();
            }
            else if (screen == 4){
                screen_4();
            }
            else if (screen == 5){
                screen_5();
            }
            else if (screen == 6){
                screen_6();
            }
            else if (screen == 7){
                screen_7();
            }
        }

        if (screen == 6){
            
            if (b2 == LOW){
                delay(200);
                if (tempo <= 55){
                    tempo = tempo + 5;
                    screen_6();
                }
                else{
                    tempo = 5;
                    screen_6();
                }
            }
            
            else if(b3 == LOW){
                delay(200);
                char timeConv[10];
                sprintf(timeConv, "%d", tempo);
                publish(client, "TIME", timeConv);
            }
        }

        else if (screen == 5){
            
            if (b2 == LOW){
                delay(200);
               
                publish(client, "LED", led);
                
            }           
        }

        else if(flag == 1 && screen == 1){
            screen_1();
            flag = 0;
        }
        else if(flag == 1 && screen == 2){
            screen_2();
            flag = 0;
        }
        else if(flag == 1 && screen == 3){
            screen_3();
            flag = 0;
        }
        else if(flag == 1 && screen == 4){
            screen_4();
            flag = 0;
        }
    }
    pthread_exit(NULL);
}

void *send_connection(){
    while(1){
        publish(client, "NODESTATUS", "CON");
        delay(10000);
        if(status_con == 1){
            status_con = 2;
        }
        else{
            status_con = 0;
        }
        if(screen == 7){
            screen_7();
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

    pthread_t interrupt, statust;
    pthread_create(&interrupt, NULL, screen_manager, NULL);
    pthread_create(&statust, NULL, send_connection, NULL);

    write_2line("    IoT_MQTT    ", "    MI - SD     ");

    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 1000;
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
    MQTTClient_subscribe(client, "LEDANS", 2);
    MQTTClient_subscribe(client, "TIMEANS", 2);
    MQTTClient_subscribe(client, "SBC/TIME", 2);
    MQTTClient_subscribe(client, "SBC/LED", 2);
    MQTTClient_subscribe(client, "STATUSCON", 2);

    while(1){
        
    }
}

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 1;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;
    
    if(strcmp(topicName, "SBC/TIME") == 0){
        publish(client, "TIME", payload);
    }
    
    if(strcmp(topicName, "SBC/LED") == 0){
        publish(client, "LED", payload);
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

    else if(strcmp(topicName, "LEDANS") == 0){
        puts(payload);
        publish(client, "APP/LED", payload);
        strcpy(led, payload);        
        if (screen == 5){
            screen_5();  
        }        
    }

    else if(strcmp(topicName, "TIMEANS") == 0){
        puts(payload);
        publish(client, "APP/TIME", payload);
        tempo = atoi(payload);
        if(screen == 6){
            screen_6();
        }    
    }

    else if(strcmp(topicName, "STATUSCON") == 0){
        status_con = 1;  
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