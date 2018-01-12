#include <Servo.h>
int servoPins[] = {9, 10};
//int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38};
//int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
const int numServos = sizeof(servoPins) / sizeof(int);
Servo servos[numServos];

char serialData[numServos * 3];
char tempData[3];

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  for (int i = 0; i < numServos; i++) {
    servos[i].attach(servoPins[i]);
    //servos[i].write(20);
  }
}

void loop() {
  if (Serial.available()) {
    Serial.readBytesUntil('\0', serialData, numServos * 3);

    for (int i = 0; i < numServos; i++) {
      memmove(tempData, serialData + i * 3, 3);
      servos[i].write(atoi(tempData));
    }
  }
}

// <-0  ^90 ->360
