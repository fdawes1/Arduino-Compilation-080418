//#include <SoftwareSerial.h>

//SoftwareSerial bluetooth(9, 8);

//Standard PWM DC control
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control
int encoderValue = 0;

void count(void); // code for counting the increasing values of encoder ticks void setup()

void stop(void)                    //Stop
{
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
}
void advance(char a, char b)         //Move forward
{
  analogWrite (E1, a);     //PWM Speed Control
  digitalWrite(M1, HIGH);
  analogWrite (E2, b);
  digitalWrite(M2, HIGH);
}
void back_off (char a, char b)         //Move backward
{
  analogWrite (E1, a);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);
}
void turn_L (char a, char b)            //Turn Left
{
  analogWrite (E1, a);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, HIGH);
}
void turn_R (char a, char b)            //Turn Right
{
  analogWrite (E1, a);
  digitalWrite(M1, HIGH);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);
}

void setup(void)
{
  int i;
  for (i = 4; i <= 7; i++)
    pinMode(i, OUTPUT);
  Serial.begin(9600);      //Set Baud Rate
  //bluetooth.begin(9600);
  // bluetooth.println("Bluetooth Control Activated");


  pinMode(7, INPUT);

  attachInterrupt(2, count, FALLING);

  encoderValue = 0;


  Serial.println("Keyboard control Activated");
}

void loop(void)
{

  if (Serial.available() > 0) {
    char val = Serial.read();
    if (val != -1)
    {
      switch (val)
      {
        case 'w'://Move Forward
          advance (255, 255);  //move forward in max speed
          break;
        case 's'://Move Backward
          back_off (255, 255);  //move back in max speed
          break;
        case 'd'://Turn Left
          turn_L (100, 100);
          break;
        case 'a'://Turn Right
          turn_R (100, 100);
          break;
        case 'z':
          Serial.println("Serial Ready");
          //          bluetooth.println("Bluetooth Ready");
          break;
        case 'x':
          stop();
          break;
      }
    }
    else stop();
  }
}



void count()

{

  encoderValue++;
  Serial.println(encoderValue);

}

