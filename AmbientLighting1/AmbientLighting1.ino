
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

  //initial values (no significance)
  int red = 255;
  int blue = 255;
  int green = 255;

Average<float> ave(10);

void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
 }
void loop()
{
  //protocol expects data in format of 4 bytes
  //(xff) as a marker to ensure proper synchronization always
  //followed by red, green, blue bytes
  if (Serial.available() >= 4) {
    if (Serial.read() == 0xff) {
      red = Serial.read();
      green = Serial.read();
      blue = Serial.read();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB(red, green, blue);
      }
    }
  }
  //finally control led brightness through pulse-width modulation
int i;
for (int i = 0; i < 1200; i++)
  {
    ave.push(map(analogRead(A0), 0, 1023, 0, 255));
  }
  int x = (int)ave.get(i);
  
    BRIGHTNESS = x;

    FastLED.setBrightness(BRIGHTNESS);


  FastLED.show();

  delay(10); //just to be safe
}



