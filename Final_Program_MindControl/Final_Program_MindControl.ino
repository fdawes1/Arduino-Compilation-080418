//PRESENTLY, INFORM USER TO HOLD EMERGENCY STOP BUTTON DOWN WHEN SWITCHING OFF THE DEVICE.

//    Program       : Mindwave - Arduino - Robotic Arm                //
//    Interfacing   : HC-05 Bluetooth Module                          //
//    Output        : Eye Blink and Attention control of Robotic Arm  //
//    Written by    : Matthew Haire                                   //

#define   BAUDRATE           57600
//the term 'BAUDRATE' can now be used to represent of 57600 (our desired baudrate) within the program.
#define   Theshold_Eyeblink  98 // dropped from 110 to 98
#define   EEG_AVG            70

// Forward motion acronym = FWD
// Backward motion acronym = BCK

#define TORSO_LEFT      10
// the term 'TORSO_LEFT' can now be used to represent '10'; the GPIO pin that will cause the TORSO motor of the robot arm to turn LEFT when driven high.
//this makes it easier for the programmer to keep track of the purpose of the pin without having to reference the definitions at the start of the program.
#define TORSO_RIGHT     11
#define SHOULDER_FWD    6
#define SHOULDER_BCK    7
#define ELBOW_FWD       8
#define ELBOW_BCK       9
#define WRIST_FWD       4
#define WRIST_BCK       5
#define GRIP_CLOSE      2
#define GRIP_OPEN       3

#define EMERGENCY_STOP  12

#define EYE_ENABLE_LED  13

#define TORSO_ENABLE    A0
#define SHOULDER_ENABLE A1
#define ELBOW_ENABLE    A2
#define WRIST_ENABLE    A3
#define GRIP_ENABLE     A4
#define SPOTLIGHT       A5

long payloadDataS[5] = {0};
long payloadDataA[64] = {0};
//This create an vector called playloadData consisting of 64 long integers and assigns each element of the array a value of '0'.
byte checksum = 0, generatedchecksum = 0, attention = 0, meditation = 0;
unsigned int Raw_data = 0, Poorquality = 0, Plength = 0, Eye_Enable = 0, On_Flag = 0, Off_Flag = 1 ;
unsigned int j = 0, n = 0, ls = 0, Initial_time = 0, Current_time = 0, Desired_time = 0;
long Temp = 0, Avg_Raw = 0, Temp_Avg = 0;

void setup() //declare the 'setup()' function as void; it will return no value when executed.
{
  pinMode(TORSO_LEFT, OUTPUT);
  pinMode(TORSO_RIGHT, OUTPUT);
  pinMode(SHOULDER_FWD, OUTPUT);
  pinMode(SHOULDER_BCK, OUTPUT);
  pinMode(ELBOW_FWD, OUTPUT);
  pinMode(ELBOW_BCK, OUTPUT);
  pinMode(WRIST_FWD, OUTPUT);
  pinMode(WRIST_BCK, OUTPUT);
  pinMode(GRIP_CLOSE, OUTPUT);
  pinMode(GRIP_OPEN, OUTPUT);

  pinMode(EMERGENCY_STOP, INPUT);

  pinMode(EYE_ENABLE_LED, OUTPUT);

  pinMode(TORSO_ENABLE, OUTPUT);
  pinMode(SHOULDER_ENABLE, OUTPUT);
  pinMode(ELBOW_ENABLE, OUTPUT);
  pinMode(WRIST_ENABLE, OUTPUT);
  pinMode(GRIP_ENABLE, OUTPUT);
  pinMode(SPOTLIGHT, OUTPUT);

  Serial.begin(BAUDRATE);
  //Initilises the USB connection between the Arduino and the PC at 57600 bits per second.
  //We use a baudrate of 57600 because this is the communication speed of the Arduino IDE.
  //The Arduino and PC communication speeds must be the same or the data sent and recievd will be unintelligable.
}

byte ReadOneByte()           // One Byte Read Function
{
  int ByteRead;
  while (!Serial.available());
  ByteRead = Serial.read();
  return ByteRead;
}

