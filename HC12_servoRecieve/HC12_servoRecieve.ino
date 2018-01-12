#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial hc12Serial(2, 3); // RX, TX
Servo myservo;

String src_msg = "T0I0M0R0P0X";

int msgThumb = 0;
int msgIndex = 0;
int msgMiddle = 0;
int msgRing = 0;
int msgPinky = 0;

int Thumb;
int Index;
int Middle;
int Ring;
int Pinky;

void setup()
{
  Serial.begin(9600);
  hc12Serial.begin(9600);

  myservo.attach(8);
}

void loop()
{


  if (hc12Serial.available())
  {
    Serial.println(src_msg);

    Thumb = map(msgThumb, 40, 120, 0, 180);
    Index = map(msgIndex, 0, 25, 0, 180);
    Middle = map(msgMiddle, 20, 120, 0, 180);
    Ring = map(msgRing, 0, 120, 0, 180);
    Pinky = map(msgPinky, 0, 75, 0, 180);

    myservo.write(Index);
  }
  delay(5); //for stability
}



//--------- Serial Event Handling ---------//


void serialEvent()
{
  if (hc12Serial.available() > 0)
  {
    char inChar = (char)hc12Serial.read();
    if (inChar != 'X')
    {
      src_msg += inChar;
    }


    if (inChar == 'X')
    { //incoming string =  T # I # M # R # P # X

      src_msg += inChar;

      msgThumb = getSrcValue(src_msg, 'T', 'I');

      msgIndex =   getSrcValue(src_msg, 'I', 'M');

      msgMiddle = getSrcValue(src_msg, 'M', 'R');

      msgRing = getSrcValue(src_msg, 'R', 'P');

      msgPinky = getSrcValue(src_msg , 'P', 'X');

      src_msg = ""; //clear string awaiting next command
    }

  }
}



int getSrcValue(String src_msg, char c1, char c2)
{
  int c1_pose = src_msg.indexOf(c1);
  int c2_pose = src_msg.indexOf(c2);

  //ERROR HANDLING---------------------
  if (c1_pose > c2_pose)
  {
    int temp = c1_pose;
    c1_pose = c2_pose;
    c2_pose = temp;
  }
  if (c1_pose == -1 || c2_pose == -1)
  {
    return -1;
  }
  //-----------------------------------

  String res = src_msg.substring(c1_pose + 1, c2_pose);
  return atoi(res.c_str());

}
