import paho.mqtt.client as mqtt 
import time 

broker = 'broker.emqx.io'
port = 1883

client = mqtt.Client("Sub2")

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado")
    else:
        print("Falhou")    


def on_message(client, userdata, message):
    print(message.topic +" "+ str(message.payload.decode("utf-8")))

client.on_connect = on_connect
client.connect(broker, port)

client.subscribe("SBC/TIME")
client.on_message = on_message

client.loop_start()

time.sleep(100)
client.loop_stop()