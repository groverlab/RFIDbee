# clocksetter.py v2 by William H. Grover | wgrover@engr.ucr.edu | groverlab.org
# Automates setup of a new RFIDbee2 reader on a MacOS or Windows computer
# Requires pyserial from https://pythonhosted.org/pyserial;
#   install it first using "pip install pyserial"


import sys, serial, time, platform
from serial.tools.list_ports import comports

port = False
for p in comports():
    if "USB" in str(p):
        port = p.name

if not port:
    sys.exit("No port found with USB in the name")

if platform.system() == "Darwin":  # if MacOS...
    port = "/dev/" + port   # ...prepend /dev/

print(port)

ser = serial.Serial(port, timeout=3)
s = ser.read(10000)
print(s)

timestring = time.strftime("%y %m %d %H %M %S", time.gmtime())
print(timestring)
ser.write(bytes(timestring, 'utf-8'))

print("DONE")

