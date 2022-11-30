import paho.mqtt.client as mqtt 
import time 

broker = '10.0.0.101'
username = 'aluno'
password = '@luno*123'
port = 1883

client = mqtt.Client("Sub")

client.username_pw_set(username, password)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado")
    else:
        print("Falhou")    


def on_message(client, userdata, message):
    print("Received message: ", str(message.payload.decode("utf-8")))

client.on_connect = on_connect
client.connect(broker, port)

client.subscribe("SBC/voltage")
client.on_message = on_message

client.loop_start()

time.sleep(30)
client.loop_stop()
