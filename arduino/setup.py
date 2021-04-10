import os, sys, serial
from datetime import datetime

port = ""
usb_count = 0
devices = os.listdir("/dev")
for device in devices:
    if "cu.usbserial" in device:
        port = device
        usb_count += 1
if usb_count == 0:
    sys.exit("No port found")
if usb_count > 1:
    sys.exit("Multiple ports found")

print(port)
# stream = os.popen("arduino-cli compile --fqbn arduino:avr:nano RFIDbee2")
# print(stream.read())
# stream = os.popen("arduino-cli upload -p /dev/" + port + " --fqbn arduino:avr:nano RFIDbee2")
# print(stream.read())

ser = serial.Serial("/dev/" + port, timeout=3)
s = ser.read(10000)
print(s)

now = datetime.now()
print(now.strftime("%y %m %d %H %M %S"))

print("DONE")
