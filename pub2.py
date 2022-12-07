import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 

broker = 'broker.emqx.io'
port = 1883

client = mqtt.Client("sersor2")

client.connect(broker, port)

topic = "SBC/voltage"

while True:
    rand_number = uniform(2, 3.3)
    msg = str(rand_number)[0:4]
    client.publish(topic, msg)
    print(str(rand_number)[0:4])
    time.sleep(4)