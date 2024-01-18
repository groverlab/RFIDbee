# bootstrap.py v1 by William H. Grover | wgrover@engr.ucr.edu | groverlab.org
# Automates setup of a new RFIDbee2 reader on a Mac
# Requires pyserial from https://pythonhosted.org/pyserial
# Requires arduino-cli from https://arduino.github.io/arduino-cli
# This likely only works on a Mac.

import os, sys, serial, time

port = ""
usb_count = 0
devices = os.listdir("/dev")
for device in devices:
    if "cu.usbserial" in device:
        port = device
        usb_count += 1
if usb_count == 0:
    sys.exit("No connected Arduino found")
if usb_count > 1:
    sys.exit("Multiple ports found; can't tell which one to use")

port = "/dev/" + port

# print(port)
stream = os.popen("arduino-cli compile --fqbn arduino:avr:nano RFIDbee2")
stream.read()
stream = os.popen("arduino-cli upload -p " + port + " --fqbn arduino:avr:nano RFIDbee2")
stream.read()
# print("DONE UPLOADING")

ser = serial.Serial(port, timeout=3)
s = ser.read(10000)

if b"Real-time clock needs to be set" in s:  # need to set clock
    timestring = time.strftime("%y %m %d %H %M %S", time.gmtime())
    # print(timestring)
    ser.write(bytes(timestring, 'utf-8'))

elif b"Startup\tStartup" in s:  # successful startup
    pass

else:
    sys.exit("No successful Arduino startup detected")

print("DONE")

