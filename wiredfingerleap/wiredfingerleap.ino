//#include <SoftwareSerial.h>

//SoftwareSerial  HC12Serial(2, 3);      //                                          (RX, TX) on board

#include <Servo.h>
Servo thumbServo;
Servo indexServo;
Servo middleServo;
Servo ringServo;
Servo pinkieServo;
Servo wristRollservo; 

String src_msg = "T0I60M0R0P0E";

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

void setup()
{
//  HC12Serial.begin(19200);
Serial.begin(19200);
  thumbServo.attach(8);
  indexServo.attach(9);
  middleServo.attach(10);
  ringServo.attach(11);
  pinkieServo.attach(12);
 // wristRollservo.attach(13);
}

void loop()
{
  if (Serial.available()  >  0 )
  {
    SerialReceive();

    Thumb = map(msgThumb, 60, 100, 180, 0);
    Index = map(msgIndex, 40, 100, 180, 0);
    Middle = map(msgMiddle, 40, 105, 180, 0);
    Ring = map(msgRing, 40, 100, 0, 180);
    Pinkie = map(msgPinkie, 35, 90, 0, 180);
   // WristRoll =  msgWristRoll;

    thumbServo.write(Thumb);
    indexServo.write(Index);
    middleServo.write(Middle);
    ringServo.write(Ring);
    pinkieServo.write(Pinkie);

   // wristRollservo.write(WristRoll);
  }
}


void SerialReceive()
{
  char inChar = (char)Serial.read();
  if (inChar != 'E')
  {
    src_msg += inChar;
  }
  if (inChar == 'E')
  { //incoming string =  T # I # M # R # P # W # E
    src_msg += inChar;
    msgThumb = getSrcValue(src_msg, 'T', 'I');
    msgIndex   =   getSrcValue(src_msg, 'I', 'M');
    msgMiddle = getSrcValue(src_msg, 'M', 'R');
    msgRing    = getSrcValue(src_msg, 'R', 'P');
    msgPinkie   = getSrcValue(src_msg , 'P', 'E');
    //    msgWristRoll = getSrcValue(src_msg , 'W', 'E');
    src_msg = "";              //clear string awaiting next command
  }
}


int getSrcValue(String src_msg, char c1, char c2)
{
  int c1_pose = src_msg.indexOf(c1);
  int c2_pose = src_msg.indexOf(c2);

  if (c1_pose > c2_pose)
  {
    int temp = c1_pose;
    c1_pose = c2_pose;
    c2_pose = temp;
  }
  // This function also deals with the error handling
  if (c1_pose == -1 || c2_pose == -1)
  {
    return -1;
  }

  String res = src_msg.substring(c1_pose + 1, c2_pose);
  return atoi(res.c_str());
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
