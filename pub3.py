import paho.mqtt.client as mqtt 
from random import randrange, uniform
import time 

broker = '10.0.0.101'
username = 'aluno'
password = '@luno*123'
port = 1883

client = mqtt.Client("sersor2")

client.username_pw_set(username, password)

client.connect(broker, port)

topic = "SBC/TIME"

client.publish(topic, "15")

