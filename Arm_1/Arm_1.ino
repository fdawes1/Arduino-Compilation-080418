 // #include <SoftwareSerial.h>
  
 // SoftwareSerial  BluetoothSerial(2, 3);      //                                          (RX, TX) on board
  // sonar(9,8);      //                                          (RX, TX) on board
 
  
  #include <Servo.h>
  Servo BaseServo;
  Servo ShoulderServo;
  Servo ElbowServo;
  Servo WristxServo;
  Servo WristyServo;
  Servo GripperServo; 
  
  String src_msg = "B0S0E0X0Y0G0U";
   
  int msgBase = 90;
  int msgShoulder = 90;
  int msgElbow = 90;
  int msgWristx = 90;
  int msgWristy = 90;
  int msgGripper = 90;
  
  int Base;
  int Shoulder;
  int Elbow;
  int Wristx;
  int Wristy;
  int Gripper;
  
  void setup()
  {
    Serial.begin(9600);
    BaseServo.attach(13);
    ShoulderServo.attach(12);
    ElbowServo.attach(11);
    WristxServo.attach(7);
    WristyServo.attach(6);
    GripperServo.attach(5);
  }
  
  void loop()
  {
    if (Serial.available()  >  0 )
    {
      SerialReceive();
  
     Base = map(msgBase, 0, 180, 0, 180);
      Shoulder = map(msgShoulder, 0, 180, 0, 180);
      Elbow = map(msgElbow, 0, 180, 0, 180);
      Wristx = map(msgWristx, 0, 180, 0, 180);
      Wristy = map(msgWristy, 0, 180, 0, 180);
      Gripper =  msgGripper;


    BaseServo.write(Base);
    ShoulderServo.write(Shoulder);
    ElbowServo.write(Elbow);
    WristxServo.write(Wristx);
    WristyServo.write(Wristy);
    GripperServo.write(Gripper);
  }
}


void SerialReceive()
{
  char inChar = (char)Serial.read();
  if (inChar != 'U')
  {
    src_msg += inChar;
  }
  if (inChar == 'U')
  { //incoming string =  B # S # E # X # Y # G # U
    src_msg += inChar;
    msgBase        = getSrcValue(src_msg, 'B', 'S');
    msgShoulder    =   getSrcValue(src_msg, 'S', 'E');
    msgElbow       = getSrcValue(src_msg, 'E', 'X');
    msgWristx      = getSrcValue(src_msg, 'X', 'Y');
    msgWristy      = getSrcValue(src_msg , 'Y', 'G');
    msgGripper     = getSrcValue(src_msg , 'G', 'U');
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

