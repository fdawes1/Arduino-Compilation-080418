#include <Servo.h>
Servo blindServo;  // create servo object to control a servo

int CLK = 9;  // Pin 9 to clk on encoder
int DT = 8;  // Pin 8 to DT on encoder
int SW = 10; // Pin 10 to Switch on encoder
int RedLed = 5;// You do not need to use the leds.
// you can take a look in the serial monitor if you dont have leds.
// there it will display values.
int GreenLed = 6;
int BlueLed = 7;
int RotPosition = 0;
int rotation;
int value;


boolean LeftRight;

void setup() {
  blindServo.attach(4);  // attaches the servo on pin 2 to the servo object
  Serial.begin (9600);
  pinMode (CLK, INPUT);
  pinMode (DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode (RedLed, OUTPUT);
  pinMode (GreenLed, OUTPUT);
  pinMode (BlueLed, OUTPUT);
  rotation = digitalRead(CLK);
  blindServo.write(96);
}
void loop() {
  value = digitalRead(CLK);
  if (value != rotation) { // we use the DT pin to find out which way we turning.
    if (digitalRead(DT) != value) {  // Clockwise
      RotPosition ++;
      LeftRight = true;
    } else { //Counterclockwise
      LeftRight = false;
      RotPosition--;
    }
    if (LeftRight) { // turning right will turn on red led.
      Serial.println ("clockwise");
      digitalWrite (RedLed, HIGH);
      digitalWrite (GreenLed, LOW);
      blindServo.write(RotPosition + 96);

    } else {       // turning left will turn on green led.
      Serial.println("counterclockwise");
      digitalWrite (RedLed, LOW);
      digitalWrite (GreenLed, HIGH);
      blindServo.write(RotPosition + 96);
    }
    Serial.print("Encoder RotPosition: ");
    Serial.println(RotPosition);
    if(SW
    if (RotPosition == 0) {
      digitalWrite(BlueLed, HIGH);
    }
    else   {
      digitalWrite(BlueLed, LOW);
    }

    // this will print in the serial monitor.

  }
  rotation = value;
}
