#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial hc12Serial(2, 3); // RX, TX on board
Servo myservo;

String src_msg = "T0I60M0R0P0E";

int msgThumb = 90;
int msgIndex = 90;
int msgMiddle = 90;
int msgRing = 90;
int msgPinky = 90;

int Thumb;
int Index;
int Middle;
int Ring;
int Pinky;
void SerialReceive();
void setup()
{
  hc12Serial.begin(19200);
  hc12Serial.println("Serial Active");
  myservo.attach(8);
}

void loop()
{
  if (hc12Serial.available() > 0 )
  {
    SerialReceive();

    Thumb = map(msgThumb, 10, 110, 180, 0);
    Index = map(msgIndex, 50, 110, 0, 180);
    Middle = map(msgMiddle, 40, 120, 0, 180);
    Ring = map(msgRing, 30, 100, 0, 180);
    Pinky = map(msgPinky, 30, 90, 0, 180);

    Serial.print(src_msg);
    myservo.write(Thumb);
  }
}

void SerialReceive()
{

  char inChar = (char)hc12Serial.read();
  if (inChar != 'E')
  {
    src_msg += inChar;
  }

  if (inChar == 'E')
  { //incoming string =  T # I # M # R # P # X
    src_msg += inChar;
    msgThumb = getSrcValue(src_msg, 'T', 'I');
    msgIndex =   getSrcValue(src_msg, 'I', 'M');
    msgMiddle = getSrcValue(src_msg, 'M', 'R');
    msgRing = getSrcValue(src_msg, 'R', 'P');
    msgPinky = getSrcValue(src_msg , 'P', 'E');
    src_msg = ""; //clear string awaiting next command

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
