import tkinter as tkinter
from matplotlib import pyplot as plt
import matplotlib.colors as mcolors
import pymysql as sql 
import mysql.connector
import serial
from pathlib import Path
from time import sleep
import pymysql as sql
import mysql.connector
import time
import serial
from datetime import time

arduino = serial.Serial('/dev/cu.usbmodem141401', 115200)
data = arduino.readline()
pieces = data.split("|")
power = []
for i in range(len(pieces)):
    if i>0:
        sub_array = pieces[i].split(">")
        power.append(sub_array[0])
    else:
        i+=1

class Application(tk.Frame):
    def___init____(self, master=None)
    super().____init____(master)
    new_window = Tk()
    new_window.title("Graph Monitor")
    time_counter=0
    while True:
        start = datetime.now() #(hour, minute, second, microsecond) --> format in which time is stored
        microsecond_start = (((start.minute*60)+(start.hour*60*60)+(start.second))*1000000) + start.microsecond
        if (((((datetime.now().minute)*60 + datetime.now().second + ((datetime.now().hour)*60*60))*1000000) + datetime.now().microsecond) - microsecond_start) mod 21 == 0:
            for i in range(len(power)):
                angle_new = ((power[i]-1000)/1000)*360
                level = ax.set_label(power[i])
                while power[i]<1500 and power[i]>1000:
                    plt.pie([2000-power[i], power[i]],colors='black', startangle=0) #look into this more
                    plt.pause(0.001)
                    plt.draw() #Used instead of show for "animation"
                while power[i]>1500 and power[i]<2000:
                    start_time = (((((datetime.now().minute)*60 + datetime.now().second + ((datetime.now().hour)*60*60))*1000000) + datetime.now().microsecond)
                    plt.pie([power[i], 2000-power[i]],colors='blue',startangle=angle_new) #look into this more
                    plt.pause(0.001)
                    plt.draw() #Used instead of show for "animation"
                    if power[i]<1500:
                        end_time = (((((datetime.now().minute)*60 + datetime.now().second + ((datetime.now().hour)*60*60))*1000000) + datetime.now().microsecond)
                        time_counter = end_time - start_time              
    `

app = GraphPage()
app.mainloop()
