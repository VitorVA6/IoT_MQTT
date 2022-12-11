import paho.mqtt.client as mqtt 
from random import randrange, uniform
from random import randint
import time

broker = 'broker.emqx.io'
port = 1883

client = mqtt.Client("sersor")
client.connect(broker, port)

while True:    
    
    k = str(randint(0, 1))

    client.publish("SBC/VOLTAGE", str(uniform(2, 3.3))[0:4])
    client.publish("SBC/D0", str(str(randint(0, 1))))
    client.publish("SBC/D1", str(str(randint(0, 1))))
    client.publish("SBC/D2", str(str(randint(0, 1))))
    client.publish("SBC/D3", str(str(randint(0, 1))))
    client.publish("SBC/D4", str(str(randint(0, 1))))
    client.publish("SBC/D5", str(str(randint(0, 1))))
    client.publish("SBC/D6", str(str(randint(0, 1))))

    time.sleep(5)