// fake_bee for stress testing RFIDbee readers
// by William H. Grover  |  wgrover@engr.ucr.edu

#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(9);  
}

// 10% duty cycle for tag reading:
void loop() {
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  delay(4000);
  myservo.write(180);
  delay(1000);
  myservo.write(90);
  delay(4000);
}
