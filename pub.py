import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 

broker = '10.0.0.101'
username = 'aluno'
password = '@luno*123'
port = 1883

client = mqtt.Client("sensor")

client.username_pw_set(username, password)

client.connect(broker, port)

topic = "LEDS/voltage"
topic2 = "LEDS/D0"
topic3 = "LEDS/D1"

while True:
    rand_number = uniform(0, 1.3)
    rand_number2 = uniform(2, 3.3)
    rand_number3 = uniform(4, 5.3)

    msg = str(rand_number)[0:4]
    msg2 = str(rand_number2)[0:4]
    msg3 = str(rand_number3)[0:4]

    client.publish(topic, msg, retain=True)
    
    client.publish(topic2, msg2, retain=True)
    
    client.publish(topic3, msg3, retain=True)

    time.sleep(5)