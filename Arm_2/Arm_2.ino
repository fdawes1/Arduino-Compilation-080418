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

  int BasePin = A0;    // select the input pin for the potentiometer
  int ShoulderPin = A1;
  int ElbowPin = A2;
  int WristxPin = A3;
  int WristyPin = A4;
  int GripperPin = A5;
   
  int Base = 90;
  int Shoulder = 90;
  int Elbow = 90;
  int Wristx = 90;
  int Wristy = 90;
  int Gripper = 90;

  int msgBase;
  int msgShoulder;
  int msgElbow;
  int msgWristx;
  int msgWristy;
  int msgGripper;
 
  
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

    msgBase = analogRead(BasePin);
    msgShoulder  = analogRead(ShoulderPin);
    msgElbow = analogRead(ElbowPin);
    msgWristx = analogRead(WristxPin);
    msgWristy = analogRead(WristyPin);
    msgGripper = analogRead(GripperPin);
    Serial.println(msgGripper);
      
      Base = map(msgBase, 0, 1023, 0, 180);
      Shoulder = map(msgShoulder, 0, 1023, 0, 180);
      Elbow = map(msgElbow, 0, 1023, 180, 0);
      Wristx = map(msgWristx, 0, 1023, 180, 0);
      Wristy = map(msgWristy, 0, 1023, 180, 0);
      Gripper =  map(msgGripper, 0, 1023, 180 ,0);


    BaseServo.write(Base);
    ShoulderServo.write(Shoulder);
    ElbowServo.write(Elbow);
    WristxServo.write(Wristx);
    WristyServo.write(Wristy);
    GripperServo.write(Gripper);
  }



