import Tkinter as tkinter
from matplotlib import pyplot as plt
import matplotlib.colors as mcolors
from matplotlib.animation import FuncAnimation
import serial
from datetime import time
from matplotlib.backends import FigureCanvasTkAgg

arduino = serial.Serial('/dev/cu.usbmodem141401', 115200)
data = arduino.readline()
pieces = data.split("|")
power = []
for i in range(len(pieces)):
    if i>0 and i%2 != 0:
        sub_array = pieces[i].split(">")
        power.append(sub_array[0])
    else:
        i+=1
fig1, ax1 = plt.subplots()
def init_func():
    ax1.plot(self)

class Application(tk.Frame):
    def___init____(self, master=None)
    super().____init____(master)
    root=Tk()
    new_window = Tk()
    new_window.title("Graph Monitor")
    time_counter= 0
    timer = 0
    programIsRunning = True
    while programIsRunning:
        start = datetime.now() #(hour, minute, second, microsecond) --> format in which time is stored
        microsecond_start = (((start.minute*60)+(start.hour*60*60)+(start.second))*1000000) + start.microsecond
            def update_plot(i):
                data_skip = 1
                ax1.clear()
                ax1.axis('equal')
                for i in range(len(power)):
                    angle_new = ((power[i]-1000)/1000)*360
                    level = ax1.set_label(power[i])
                    while power[i]<1500 and power[i]>1000:
                        ax1.pie([2000-power[i:i+data_skip], power[i:i+data_skip]],colors='black', startangle=0) #look into this more
                    while power[i]>1500 and power[i]<2000:
                        start_time = (((((datetime.now().minute)*60 + datetime.now().second + ((datetime.now().hour)*60*60))*1000000) + datetime.now().microsecond)
                        ax1.pie([power[i:i+data_skip], 2000-power[i:i+data_skip]],colors='blue',startangle=angle_new) #look into this more
                        if power[i]<1500:
                            end_time = (((((datetime.now().minute)*60 + datetime.now().second + ((datetime.now().hour)*60*60))*1000000) + datetime.now().microsecond)
                            time_counter = end_time - start_time           
            animation = FuncAnimation(ax1, update_plot, frames=np.arange(0,len(power), data_skip), init_func = init_func, interval = 0.001)
    canvas = FigureCanvasTkAgg(ax1, root)
    canvas.draw()
    canvas.get_tk_widget().pack(side=tkinter.TOP, fill=tkinter.BOTH, expand=1)




app = GraphPage()
app.mainloop()

