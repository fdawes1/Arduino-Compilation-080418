

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(6, INPUT);
  pinMode(12, OUTPUT);
    pinMode(11, OUTPUT);
      pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  if (6 == HIGH)
  {
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  }
}
