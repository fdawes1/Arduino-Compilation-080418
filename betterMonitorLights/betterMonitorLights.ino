#include "FastLED.h"
#include <FastLED.h>
#include "Average.h"
#include <Average.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60
// CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
int BRIGHTNESS = 60;
#define FRAMES_PER_SECOND  120
void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

}

Average<float> ave(10);

int i = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(gHue, 255, 255);
    }


  for (int i = 0; i < 1200; i++)
  {
    ave.push(map(analogRead(A0), 0, 1023, 0, 200));
  }
  int x = (int)ave.get(i);
  
  
  
    BRIGHTNESS = x;
  
  
    FastLED.setBrightness(BRIGHTNESS);
    gHue += 5;

  
    fadeToBlackBy( leds, NUM_LEDS, 20);
   // fill_rainbow( leds, NUM_LEDS, gHue, 2);
    FastLED.show();
    delay (100);
}





