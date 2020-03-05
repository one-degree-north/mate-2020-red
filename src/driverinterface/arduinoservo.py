import pygame

class ArduinoServoFactory:
    @classmethod 
    def create_motors_servos(self, rawarr):
        motors_servos = []
        for pair in rawarr:
            servo_type, pin = pair
            servo = None
            if servo_type == "motor":
                servo = make_motor(pin)
            elif servo_type == "servo":
                servo = make_servo(pin)
            if not servo == None:
                motors_servos.append(servo)
        return motors_servos

    def make_motor(self, pin):
        motor = Motor("Motor", pin)
        return motor

    def make_servo(self, pin):
        servo = Servo("Servo", pin)
        return servo

class ArduinoServo:
    # Constants
    OUTPUT_EQUILIBRIUM = 1500
    DISPLAY_WIDTH = 30
    MAX_HEIGHT = 50

    def __init__(self, name, pin):
        self.name = name
        self.pin = pin

        self.output = ArduinoServo.OUTPUT_EQUILIBRIUM
        self.controllable = False
        self.display_rect = None
        self.equilibrium_xpos = 0
        self.equilibrium_ypos = 0

    def get_name(self):
        return self.name

    def set_name(self, inp):
        self.name = inp

    def get_pin(self):
        return self.pin

    def get_output(self):
        return self.output

    def set_output(self, inp):
        self.output = inp
    
    @classmethod
    def get_output_equilibrium(self):
        return ArduinoServo.OUTPUT_EQUILIBRIUM

    def init_display(self, xpos, ypos):
        self.equilibrium_xpos = xpos
        self.equilibrium_ypos = ypos
        self.display_rect = pygame.Rect(xpos, ypos, ArduinoServo.DISPLAY_WIDTH, 0)

    def update_display(self):
        height = map_output_to_height()

    def get_display(self):
        return display_rect

    def map_output_to_height():
        height = output - ArduinoServo.OUTPUT_EQUILIBRIUM
        
        pass

    def is_controllable(self):
        return self.controllable

    def set_controllable(self, status):
        self.controllable = status

    def __str__(self):
        return '{self.name},{self.pin}'.format(self=self)

class Motor(ArduinoServo):
    # Constants
    MAX_OUTPUT_DEVIATION = 300

    def __init__(self, name, pin):
        super(Motor, self).__init__(name, pin)
        self.name = name
        self.pin = pin

    def map_output_to_height():
        height = self.get_output() - self.get_output_equilibrium()
        height /= (Motor.MAX_OUTPUT_DEVATION / ArduinoServo.MAX_HEIGHT)
        return height

class Servo(ArduinoServo):
    # Constants
    MAX_OUTPUT_DEVIATION = 600

    def __init__(self, name, pin):
        super(Servo, self).__init__(name, pin)
        self.name = name
        self.pin = pin

    def map_output_to_height():
        height = self.get_output() - self.get_output_equilibrium()
        height /= (Servo.MAX_OUTPUT_DEVIATION / ArduinoServo.MAX_HEIGHT)
        return height
