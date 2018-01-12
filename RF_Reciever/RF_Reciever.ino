const int inPin = A0;
float sensVal = 0;
int led = 13;
int cert =0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sensVal = analogRead(inPin);

  Serial.print ("Voltage Output = ");
  Serial.print (sensVal*5/1023);
  Serial.println("");

  if (sensVal >1000)
  {
    cert ++;
  }
  
  else 
  {
    cert =0;
  }
  
  if (cert >= 4)
  {
    digitalWrite(led,HIGH);
  }
  
  else
  {
    digitalWrite(led, LOW);
  }
  
  delay (50);
}

