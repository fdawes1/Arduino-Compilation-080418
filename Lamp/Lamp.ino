#include "FastLED.h"
#include "Average.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    6
CRGB leds[NUM_LEDS];

int BRIGHTNESS = 30;
Average<float> ave(10);
void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

}
int i = 0;
void loop()
{



  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::BurlyWood;
  }
  FastLED.show();
  Serial.println(analogRead(A0));
}
