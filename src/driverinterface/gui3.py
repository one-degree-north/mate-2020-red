import serial, sys
from time import sleep
import serialparser
import arduinoservo
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
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
SCREEN_SIZE = (SCREEN_WIDTH, SCREEN_HEIGHT)
DELAY_TIME = 0.15

# MOTORS AND SERVO
motors_servos = []
controllable_motors_servos = []

# PYGAME VARIABLES
screen = pygame.display.set_mode(SCREEN_SIZE)

def main():
    setup_app()
    run_app()

def setup_app():
    ser = SerialParser()
    while not ser.is_ready_to_read():
        continue
    create_motors_servos()
    pygame.init()

def create_motors_servos():
    try:
        raw = ser.get_motors_servos()
        motors_servos = ArduinoServoFactory.create_motors_servos(raw)
        return True
    except:
        return False

def run_app():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                quit_app()
            elif event.type == KEYDOWN:
                key_event(event.key)

        line = read_serial_line()
        print(line)
    sleep(DELAY_TIME)

def key_event(key):
    if key == K_ESCAPE:
        quit_app()
    if key == K_w:
        pass
    if key == K_a:
        pass
    if key == K_s:
        pass
    if key == K_d:
        pass
    if key == K_i:
        pass
    if key == K_k:
        pass
    if key == K_j:
        pass
    if key == K_l:
        pass

def quit_app():
    print("Quitting app...")
    ser.close()
    sys.exit()

if __name__ == "__main__":
    main()
