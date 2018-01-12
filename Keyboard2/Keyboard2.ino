#include "Keyboard.h"


int buttonPin = 9;  // Set a button to any pin

void setup()
{
  pinMode(buttonPin, INPUT);  // Set the button as an input
  digitalWrite(buttonPin, HIGH);  // Pull the button high
}

void loop()
{
  if (digitalRead(buttonPin) == 0)  // if the button goes low
  {
    Keyboard.write(' 9870\n');  // send a 'z' to the computer via Keyboard HID
    delay(500);  // delay so there aren't a kajillion z's
  }
}
