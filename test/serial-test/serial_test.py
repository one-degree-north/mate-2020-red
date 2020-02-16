import serial

ser = serial.Serial('/dev/cu.usbmodem141401', 115200)
print("Opened serial on port", ser.name, '\n\n--------------------------------------------------')
line = ""
while(ser.is_open and line != "setup completed"):
    line = ser.readline()
    print(line)
ser.close()

# print(serial.__file__)