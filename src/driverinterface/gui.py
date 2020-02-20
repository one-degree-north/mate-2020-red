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
import serial
import time

mydb = mysql.connector.connect(
    host = "localhost",
    user = "root",
    passwd = "password123",
    database="testdb"
    )

arduino = serial.Serial('/dev/cu.usbmodem141401', 115200)
data = arduino.readline()
time.sleep(1) # wait before reading next line tabbed split info
data = arduino.readline()
pieces = data.split(">")

pieces[0] = time
pieces[1] = power_level

with mydb:
    mycursor = mydb.cursor()
    # mycursor.execute("CREATE TABLE output(power_level INTEGER(10), time INTEGER(10))") 
    mycursor.execute("""INSERT INTO output VALUES('',%s,%s)""", (power_level, time))
    mydb.commit()
    mycursor.close() 


class Application(tk.Frame):
    def___init____(self, master=None)
    super().____init____(master)
    new_window = Tkinter.Tk()
    new_window.title("Graph Monitor")
    parser_input = []
    time_input = []
    mycursor = x.mydb.mycursor()
    parser_input.append(mycursor.execute("SELECT power_level.*")) #recieves all of the input and plugs it into parser_input array
    time_input.append(mycursor.execute("SELECT time.*"))
    root=Tk()
    labels = [0]
    nums = [0]
    time_counter = 0
    angle_new = ((parser_input[i]-1000)/1000)*360
    level = ax.set_label(parser_input)
    def update_graphs(parser_input):
        for i in range(0, len(parser_input)):
            while parser_input<1500 and parser_input>1000:
                plt.pie([2000-parser_input[i], parser_input[i]],colors='black', startangle=0) #look into this more
                plt.pause(0.001)
                plt.draw() #Used instead of show for "animation"
                sleep(1)
            while parser_input>1500 and parser_input<2000:
                start_time = time_input[i]
                plt.pie([parser_input[i], 2000-parser_input[i]],colors='blue',startangle=angle_new) #look into this more
                plt.pause(0.001)
                plt.draw() #Used instead of show for "animation"
                sleep(1)
                if parser_input[i+1]<1500:
                    end_time = time_input[i+1]
                time_counter = end_time - start_time
        print time_counter

            
    update_graphs(parser_input, time_input)
app = GraphPage()
app.mainloop();
