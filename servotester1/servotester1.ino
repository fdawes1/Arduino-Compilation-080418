#include <Servo.h>

Servo servo1; Servo servo2; Servo servo3; Servo servo4; Servo servo5;


void setup() {
  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);
  servo5.attach(12);

  Serial.begin(19200);
  Serial.println("Ready");

}

void loop() {

  static int v = 0;

  if ( Serial.available()) {
    char ch = Serial.read();

    switch (ch) {
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
      case 't':
        servo1.write(v);
        v = 0;
        break;
      case 'i':
        servo2.write(v);
        v = 0;
        break;
      case 'm':
        servo3.write(v);
        v = 0;
        break;
      case 'r':
        servo4.write(v);
        v = 0;
        break;
      case 'p':
        servo5.write(v);
        v = 0;
        break;
    }
  }

  // Servo::refresh();

}
