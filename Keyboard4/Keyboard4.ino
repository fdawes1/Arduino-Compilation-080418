#include "Keyboard.h"

const int buttonPin = 2;
int buttonState = 0;
int prevButtonState = HIGH;

void setup() {


  pinMode(6, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH);
  Keyboard.begin();
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if ((buttonState != prevButtonState) && (buttonState == HIGH)) {
    Keyboard.press(KEY_LEFT_CTRL); 
    Keyboard.press('n');
    delay(100);
    Keyboard.releaseAll(); // This is important after every Keyboard.press it will continue to be pressed
  }
  prevButtonState = buttonState;


  digitalWrite(6, 0);          // Turns ON Relays 1
  Serial.println("Light ON");
  delay(2000);                                      // Wait 2 seconds

  digitalWrite(6, 1);         // Turns Relay Off
  Serial.println("Light OFF");
  delay(2000);

}
