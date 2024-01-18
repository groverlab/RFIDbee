# clocksetter.py v2 by William H. Grover | wgrover@engr.ucr.edu | groverlab.org
# Automates setup of a new RFIDbee2 reader on a MacOS or Windows computer
# Requires pyserial from https://pythonhosted.org/pyserial;
#   install it first using "pip install pyserial"


import sys, serial, time, platform
from serial.tools.list_ports import comports

ports = 0
port = ""
for p in comports():
    if "USB" in str(p):
        port = p.name
        ports += 1

if ports == 0:
    sys.exit("No port found - is the RFIDbee2 plugged in?")

if ports >= 2:
    sys.exit("Too many ports found - unplug everything but the RFIDbee2 and try again")

if platform.system() == "Darwin":  # if MacOS...
    port = "/dev/" + port   # ...prepend /dev/

print("Found an RFIDbee2 at " + port)

ser = serial.Serial(port, timeout=3)
s = ser.read(10000)
print(s.decode("utf-8"))

print("Setting the clock using this time:   ", end="")
timestring = time.strftime("%y %m %d %H %M %S", time.gmtime())
print(timestring)
ser.write(bytes(timestring, 'utf-8'))

print("DONE")

