import paho.mqtt.client as mqtt 
from random import randrange, uniform
from random import randint
import time

broker = 'broker.emqx.io'
port = 1883

client = mqtt.Client("sersor")
client.connect(broker, port)

while True:
    client.publish("SBC/VOLTAGE", "2")
    time.sleep(5)