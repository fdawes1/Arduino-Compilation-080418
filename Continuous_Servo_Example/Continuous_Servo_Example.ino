
// Rotating a continuous servo 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
void setup() 
{ 
  myservo.attach(4);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
  
  myservo.write(45);                  // rotate counterclockwise full speed
  delay(2000);                           
  myservo.write(90);                  // stop
  delay(100);
  myservo.write(135);                 // rotate clockwise full speed
  delay(2000);
  myservo.write(90);                  // X
  delay(100);
  myservo.write(80);                  // rotate very slowly counterclockwise
  delay(2000);
  myservo.write(90);                  // stop
  delay(100);
  myservo.write(100);                 // rotate very slowly clockwise
  delay(2000);
} 
