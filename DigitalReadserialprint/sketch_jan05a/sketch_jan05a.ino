int Loadpin = 9;
int LEDpin = 13;
boolean toggle  = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(Loadpin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(A0);
  Serial.println(val);
  delay(1);

  if ( val > 450 )
  {
    delay(500);
    digitalWrite(Loadpin, toggle);
    digitalWrite(LEDpin, toggle);
toggle = !toggle;
  }
}


