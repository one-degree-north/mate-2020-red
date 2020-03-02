import serial
import sys
import pygame
from pygame.locals import (
    K_w,
    K_a,
    K_s,
    K_d,
    K_i,
    K_j,
    K_k,
    K_l,
    K_ESCAPE,
    KEYDOWN,
    QUIT,
)

# CONSTANTS
ARDUINO_BAUDRATE = 115200
ARDUINO_PORT = '/dev/cu.usbmodem143101'

# SETUP
ser = serial.Serial(ARDUINO_BAUDRATE, ARDUINO_PORT)

class Screen:
    def __init__(self, width, height): 
        self.width = width
        self.height = height

    def get_width(self):
        return self.width

    def get_height(self):
        return self.height

    def get_size(self):
        return (self.width, self.height)

def main():
    pass

def setup_app():
    
    # SETUP PYGAME
    pygame.init()
    pass

def run_app():
    while True:
        for event in pygame.event.get():

    pass

def quit_app():
    print("Quitting app...")
    ser.close()
    sys.exit()
