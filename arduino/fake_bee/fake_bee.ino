// fake_bee for stress testing RFIDbee battery packs
// by William H. Grover  |  wgrover@engr.ucr.edu

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  delay(9000);
}
