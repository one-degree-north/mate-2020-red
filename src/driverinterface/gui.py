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

class SerialConsts:
    def __init__(self, baudrate, port):
        self.baudrate = baudrate
        self.port = port

    def get_baudrate(self):
        return self.baudrate
    
    def get_port(self):
        return self.port

serial_consts = SerialConsts(115200, '/dev/cu.usbmodem143101')

def main():
    pass

def setup_app():
    # SETUP CONNECTION WITH ARDUINO SERIAL
    ARDUINO_BAUDRATE = serialconsts.get_baudrate()
    ARDUINO_PORT = serialconsts.get_port()
    ser = serial.Serial(ARDUINO_BAUDRATE, ARDUINO_PORT)

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
