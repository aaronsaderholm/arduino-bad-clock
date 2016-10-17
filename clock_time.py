import time
import serial
import struct

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(port='/dev/cu.usbserial-A900JEHX',baudrate=115200)
time.sleep(2)
time = int(time.time())
print(time,"\n")
ser.write(bytes(time))
time.sleep(1)
while 1:
    try:
        print(ser.readline())
        time.sleep(1)
    except ser.SerialTimeoutException:
        print('Data could not be read')
        time.sleep(1)