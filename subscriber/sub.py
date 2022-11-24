import paho.mqtt.client as mqtt 
import time 

broker = 'broker.emqx.io'
port = 1883
client = mqtt.Client("Sub")

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado")
    else:
        print("Falhou")    


def on_message(client, userdata, message):
    print("Received message: ", str(message.payload.decode("utf-8")))

client.on_connect = on_connect
client.connect(broker, port)

client.subscribe("LEDS/voltage")
client.on_message = on_message

client.loop_start()

time.sleep(30)
client.loop_stop()