void loop()                     // Main Loop Function
{
  if (ReadOneByte() == 170)       // First Data Snychronisation
  {
    if (ReadOneByte() == 170)     // Second Data Synchronisation
    {
      Plength = ReadOneByte();
      if (Plength > 169) return;  //Payload length can not be greater than 169

      if (Plength == 4) Small_Packet (); // Small Packets of data to determine eye enable function

      Attention_Parse ();  //Big packets of data used for attention

      Output_Function ();  //output function that uses data collected from Eye Blink and Attention functions
    }
  }
}

void Small_Packet ()
{
  generatedchecksum = 0;
  for (int i = 0; i < Plength; i++)
  {
    payloadDataS[i]     = ReadOneByte();      //Read payload into memory
    generatedchecksum  += payloadDataS[i] ;
  }
  generatedchecksum = 255 - generatedchecksum;
  checksum  = ReadOneByte();
  if (checksum == generatedchecksum)       // Varify Checksum
  {
    if (j < 512)
    {
      Raw_data  = ((payloadDataS[2] << 8) | payloadDataS[3]);
      if (Raw_data & 0xF000)
      {
        Raw_data = (((~Raw_data) & 0xFFF) + 1);
      }
      else
      {
        Raw_data = (Raw_data & 0xFFF);
      }
      Temp += Raw_data;
      j++;
    }
    else
    {
      Onesec_Rawval_Fun ();
    }
  }
}

void Onesec_Rawval_Fun ()
{
  Avg_Raw = Temp / 512;
  if (On_Flag == 0 && Off_Flag == 1)
  {
    if (n < 3)
    {
      Temp_Avg += Avg_Raw;
      n++;
    }
    else
    {
      Temp_Avg = Temp_Avg / 3;
      if (Temp_Avg < EEG_AVG)
      {
        On_Flag = 1; Off_Flag = 0;
      }
      n = 0; Temp_Avg = 0;
    }
  }
  Eye_Blink ();
  j = 0;
  Temp = 0;
}

void Eye_Blink ()
{
  if (Eye_Enable)
  {
    digitalWrite(EYE_ENABLE_LED, HIGH);
    if (On_Flag == 1 && Off_Flag == 0)
    {
      if ((Avg_Raw > Theshold_Eyeblink) && (Avg_Raw < 350))
      {
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        if (ls == 9) {
          ls = 0;
        }
        else {
          ls++;
        }
        delay(250);  //DELAY AFTER INCREMENT PREVENTS PROGRAM SKIPPING JOINTS
      }
      else
      {
        if (Avg_Raw > 350)
        {
          On_Flag = 0; Off_Flag = 1; //CHANGED COMPARATOR OPERATOR (==) TO ASSIGNMENT OPERATOR (=)
          digitalWrite(EYE_ENABLE_LED, LOW);
        }
      }
    }
  }
}

void Attention_Parse () {
  generatedchecksum = 0;
  for (int i = 0; i < Plength; i++) {
    payloadDataA[i] = ReadOneByte();            //Read payload into memory
    generatedchecksum += payloadDataA[i];
  }

  checksum = ReadOneByte();                      //Read checksum byte from stream
  generatedchecksum = 255 - generatedchecksum;   //Take one's compliment of generated checksum

  if (checksum == generatedchecksum) {

    Poorquality = 200;
    attention = 0;
    meditation = 0;

    for (int i = 0; i < Plength; i++) {   // For loop used to parse the payload
      switch (payloadDataA[i]) {
        case 2:
          i++;
          Poorquality = payloadDataA[i];
          if (Poorquality == 0 )
          {
            Eye_Enable = 1;
          }
          else
          {
            Eye_Enable = 0;
          }
          break;
        case 4:
          i++;
          attention = payloadDataA[i];
          break;
        case 0x80:
          i = i + 3;
          break;
        case 0x83:
          i = i + 25;
          break;
        default:
          break;
      } // switch
    } // for loop
  }
  else {  // Checksum Error
  }  // end if else for checksum
}

