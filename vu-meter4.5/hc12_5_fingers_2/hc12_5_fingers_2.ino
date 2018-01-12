#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial hc12Serial(2, 3); // RX, TX on board
Servo thumbservo;
Servo indexservo;
Servo middleservo;
Servo ringservo;
Servo pinkieservo;
Servo wristRollservo;

String src_msg = "T0I60M0R0P0E";   //W0E";

int msgThumb = 90;
int msgIndex = 90;
int msgMiddle = 90;
int msgRing = 90;
int msgPinkie = 90;
int msgWristRoll = 90;

int Thumb;
int Index;
int Middle;
int Ring;
int Pinkie;
int WristRoll;

void SerialReceive();

void setup()
{
  Serial.begin(19200);
  hc12Serial.begin(19200);
 // hc12Serial.println("Serial Active");
  
  thumbservo.attach(8);
  indexservo.attach(9);
  middleservo.attach(10);
  ringservo.attach(11);
  pinkieservo.attach(12);
  wristRollservo.attach(13);
}

void loop()
{
  if (hc12Serial.available() > 0)
  {
    SerialReceive();    
    Thumb = map(msgThumb, 50, 100, 180, 0);
    Index = map(msgIndex, 65, 100, 180, 0);
    Middle = map(msgMiddle, 40, 105, 180, 0);
    Ring = map(msgRing, 40, 105, 0, 180);
    Pinkie = map(msgPinkie, 35, 90, 0, 180);
    WristRoll =  map(msgWristRoll, 0, 180, 0, 180);
  
    thumbservo.write(Thumb);
    indexservo.write(Index);
    middleservo.write(Middle);
    ringservo.write(Ring);
    pinkieservo.write(Pinkie);
  //  wristRollservo.write(WristRoll);
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
    msgPinkie = getSrcValue(src_msg , 'P', 'E');
    msgWristRoll = getSrcValue(src_msg , 'W', 'E');
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
