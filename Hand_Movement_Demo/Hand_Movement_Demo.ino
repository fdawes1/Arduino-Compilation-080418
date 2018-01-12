#include <Servo.h>
Servo thumbServo;
Servo indexServo;
Servo middleServo;
Servo ringServo;
Servo pinkieServo;
Servo wristRollservo;

int Thumb;
int Index;
int Middle;
int Ring;
int Pinkie;
int WristRoll;

void setup()
{
  Serial.begin(9600);

  thumbServo.attach(8);
  indexServo.attach(9);
  middleServo.attach(10);
  ringServo.attach(11);
  pinkieServo.attach(12);
 // wristRollservo.attach(13);
}

void loop()
{
  //reset wrist position
  wristRollservo.write(90);
Serial.println("Wrist Reset");
  delay(1000);

  //HAND OPEN
   thumbServo.write(0);
   delay(500);
  indexServo.write(0);
   delay(500);
  middleServo.write(0); 
  delay(500);
  ringServo.write(180);
   delay(500);
  pinkieServo.write(180);
  delay(500);
  Serial.println("hand open");
  delay(1500);

  // HAND CLOSED
  thumbServo.write(180);
  indexServo.write(180);
  middleServo.write(180);
  ringServo.write(0);
  pinkieServo.write(0);
  Serial.println("hand closed");
  delay(1500);
  

   //HAND OPEN
  thumbServo.write(0);
  indexServo.write(0);
  middleServo.write(0);
  ringServo.write(180);
  pinkieServo.write(180);
  Serial.println("hand open");
  delay(1000);


  // HAND CLOSED
  thumbServo.write(180);
   delay(500);
  indexServo.write(180);
  delay(500);
  middleServo.write(180);
  delay(500);
  ringServo.write(0);
  delay(500);
  pinkieServo.write(0);
  Serial.println("hand closed");
  delay(1500);

//  // WAVE
//
//  for (int i = 0; i < 180; i++)
//  {
//    wristRollservo.write(i);
//    Serial.println(i);
//    //delay for stability
//    delay(10);
//  }
//
//  delay(500);
//
//  for (int i = 180; i > 0; i--)
//  {
//    wristRollservo.write(i);
//    Serial.println(i);
//    //delay for stability
//    delay(10);
//  }
//
//  delay(500);
//
//  for (int i = 0; i < 180; i++)
//  {
//    wristRollservo.write(i);
//    Serial.println(i);
//    //delay for stability
//    delay(10);
//  }
//
//  delay(500);
//
//  for (int i = 180; i > 0; i--)
//  {
//    wristRollservo.write(i);
//    Serial.println(i);
//    //delay for stability
//    delay(10);
//  }

  //wait 3 seconds before re-iterating
  delay(3000);

}