void Output_Function () {

  //ALTER OUTPUTS FOR NEW ARM CIRCUITRY

  switch (ls) {  //THESE LEDS REPRESENT WHICH JOIN IS BEING CONTROLLED
    //OUTPUT REGARDLESS OF ATTENTION VALUE (USER WILL KNOW WHICH JOINT THEY WILL BE CONTROLLING BEFORE THEY ACTUATE MOTION)
    //IF THE SPOTLIGHT LED IS ACTIVE, IT MEANS IT IS ACTUATING FORWARD CONTROL
    //IF THE SPOTLIGHT LED IS INACTIVE, IT MEANS IT IS ACTUATING REVERSE CONTROL
    case 0:
      digitalWrite(TORSO_ENABLE, HIGH);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, HIGH);
      break;
    case 1:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, HIGH);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, HIGH);
      break;
    case 2:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, HIGH);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, HIGH);
      break;
    case 3:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, HIGH);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, HIGH);
      break;
    case 4:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, HIGH);
      digitalWrite(SPOTLIGHT, HIGH);
      break;
    case 5:
      digitalWrite(TORSO_ENABLE, HIGH);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, LOW);
      break;
    case 6:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, HIGH);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, LOW);
      break;
    case 7:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, HIGH);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, LOW);
      break;
    case 8:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, HIGH);
      digitalWrite(GRIP_ENABLE, LOW);
      digitalWrite(SPOTLIGHT, LOW);
      break;
    case 9:
      digitalWrite(TORSO_ENABLE, LOW);
      digitalWrite(SHOULDER_ENABLE, LOW);
      digitalWrite(ELBOW_ENABLE, LOW);
      digitalWrite(WRIST_ENABLE, LOW);
      digitalWrite(GRIP_ENABLE, HIGH);
      digitalWrite(SPOTLIGHT, LOW);
      break;
  }
  if (digitalRead(EMERGENCY_STOP) == LOW && attention > 50) {
    switch (ls) {    //MAIN CONDITIONAL OUPUT; REQUIRES EMERGANCY STOP NOT ACTIVE AND HIGH ATTENTION ESENCE VALUE TO ACTUATE MOTORS
      case 0:    //TORSO LEFT ROTATION
        digitalWrite(TORSO_LEFT, HIGH);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 1:  //SHOULDER FORWARD ROTATION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, HIGH);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 2:  //ELBOW FORWARD ROTATION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, HIGH);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 3:  //WRIST FORWARD ROTATION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, HIGH);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 4:  //CLOSE GRIP CONTROL
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, HIGH);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 5:  //TORSO RIGHT ROTATION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, HIGH);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 6:  //SHOULDER BACKWARDS ROTATION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, HIGH);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 7:  //ELBOW BACKWARDS ROTATION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, HIGH);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 8:  //WRIST BACKWARD ROTAION
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, HIGH);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, LOW);
        break;
      case 9:  //OPEN GRIP CONTROL
        digitalWrite(TORSO_LEFT, LOW);
        digitalWrite(TORSO_RIGHT, LOW);
        digitalWrite(SHOULDER_FWD, LOW);
        digitalWrite(SHOULDER_BCK, LOW);
        digitalWrite(ELBOW_FWD, LOW);
        digitalWrite(ELBOW_BCK, LOW);
        digitalWrite(WRIST_FWD, LOW);
        digitalWrite(WRIST_BCK, LOW);
        digitalWrite(GRIP_CLOSE, LOW);
        digitalWrite(GRIP_OPEN, HIGH);
        break;
    }
  }
  else { //DRIVE ALL OUTPUTS LOW
    digitalWrite(TORSO_LEFT, LOW);
    digitalWrite(TORSO_RIGHT, LOW);
    digitalWrite(SHOULDER_FWD, LOW);
    digitalWrite(SHOULDER_BCK, LOW);
    digitalWrite(ELBOW_FWD, LOW);
    digitalWrite(ELBOW_BCK, LOW);
    digitalWrite(WRIST_FWD, LOW);
    digitalWrite(WRIST_BCK, LOW);
    digitalWrite(GRIP_CLOSE, LOW);
    digitalWrite(GRIP_OPEN, LOW);
  }
}

