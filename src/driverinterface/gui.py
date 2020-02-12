#Note: I called the input from the serial monitor parser_input -- make sure you use that variable name in your file
import tkinter as tkinter
from matplotlib import pyplot as plt, rcParams, animation, rc
import numpy as np
fig, ax = plt.subplots()

class Application(tkinter.Frame):
    def __init__(self, master=None):
      super().___init___(master)
      new_window = Tkinter.Tk()
      new_window.title(graph_monitor)
      root =Tk()
      explode = [0.01]
      labels = [0]
      nums = [0]
      level = ax.set_label(parser_input)
    def update_graphs(num):
        if(parser_input<1500 and parser_input>1000):
          ax.clear()
          ax.axis('equal')
          for x in range(1):
            nums[x] += str_num.count(str(x))
            ax.pie(nums, explode=explode, labels=labels, colors=black, shadow=True, startangle=90)
        #clear the axes for very regeneration of input
        if(parser_input>1500 and parser_input<2000):
          ax.clear()
          ax.axis('equal')
          str_num = str(num)
          for y in range(1):
            nums[y] += str_num.count(str(y))
          for x in range(1):
            nums[x] += str_num.count(str(x))
            ax.pie(nums, explode=explode, labels=labels, colors=colors, autopct='%1.1f%%', shadow=True, startangle=90)
    ani = animation.FuncAnimation(fig, update_graphs, frames=range(1000), interval=200)
    plt.show()

app = GraphPage()
app.mainloop();
