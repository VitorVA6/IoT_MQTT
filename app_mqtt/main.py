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

Window.size = (800, 600)

client = 0
topics = {}
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
        self.ids.mqtt.text = "broker.emqx.io"
        self.ids.porta.text = "1883"
        self.ids.cli.text = "Desktop"
    
    def connect_broker(self):
        global client
        broker = self.ids.mqtt.text
        port = int(self.ids.porta.text)
        id = self.ids.cli.text

        client = mqtt.Client(id)
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
    
    def on_message(self, client, userdata, message):
        global actual_plot
        global topics
        msg_list = str(message.payload.decode("utf-8")).split('-')
        if msg_list[0] in topics:
            if(len(topics[msg_list[0]][1]) < 10 ):
                topics[msg_list[0]][1].append(float(msg_list[1]))
                topics[msg_list[0]][0].text_sensor = msg_list[1]
                if actual_plot != None:
                    actual_plot.update()
            else:
                del(topics[msg_list[0]][1][0])
                topics[msg_list[0]][1].append(float(msg_list[1]))
                topics[msg_list[0]][0].text_sensor = msg_list[1]
                if actual_plot != None:
                    actual_plot.update()
    
    def subscribe_topic(self, card, topic):
        global client
        global topics
        topics[topic] = [card, []] 
        client.subscribe(topic)
        client.on_message = self.on_message
        thread.start_new_thread(self.subscribe_loop, ())

    def subscribe_loop(self):
        client.loop_start()
        time.sleep(100)
        client.loop_stop()

    def open_card(self):
        self.add_widget(TopCard())

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

    def add_card(self, name, topic):
        
        if(name != '' and topic != ''):
            new_card = Topic(text_name = name, identifier = topic)
            self.parent.ids.scroll_subs.add_widget(new_card)
            self.parent.subscribe_topic(new_card, topic)
            self.close_card()
        else:
            self.ids.msg_error.text = 'Dados inválidos'

class Topic(MDCard):
    
    def remove_topic(self):
        global topics
        self.parent.remove_widget(self)  
        client.unsubscribe(self.identifier)     
        topics.pop(self.identifier)  

class TopicPub(MDCard):
    pass

class Graphics(MDCard):
    def __init__(self, **kwargs):
        global topics
        super().__init__(**kwargs)
        self.samples = 10
        self.graph = Graph(xmin = 0, xmax = self.samples-1, 
                            ymin = 0, ymax = 3.5,
                            border_color = [0, 1, 1, 1],
                            tick_color = [0, 1, 1, 0.7],
                            x_grid = True, y_grid = True, 
                            draw_border = True, 
                            x_grid_label=True, y_grid_label = True,
                            x_ticks_major = 1, y_ticks_major = 0.5
                            )
        self.ids.graph.add_widget(self.graph)
        self.plot_y = np.array(topics['TEMPERATURE'][1])
        self.plot_x = np.arange(len(self.plot_y))
        self.plot = LinePlot(color = [0, 1, 1, 1], line_width = 1.1)
        self.plot.points = [(x, y) for x, y in zip(self.plot_x, self.plot_y)]
        self.graph.add_plot(self.plot)

    def update(self):
        self.plot_y = np.array(topics['TEMPERATURE'][1])
        self.plot_x = np.arange(len(self.plot_y))
        self.plot.points = [(x, y) for x, y in zip(self.plot_x, self.plot_y)]

    def close_card(self):
        global actual_plot
        actual_plot = None
        self.parent.remove_widget(self)

class MyApp(MDApp):               

    def build(self):   
        self.title = 'Client - MQTT' 
        self.theme_cls.theme_style = "Light"
        self.theme_cls.primary_palette = "DeepPurple"     
        self.theme_cls.material_style = "M3" 
        return Manager()

MyApp().run()