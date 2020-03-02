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
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
SCREEN_SIZE = (SCREEN_WIDTH, SCREEN_HEIGHT)

# SETUP
# ser = serial.Serial(ARDUINO_BAUDRATE, ARDUINO_PORT)
screen = pygame.display.set_mode(SCREEN_SIZE)

def main():
    setup_app()
    run_app()
    pass

def setup_app():
    
    # SETUP PYGAME
    pygame.init()
    pass

def run_app():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                quit_app()
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    quit_app()

def quit_app():
    print("Quitting app...")
    ser.close()
    sys.exit()

if __name__ == "__main__":
    main()
