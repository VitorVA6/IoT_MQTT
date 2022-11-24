import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 

broker = 'broker.emqx.io'
port = 1883
client = mqtt.Client("sersor")
client.connect(broker, port)

topic = "TEMPERATURE"

while True:
    rand_number = uniform(0, 3.3)
    msg = topic + '-' + str(rand_number)[0:4]
    client.publish(topic, msg)
    print(str(rand_number)[0:4])
    time.sleep(1)