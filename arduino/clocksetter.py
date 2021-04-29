import os, sys, serial, time

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

port = "/dev/" + port

print(port)
# stream = os.popen("arduino-cli compile --fqbn arduino:avr:nano RFIDbee2")
# print(stream.read())
# stream = os.popen("arduino-cli upload -p " + port + " --fqbn arduino:avr:nano RFIDbee2")
# print(stream.read())

ser = serial.Serial(port, timeout=3)
s = ser.read(10000)
print(s)

timestring = time.strftime("%y %m %d %H %M %S", time.gmtime())
print(timestring)
ser.write(bytes(timestring, 'utf-8'))

print("DONE")

