import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 

broker = '10.0.0.101'
username = 'aluno'
password = '@luno*123'
port = 1883

client = mqtt.Client("sersor")

client.username_pw_set(username, password)

client.connect(broker, port)

topic = "LEDS/voltage"

while True:
    rand_number = uniform(0, 3.3)
    msg = topic + '-' + str(rand_number)[0:4]
    client.publish(topic, msg)
    print(str(rand_number)[0:4])
    time.sleep(4)