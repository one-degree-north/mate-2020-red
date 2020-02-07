import serial

ser = serial.Serial('/dev/cu.usbmodem141401', 115200)
print("Opened serial on port", ser.name, '\n\n--------------------------------------------------')
while(ser.is_open):
    line = ser.readline()
    print(line)
ser.close()

# print(serial.__file__)