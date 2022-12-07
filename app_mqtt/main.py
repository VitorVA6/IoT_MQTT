import paho.mqtt.client as mqtt 
import socket
import _thread as thread
import time
from kivy.clock import Clock
from kivymd.app import MDApp 
from kivy.lang import Builder
from kivy.uix.screenmanager import ScreenManager, NoTransition, SlideTransition
from kivymd.uix.screen import MDScreen
from kivymd.uix.card import MDCard
import numpy as np
from kivy.core.window import Window
from kivy_garden.graph import Graph, LinePlot

Window.size = (900, 700)

client = 0
topics = {'analogico':[], 'd0':[], 'd1':[]}
actual_plot = None

class Manager(ScreenManager):
    pass

class DummyScreen(MDScreen):
    
    def on_enter(self):
        Clock.schedule_once(self.switch_screen)
    
    def switch_screen(self, dt):
        self.manager.transition = NoTransition()
        self.manager.current = "connect"
        self.manager.transition = SlideTransition()

class ConScreen(MDScreen):
    
    def on_enter(self, *args):
        self.ids.mqtt.text = "10.0.0.101"
        self.ids.porta.text = "1883"
        self.ids.cli.text = "Desktop"
        self.ids.user.text = "aluno"
        self.ids.password.text = '@luno*123'
    
    def connect_broker(self):
        global client
        broker = self.ids.mqtt.text
        port = int(self.ids.porta.text)
        id = self.ids.cli.text
        username = self.ids.user.text
        password = self.ids.password.text

        client = mqtt.Client(id)
        client.username_pw_set(username, password)
        if (broker != '' and port != '' and id != ''):
            try:
                client.connect(broker, port)
                MDApp.get_running_app().root.current = "topics"
                self.ids.msg.text = ''
            except (socket.gaierror, socket.timeout, ValueError):
                self.ids.msg.text = 'Connection failed'
        else:
            self.ids.msg.text = 'Missing data'

class TopicScreen(MDScreen):

    def on_enter(self, *args):
        global client
        client.subscribe('SBC/voltage')
        client.on_message = self.on_message
        thread.start_new_thread(self.subscribe_loop, ())
    
    def on_message(self, client, userdata, message):
        global actual_plot
        global topics
        msg = str(message.payload.decode("utf-8"))
        if message.topic == 'SBC/voltage':
            if(len(topics['analogico']) < 10 ):
                topics['analogico'].append(float(msg))
                self.ids.sensor_an.text_sensor = msg
                if actual_plot != None:
                    actual_plot.update()
            else:
                del(topics['analogico'][0])
                topics['analogico'].append(float(msg))
                self.ids.sensor_an.text_sensor = msg
                if actual_plot != None:
                    actual_plot.update()        

    def subscribe_loop(self):
        client.loop_start()
        time.sleep(100)
        client.loop_stop()

    def back_start(self):
        client.disconnect()
        MDApp.get_running_app().root.current = "connect"

    def create_graph(self):
        global actual_plot
        actual_plot = Graphics()
        self.add_widget(actual_plot)

class TopCard(MDCard):
    
    def close_card(self):
        self.parent.remove_widget(self)


class Topic(MDCard):
    
    pass 

class TopicPub(MDCard):
    pass

class Graphics(MDCard):
    def __init__(self, **kwargs):
        global topics
        super().__init__(**kwargs)
        self.samples = 10
        self.graph = Graph(xmin = 0, xmax = self.samples-1, 
                            ymin = 0, ymax = 3.5,
                            border_color = [1, 1, 1, 1],
                            tick_color = [1, 1, 1, 1],
                            x_grid = True, y_grid = True, 
                            draw_border = True, 
                            x_grid_label=True, y_grid_label = True,
                            x_ticks_major = 2, y_ticks_major = 1
                            )
        self.ids.graph.add_widget(self.graph)
        self.plot_y = np.array(topics['analogico'])
        self.plot_x = np.arange(len(self.plot_y))
        self.plot = LinePlot(color = [1, 1, 1, 1], line_width = 1.1)
        self.plot.points = [(x, y) for x, y in zip(self.plot_x, self.plot_y)]
        self.graph.add_plot(self.plot)

    def update(self):
        self.plot_y = np.array(topics['analogico'])
        self.plot_x = np.arange(len(self.plot_y))
        self.plot.points = [(x, y) for x, y in zip(self.plot_x, self.plot_y)]

    def close_card(self):
        global actual_plot
        actual_plot = None
        self.parent.remove_widget(self)

class MyApp(MDApp):               

    def build(self):   
        self.title = 'Client - MQTT' 
        self.theme_cls.theme_style = "Dark"
        self.theme_cls.primary_palette = "DeepPurple"     
        self.theme_cls.material_style = "M3" 
        return Manager()

MyApp().run()