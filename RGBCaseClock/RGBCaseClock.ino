#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    120
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120
void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

}

int i = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  FastLED.clear();


  lightPosition(i);
  delay(10);
  FastLED.show();

  i++;
  gHue++;
  if (i == 12)
  {
    i = 0;
  }
  //  EVERY_N_MILLISECONDS( 20 ) {
  //      // slowly cycle the "base color" through the rainbow
  //  }
  Serial.println(i);
}
void lightPosition(int var) {
  switch (var) {
    case 0:
      //do something
      for (int  j = 0; j < 11; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 1:
      //do something
      for (int  j = 10; j < 21; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 2:
      //do something
      for (int  j = 20; j < 31; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 3:
      //do something
      for (int j = 30; j < 41; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 4:
      //do something
      for (int j = 40; j < 51; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 5:
      //do something
      for (int  j = 50; j < 61; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 6:
      //do something
      for (int  j = 60; j < 71; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 7:
      //do something
      for (int  j = 70; j < 81; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 8:
      //do something
      for (int  j = 80; j < 91; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 9:
      //do something
      for (int j = 90; j < 101; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 10:
      //do something
      for (int  j = 100; j < 111; j++)
      {
        leds[j] = gHue;
      }
      break;

    case 11:
      //do something
      for (int  j = 110; j < 121; j++)
      {
        leds[j] = gHue;
      }
      break;

    default:
      //do something
      break;
  }
}
