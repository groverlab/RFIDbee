# clocksetter.py v1 by William H. Grover | wgrover@engr.ucr.edu | groverlab.org
# Automates setup of a new RFIDbee2 reader on a Mac
# Requires pyserial from https://pythonhosted.org/pyserial
# This likely only works on a Mac.

import os, sys, serial, time
from serial.tools.list_ports import comports




# ORIGINAL MAC STUFF:
# port = ""
# usb_count = 0
# devices = os.listdir("/dev")
# for device in devices:
#     if "cu.usbserial" in device:
#         port = device
#         usb_count += 1
# if usb_count == 0:
#     sys.exit("No port found")
# if usb_count > 1:
#     sys.exit("Multiple ports found")
# port = "/dev/" + port

port = False
for p in comports():
    if "USB" in str(p):
        port = p.name

if not port:
    sys.exit("No port found with USB in the name")

print(port)

ser = serial.Serial(port, timeout=3)
s = ser.read(10000)
print(s)

timestring = time.strftime("%y %m %d %H %M %S", time.gmtime())
print(timestring)
ser.write(bytes(timestring, 'utf-8'))

print("DONE")

