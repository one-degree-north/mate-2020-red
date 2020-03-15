import serial, sys, re
from time import sleep
import serialparser, arduinoservo
import pygame
from pygame.locals import (
    K_w,
    K_a,
    K_s,
    K_d,
    K_j,
    K_k,
    K_0,
    K_1,
    K_2,
    K_3,
    K_ESCAPE,
    KEYDOWN,
    KEYUP,
    QUIT,
)

# CONSTANTS
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
SCREEN_SIZE = (SCREEN_WIDTH, SCREEN_HEIGHT)
DELAY_TIME = 0.15

# OBJECTS
ser = None

# MOTORS AND SERVO
motors_servos = []
controllable_motors_servos = []

# PYGAME VARIABLES
screen = pygame.display.set_mode(SCREEN_SIZE)

def main():
    setup_app()
    run_app()

def setup_app():
    """
    Open the serial. Through the serial, setup the motors/servos. Then,
    get the motors/servos that are controllable.
    """
    setup_serial()
    create_motors_servos()
    set_controllable_motors_servos()
    pygame.init()

def run_app():
    while True:
        handle_pygame_events()

        line = read_serial_line()
        print(line)
        parsed_output_value = parse_output_value(line)

    sleep(DELAY_TIME)

def handle_pygame_events():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            quit_app()
        elif event.type == KEYDOWN:
            key_event_down(event.key)
        elif event.type == KEYUP:
            key_event_up(event.key)

def setup_serial():
    ser = SerialParser()
    success = ser.open_serial()
    if not success:
        quit_app("Serial failed to open")
    while not ser.is_ready_to_read():
        continue

def create_motors_servos():
    """
    Converts the setup output into ArduinoServo objects.
    TESTED, PASSES
    """
    try:
        raw = ser.get_motors_servos()
        motors_servos = ArduinoServoFactory.create_motors_servos(raw)
    except:
        quit_app("Failed to create motors/servos")

def set_controllable_motors_servos():
    """
    Gets the motors/servos which are controllable with the
    driver interface from the GUI_CONFIG_FILE. Adds them to
    the controllable_motors_servos list.
    TESTED, PASSES
    """
    GUI_CONFIG_FILE = "guiconfig.txt"
    try:
        config = open(GUI_CONFIG_FILE, 'r')
        controllable_arr = []
        for line in config:
            # Read the line and add it to the list.
            # When reading the line, it will also add the newline character.
            # This causes equality checks to be false.
            # Handles/strips the newline character with rstrip.
            controllable_arr.append(line.rstrip('\n'))
        for motor_servo in motors_servos:
            for controllable_motor in controllable_arr:
                if motor_servo.get_name() == controllable_motor:
                    controllable_motors_servos.append(motor_servo)
                    controllable_arr.remove(controllable_motor)
                    motor_servo.set_controllable(True)
    except:
        quit_app("Failed to set controllable motors/servos")

def key_event_down(key):
    if key == K_ESCAPE:
        quit_app()
    elif key == K_w:
        pass
    elif key == K_s:
        pass
    elif key == K_a:
        pass
    elif key == K_d:
        pass
    elif key == K_k:
        pass
    elif key == K_j:
        pass

def key_event_up(key):
    if key == K_w:
        pass
    elif key == K_a:
        pass
    elif key == K_s:
        pass
    elif key == K_d:
        pass
    elif key == K_k:
        pass
    elif key == K_j:
        pass

def parse_output_value(raw):
    """
    Parses the output of the serial into a list of the output values.
    TESTED, PASSES
    """
    raw_output_values = re.findall("[\d]+>", raw)
    output_values = []
    for value in raw_output_values:
        int_value = int(value[:-1])
        output_values.append(int_value)
    return output_values

def quit_app():
    print("Quitting app...")
    ser.close()
    sys.exit()

def quit_app(err):
    """
    Prints the error encountered while quitting.
    """
    print("Quitting app...")
    print("ERROR:", err)
    ser.close()
    sys.exit()

if __name__ == "__main__":
    main()
