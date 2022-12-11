from kivy.config import Config

Config.set('graphics','width',1000)
Config.set('graphics','height',800)
Config.set('graphics', 'resizable', False)
Config.set('graphics', 'custom_titlebar', True)

import paho.mqtt.client as mqtt 
import socket
import _thread as thread
import time
from kivy.clock import Clock
from kivymd.app import MDApp 
from kivy.lang import Builder
from kivy.uix.screenmanager import ScreenManager, NoTransition, SlideTransition
from kivymd.uix.screen import MDScreen
from kivymd.uix.boxlayout import MDBoxLayout
from kivymd.uix.card import MDCard
import numpy as np
from scipy.interpolate import make_interp_spline
from kivy.core.window import Window
from kivy_garden.graph import Graph, LinePlot
import matplotlib
matplotlib.use("module://kivy.garden.matplotlib.backend_kivy")
from kivy.garden.matplotlib.backend_kivyagg import FigureCanvasKivyAgg
import matplotlib.pyplot as plt


plt.style.use('grayscale')
fig = plt.figure()
fig.patch.set_alpha(0)

ax = plt.axes()
ax.patch.set_alpha(0)

y = np.array([2.1, 2.3, 1.2, 1.9, 2, 0.7, 2.7, 2.1, 1.3, 0.6])
x = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
ax.grid(color='#673AB7', linestyle='-', linewidth=.5, alpha = .3)
xlabels = [ '10th', '9th', '8th', '7th', '6th', '5th', '4th', '3th', '2th', '1th']


xy_spline = make_interp_spline(x, y)
x1 = np.linspace(x.min(), x.max(), 500)
y1 = xy_spline(x1)

plt.plot(x1, y1, color = '#673AB7')
plt.scatter(x, y, color = '#673AB7')
plt.xticks(np.arange(min(x), max(x)+1, 1.0))
plt.tick_params('both', colors = '#673AB7', bottom = False, left = False)
plt.grid(True)
plt.tight_layout()
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['left'].set_visible(False)
plt.gca().spines['bottom'].set_visible(False)
plt.gca().set_xticklabels(xlabels)


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
        self.ids.mqtt.text = "broker.emqx.io"
        self.ids.porta.text = "1883"
        self.ids.cli.text = "Desktop"
        self.ids.user.text = ""
        self.ids.password.text = ''
    
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

class Digitais(MDCard):
    pass

class Time(MDCard):
    def set_time(self):
        client.publish("SBC/TIME", self.ids.slider_time.value)

class Led(MDCard):
    def on_checkbox_active(self, checkbox, value):
        if value:
            self.ids.led_icon.icon = 'led-on'
            client.publish('SBC/LED', 'on')
        else:
            self.ids.led_icon.icon = 'led-off'
            client.publish('SBC/LED', 'off')

class TopicPub(MDCard):
    pass

class Graphics(MDBoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
            
        self.add_widget(FigureCanvasKivyAgg(plt.gcf()))

class Card_graph(MDCard):
    pass

class MyApp(MDApp):               

    def build(self):   
        self.title = 'Client - MQTT' 
        self.theme_cls.theme_style = "Dark"
        self.theme_cls.primary_palette = "DeepPurple"     
        self.theme_cls.material_style = "M3" 
        return Manager()

MyApp().run()