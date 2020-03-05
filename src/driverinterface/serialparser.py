import serial

class SerialParser:
    SERIAL_PORT = None
    SERIAL_BAUDRATE = None
    ser = None
    motors_servos_dict = []
    ready_to_read = False

    def __init__(self):
        open_serial()

    def open_serial():
        try:
            read_config()
            ser = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE)
            add_motors_servos()
            complete_setup()
        except:
            print("Could not initialize serial")

    def read_config(self):
        try:
            config = open("config.txt", "r")
            SERIAL_PORT = config.readline().decode('utf-8')
            SERIAL_BAUDRATE = toInt(config.readline().decode('utf-8'))
        except:
            print("Could not read config")

    def add_motors_servos(self):
        line = self.read_serial_line()
        while(line != "starting 7.5s stop test       ||"):
            a_servo = parse_setup_input(line)
            motors_servos_dict.append(a_servo)

    def get_motor_servos(self):
        return motors_servos_dict

    def complete_setup(self):
        line = self.read_serial_line()
        while(line != "setup completed"):
            continue

    def parse_setup_input(line):
        contents = line.split(" ")
        a_servo = contents[0]     # motor/servo should be first in list
        a_pin = contents[4]       # motor/servo should be fifth in list
        return (a_servo, a_pin)
        
    def read_serial_line(self):
        ser.readline().decode('utf-8')
        
    def close_serial(self):
        ser.close()
    
    def get_port(self):
        return SERIAL_PORT()

    def get_baudrate(self):
        return SERIAL_BAUDRATE

    def is_ready_to_read(self):
        return ready_to_read

    def __str__(self):
        return '{self.SERIAL_PORT} | {self.SERIAL_BAUDRATE}'.format(self=self)
