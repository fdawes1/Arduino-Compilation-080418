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
int BRIGHTNESS = 20 ;
#define FRAMES_PER_SECOND  60
void setup() {

  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{


  for (int i = 10; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(gHue, 255, 255);
    FastLED.show();
    
    // clear this led for the next time around the loop
    leds[i] = CRGB::Black;
    delay(545);
  }

    fadeToBlackBy( leds, NUM_LEDS, 20);
  //fill_rainbow( leds, NUM_LEDS, gHue, 2);


  //  FastLED.show();

  gHue += 5;
  //delay(545);
}




