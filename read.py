from pirc522 import RFID
import signal
import sys

rdr = RFID()

def end_read(signal,frame):
    global run
    print("\nCtrl+C captured, ending read.")
    run = False
    rdr.cleanup()
    sys.exit()

signal.signal(signal.SIGINT, end_read)

rdr.set_antenna_gain(7)

while True:
  rdr.wait_for_tag()
  (error, tag_type) = rdr.request()
  if not error:
    print("Tag detected")
    (error, uid) = rdr.anticoll()
    if not error:
      print("  UID: " + str(uid))

# Calls GPIO cleanup
rdr.cleanup()


