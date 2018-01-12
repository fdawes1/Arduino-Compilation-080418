#include "FastLED.h"
#include <FastLED.h>
#include <Average.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    120
CRGBArray<NUM_LEDS> leds;
int BRIGHTNESS = 0;
#define FRAMES_PER_SECOND  120

String src_msg;
void setup() {

  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  BRIGHTNESS = map(analogRead(A0), 0, 1023, 0, 200);
  
  //  for (int i = 0; i < NUM_LEDS; i++)
  //  {
  //    leds[i] = CHSV(gHue, 255, 255);
  //  }

  fadeToBlackBy( leds, NUM_LEDS, 20);
  fill_rainbow( leds, NUM_LEDS, gHue, 2);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();

  gHue += 2;
  delay(500);
}


void serialEvent()
{
  if (Serial.available())
  {
    char inChar = (char) Serial.read();
    if (inChar != ' ')
    {
      src_msg += inChar;
    }


    if (inChar == 'E')
    { // B # P # H # S # D # J # X # Y # E
      src_msg += inChar;

//      msgBrightness = getSrcValue(src_msg, 'B', 'P');
//
//      prog =   getSrcValue(src_msg, 'P', 'H');
//
//      msgHue = getSrcValue(src_msg, 'H', 'S');
//
//      msgSens = getSrcValue(src_msg, 'S', 'D');
//
//      msgDire = getSrcValue(src_msg , 'D', 'J');
//
//      msgBallCount = getSrcValue (src_msg, 'J', 'X');
//
//      msgX = getSrcValue (src_msg, 'X', 'Y');
//
//      msgY = getSrcValue (src_msg, 'Y', 'E');

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


