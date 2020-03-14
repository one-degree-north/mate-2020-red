#pygame graph_update.py

import pygame as pg
import serial 
import matplotlib as mlt
from matplotlib import pyplot
from datetime import *


class Graph(object): #no need for object, just temporary
    def pygame_init():
        pygame.init()
        start = datetime.now()
        time = (((start.minute*60)+(start.hour*60*60)+(start.second))*1000000) + start.microsecond
        return time
    
    def pygame_quit(button_click):
        pygame.quit()

    def init_graph(data_serial):
        names = [ax0,ax1,ax2,ax3,ax4,ax5,ax6,ax7]
        all_powers = data_serial()
        for i in range(0, len(all_powers[0])):
            names[i].plot(self) #main method self?
        
    def data_serial():
        arduino =  serial.Serial('/dev/cu.usbmodem141401', 115200)
        data = arduino.readlines() #creates list for multiple lines through serial_monitor
        data.split(">")
        time = pygame_init()
        for line in data:
            line.split("|")
        while ((((datetime.now().minute*60)+(datetime.now().hour*60*60)+(datetime.now().second))*1000000) + datetime.now().microsecond - time%21) == 0 #time in microseconds
            for i in data:
                if i == 0:
                    powers.append([data[i][1],data[i][2]])
                elif i == 1:
                    powers.append([data[i][1], data[i][2]])
                elif i > 1:
                        powers.append(data[i][1])
                powers_new = []
                powers_new.append(powers[0][0])
                powers_new.append(powers[0][1])
                powers_new.append(powers[1][0])
                powers_new.append(powers[1][1])
                powers_new.append(powers[2])
                powers_new.append(powers[3])
                powers_new.append(powers[4])
                powers_new.append(powers[5])
                all_powers.append(powers_new)
                powers = []
        return all_powers

    def graph_object(data_serial):
        MAX=2000
        pygame.display.init()
        start = pygame.display.get_init()
        if start != True:
            pygame.display.init()
        else:
            pygame.display.set_mode(size=(500,500), flags = 0)
        all_powers = data_serial()
        fig, axs = plt.subplot(len(all_powers[0]))
        fig.suptitle("Power charts for different motors")
        data_skip = 1
        names = [ax0,ax1,ax2,ax3,ax4,ax5,ax6,ax7]
        for c in range(0, len(all_powers)):
            for i in range(0, len(all_powers[c])):
                angle_new = ((all_powers[c][i]-1000)/1000)*360
                level = axs[i].set_label(all_powers[i][c])
                while all_powers[c][i]> 1500 and all_powers[c][i]<2000:
                    axs[i] = names[i].pie([all_powers[c][i:i+data_skip],MAX-all_powers[c][i:i+data_skip]], colors = "blue", startangle = angle_new)
                while all_powers[c][i]<1500 and all_powers[c][i]>1000:
                    axs[i] = names[i].pie([MAX-all_powers[c][i:i+data_skip], all_powers[c][i:i+data_skip]], colors = "black", startangle = 0)

        pygame.display.update()

    def main():
        dif_time = time
        pygame_init()
        data_serial()
        graph_object()
        while True: # updating the plot using FuncAnimation
            for i in range(0, 8):
                animation = FuncAnimation(axs[i], graph_object, frames=np.arange(0,len(all_powers), data_skip), init_func = graph_object, interval = 0.001)
            plt.show()
        pygame_quit()

                    
