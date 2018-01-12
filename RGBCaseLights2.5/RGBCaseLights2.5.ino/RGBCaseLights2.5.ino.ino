#include "FastLED.h"
#define NUM_LEDS 120
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

#define FORWARD 0
boolean direction = FORWARD;

String src_msg = "";

int msgProg;
int msgHue1;

int msgDire;
int msgCount;
int msgBrightness = 33;

int msgTime;

int FuncHue;
int randHue;
uint8_t gHue = 0;

int Delay;


void setup() {

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  Serial.begin(9600);

}

void loop() {
program();
   
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(FuncHue, 255, 255 );
    }
    FastLED.show();
}


void program()
{
  FuncHue =  msgHue1;
}
void serialEvent()
{
  if (Serial.available())
  { 
    
    
    char inChar = (char) Serial.read();
    if (inChar != 'E')
    {
      src_msg += inChar;
    }


    if (inChar == 'E')
    {
      src_msg += inChar;
    
      /*
        Needed Things:

        P  1  Program P
        H  2  Colour Choices (2+)
        D  3  dIrection
        C  5  counter Number (balls etc)
        B  6  bRightness
        T  7  Time       (not yet)
        X
      */


      msgProg =   getSrcValue(src_msg, 'P', 'H');

      msgHue1 = getSrcValue(src_msg, 'H', 'D');

      msgDire = getSrcValue(src_msg , 'D', 'C');

      msgCount = getSrcValue (src_msg, 'C', 'B');

      msgBrightness = getSrcValue(src_msg, 'B', 'E');

      //msgTime = getSrcValue(src_msg, 'T', 'E');

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


