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
#define NUM_LEDS    30
// CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
int BRIGHTNESS = 20;
#define FRAMES_PER_SECOND  120

#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };


#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);
unsigned long delaytime = 250;

int hourNow;
int  minuteNow;
int SecondNow;

void setup()
{

  Wire.begin();
  Serial.begin(9600);
  rtc.begin(); // Initialize the rtc object

  delay(1000); // 1 second delay for recovery

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);
}

Average<float> ave(10);

int i = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void printNumber(int v) {
  int ones;
  int tens;
  int hundreds;
  boolean negative;

  if (v < -999 || v > 999)
    return;
  if (v < 0) {
    negative = true;
    v = v * -1;
  }
  ones = v % 10;
  v = v / 10;
  tens = v % 10;
  v = v / 10;
  hundreds = v;
  if (negative) {
    //print character '-' in the leftmost column
    lc.setChar(0, 3, '-', false);
  }
  else {
    //print a blank in the sign column
    lc.setChar(0, 3, ' ', false);
  }
  //Now print the number digit by digit
  lc.setDigit(0, 2, (byte)hundreds, false);
  lc.setDigit(0, 1, (byte)tens, false);
  lc.setDigit(0, 0, (byte)ones, false);
}

void scrollDigits()
{

  lc.setDigit(0, 7, 0, false);
  lc.setDigit(0, 6, 0, false);
  lc.setDigit(0, 5, 0, false);
  lc.setDigit(0, 4, 0, false);
  lc.setDigit(0, 3, 0, false);
  lc.setDigit(0, 2, 0, false);
  lc.setDigit(0, 1, 0, false);
  lc.setDigit(0, 0, 0, false);

  delay(1000);
  lc.clearDisplay(0);
}
void loop()
{
  BRIGHTNESS = map(analogRead(A0), 0, 1023, 0, 150);
  delay(2);

  DateTime now = rtc.now(); //get the current date-time

  
  //  scrollDigits();
  printNumber(minuteNow);
  Serial.print("Time:  ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.println("");


  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::BurlyWood;
  }
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();

}




