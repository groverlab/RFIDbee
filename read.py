from pirc522 import RFID
import signal
import sys
import RPi.GPIO as GPIO
import datetime

rdr = RFID()

logfile = open("logfile.txt", "a")

def end_read(signal,frame):
    global run
    print("\nCtrl+C captured, ending read.")
    run = False
    rdr.cleanup()
    logfile.close()
    sys.exit()

signal.signal(signal.SIGINT, end_read)

rdr.set_antenna_gain(7)

GPIO.setup(36, GPIO.OUT)
led=False

while True:
  rdr.wait_for_tag()
  (error, tag_type) = rdr.request()
  if not error:
    print("Tag detected")
    (error, uid) = rdr.anticoll()
    if not error:
      print("  UID: " + str(uid))
      logfile.write(str(datetime.datetime.now()) + "\t" + str(uid) + "\n")
      logfile.flush()
      GPIO.output(36, led)
      led = not led


