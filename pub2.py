import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 

broker = 'broker.emqx.io'
port = 1883
client = mqtt.Client("emissor")
client.connect(broker, port)

topic = "VOLUME"

while True:
    rand_number = uniform(0, 1)
    msg = topic + '-' + str(rand_number)[0:4]
    client.publish(topic, msg)
    print(str(rand_number)[0:4])
    time.sleep(1)