/***************************************************
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 16 servos, one after the other

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  resetStance();
  delay(500);
  StepBack();
  delay(100);


}

















void resetStance()
{
  int  pulselength0 = map(10, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength1 = map(150, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength2 = map(10, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength3 = map(150, 0, 180, SERVOMIN, SERVOMAX);

  int  pulselength4 = map(60, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength5 = map(120, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength6 = map(180, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength7 = map(50, 0, 180, SERVOMIN, SERVOMAX);

  int  pulselength8 = map(90, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength9 = map(60, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength10 = map(70, 0, 180, SERVOMIN, SERVOMAX);
  int  pulselength11 = map(90, 0, 180, SERVOMIN, SERVOMAX);


  pwm.setPWM(0, 0, pulselength0);
  pwm.setPWM(1, 0, pulselength1);
  pwm.setPWM(2, 0, pulselength2);
  pwm.setPWM(3, 0, pulselength3);


  pwm.setPWM(4, 0, pulselength4);
  pwm.setPWM(5, 0, pulselength5);
  pwm.setPWM(6, 0, pulselength6);
  pwm.setPWM(7, 0, pulselength7);


  pwm.setPWM(8, 0, pulselength8);
  pwm.setPWM(9, 0, pulselength9);
  pwm.setPWM(10, 0, pulselength10);
  pwm.setPWM(11, 0, pulselength11);

}


void StepBack()
{

  pwm.setPWM(1, 0, map(120, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(6, 0, map(50, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(10, 0, map(10, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);

  pwm.setPWM(2, 0, map(180, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(10, 0, map(70, 0, 180, SERVOMIN, SERVOMAX));

  delay(100);
  pwm.setPWM(6, 0, map(180, 0, 180, SERVOMIN, SERVOMAX));

 


  pwm.setPWM(0, 0, map(60, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(7, 0, map(100, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(11, 0, map(60, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);


  pwm.setPWM(1, 0, map(150, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(9, 0, map(10, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);

  pwm.setPWM(2, 0, map(10, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);

  pwm.setPWM(3, 0, map(10, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);

  pwm.setPWM(11, 0, map(60, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);


  pwm.setPWM(0, 0, map(10, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);
  pwm.setPWM(8, 0, map(180, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);

  pwm.setPWM(7, 0, map(10, 0, 180, SERVOMIN, SERVOMAX));
  delay(100);


}

