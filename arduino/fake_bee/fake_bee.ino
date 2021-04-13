// fake_bee for stress testing RFIDbee readers
// by William H. Grover  |  wgrover@engr.ucr.edu

#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(9);
}

int pos = 0;

// 10% duty cycle for tag reading:
void loop() {
  for (pos = 90; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }

  for (pos = 180; pos >= 90; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  delay(5000);

  for (pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }

  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
  }

    delay(5000);

}
