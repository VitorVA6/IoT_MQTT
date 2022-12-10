import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 
import _thread as thread
from datetime import datetime, timedelta

broker = 'broker.emqx.io'
port = 1883

t = 5

client = mqtt.Client("sersor")

def on_message(client, userdata, message):
    global t
    t = int(message.payload.decode("utf-8"))

client.connect(broker, port)
client.subscribe("SBC/TIME")
client.on_message = on_message

def subscribe_loop():
    global client
    client.loop_start()
    time.sleep(100)
    client.loop_stop()

thread.start_new_thread(subscribe_loop, ())

topic = "SBC/VOLTAGE"

while True:    
    
    rand_number = uniform(2, 3.3)
    msg = str(rand_number)[0:4]
    client.publish(topic, msg)
    
    print(str(rand_number)[0:4])
    time.sleep(t)