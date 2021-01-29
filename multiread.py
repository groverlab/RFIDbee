import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522

reader = SimpleMFRC522()

while True:
	id, text = reader.read()
	print(id)
	print(text)

