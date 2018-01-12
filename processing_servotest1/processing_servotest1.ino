#include <Servo.h>

Servo myservo;              // create servo object to control a servo

int data = 0;               // variable to read from the serial port
int newdata = 0;

void setup()
{
  myservo.attach(8);        // attaches the servo on pin 8 to the servo object
  Serial.begin(9600);       // starts the serial Serial at 9600 bauds
}

void loop()
{
  if (Serial.available())   // if data is available to read
  {
    newdata = Serial.read();   // read it and store it in 'data'
  }

  if (data != newdata)
  {
    data = newdata;
           myservo.write(data);      // sets the servo position according to value in val
    
      Serial.println(data);
      delay(15);                // waits for the servo to get there
    }
}

    //
    //void SerialReceive()
    //{
    //
    //  char inChar = (char)hc12Serial.read();
    //  if (inChar != 'E')
    //  {
    //    src_msg += inChar;
    //  }
    //
    //  if (inChar == 'E')
    //  { //incoming string =  T # I # M # R # P # X
    //    src_msg += inChar;
    //    msgThumb = getSrcValue(src_msg, 'T', 'I');
    //    msgIndex =   getSrcValue(src_msg, 'I', 'M');
    //    msgMiddle = getSrcValue(src_msg, 'M', 'R');
    //    msgRing = getSrcValue(src_msg, 'R', 'P');
    //    msgPinkie = getSrcValue(src_msg , 'P', 'E');
    //    msgWristRoll = getSrcValue(src_msg , 'W', 'E');
    //    src_msg = ""; //clear string awaiting next command
    //
    //  }
    //}
    //
    //int getSrcValue(String src_msg, char c1, char c2)
    //{
    //  int c1_pose = src_msg.indexOf(c1);
    //  int c2_pose = src_msg.indexOf(c2);
    //
    //  //ERROR HANDLING---------------------
    //  if (c1_pose > c2_pose)
    //  {
    //    int temp = c1_pose;
    //    c1_pose = c2_pose;
    //    c2_pose = temp;
    //  }
    //
    //  if (c1_pose == -1 || c2_pose == -1)
    //  {
    //    return -1;
    //  }
    //  //-----------------------------------
    //
    //  String res = src_msg.substring(c1_pose + 1, c2_pose);
    //  return atoi(res.c_str());
    //
    //}
