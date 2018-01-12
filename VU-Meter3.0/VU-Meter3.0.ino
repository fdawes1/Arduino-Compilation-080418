#include <FastLED.h>

#include <EEPROM.h>

#define NUM_LEDS 30
#define DATA_PIN_left 6
#define DATA_PIN_right 7

#define COLOUR_ORDER  RGB
#define CHIPSET  WS2812B

#define FORWARD 0
#define BACKWARD 1

CRGB ledsleft[NUM_LEDS];
CRGB ledsright[NUM_LEDS];

#define MAX_BRIGHTNESS 120     // Thats full on, watch the power!
#define MIN_BRIGHTNESS 32       // set to a minimum of 25%

const int colourInPin = A0;
const int brightnessPin = A1;
int brightnessValue;
const int progPin = A5;
const int directionPin = 3;

int progstate = 0;
int prog = 0;
int direstate = 0;
int dire = 0;

boolean direction = FORWARD;

int timer1_counter;

#define BOARDLEDPIN 13

#define COOLING  75 //Less cooling = taller flames.  More cooling = shorter flame, suggested range 20-10
#define SPARKING 80 //lower chance = more flickery fire, suggested range 50-200.

#define FRAMES_PER_SECOND 60
CRGBPalette16 gPal;

void setup()
{

  FastLED.delay( 2000 ); // power-up safety delay
  FastLED.addLeds<WS2812B, DATA_PIN_left, COLOUR_ORDER>(ledsleft, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B, DATA_PIN_right, COLOUR_ORDER>(ledsright, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( MAX_BRIGHTNESS );

  pinMode(directionPin, INPUT_PULLUP);

  pinMode(BOARDLEDPIN, OUTPUT);

  Serial.begin(9600);

  for (int address = 0 ; address < EEPROM.length() ; address++)
  {
    EEPROM.write(address, 0);
  }

  noInterrupts();
  TCCR1A = 0; // DISABLE ALL INTTERUPTS
  TCCR1B = 0;

  timer1_counter = 34286; // preload timer time at 2Hz ------ 64911 = @ 100Hz ------ 64286 = @ 50Hz

  TCNT1 = timer1_counter; //preload timer
  TCCR1B |= (1 << CS12); //256 prescaler
  TIMSK1 |= (1 << TOIE1); //enable timer overflow interrupt
  interrupts(); //RE-ENABLE ALL INTERRUPTS

}

//main----------------------------------------------------------------------------------------------------------------------
ISR(TIMER1_OVF_vect) // isr = interrupt service routine
{
  TCNT1 = timer1_counter; //start timer
  digitalWrite(BOARDLEDPIN, !digitalRead(BOARDLEDPIN)); // flash board led to check isr working
  //need to check correct prog function is running
}



void loop() //main
{
  directionbutton();
  DIRECTION();

  programdial();
  program();

}
//----------------------------------------------------------------------------------------------------------------------
//FUNCTIONS
//----------------------------------------------------------------------------------------------------------------------
void directionbutton()
{
  direstate = digitalRead(directionPin);
  if (direstate == LOW)
  {
    dire++;
    delay(350);
  }
}
void programdial()// checked in every function at each iteration to remove having to waiting for function to finish
{
  int progstate = analogRead(A5); //program control
  prog = map(progstate, 0, 1023, 0, 12); // last number = no. of programs running
  //  Serial.write("program = ");
  //  Serial.println (prog);
}


//program----------------------------------------------------------------------------------------------------------------------
void program ()
{

  FastLED.clear();
  brightnessValue = map(analogRead(brightnessPin), 0, 1023, 0, 140);
  FastLED.setBrightness(constrain(brightnessValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  int valleft = analogRead(A2); //audio left
  int valright = analogRead(A3); //audio right

  int numLedsToLightleft = map(valleft, 0, 450, 0, NUM_LEDS);
  int numLedsToLightright = map(valright, 0, 450, 0, NUM_LEDS);

  int mappedHue;
  int mappedHueleft;
  int mappedHueright;


//  if (valleft <10)
//  {
//    ledsleft[NUM_LEDS] = CRGB::Black;
//    ledsright[NUM_LEDS] = CRGB::Black;
//
//  }
//  if (valright <10)
//  {
//    ledsleft[NUM_LEDS] = CRGB::Black;
//    ledsright[NUM_LEDS] = CRGB::Black;
//
//  }


  if (prog == 0)//normal
  {
    for (int led = 0; led < numLedsToLightleft; led++)
    {
      mappedHue = map(analogRead(colourInPin), 0, 1023, 0, 240);
      if (direction == FORWARD)
      {
        ledsleft[led] = CHSV(mappedHue, 255, 255);
      }
      else
      {
        ledsleft[NUM_LEDS - 1 - led] = CHSV(mappedHue, 255, 255);
      }
    }
    for (int led = 0; led < numLedsToLightright; led++)
    {
      mappedHue = map(analogRead(colourInPin), 0, 1023, 0, 240);
      if (direction == FORWARD)
      {
        ledsright[led] = CHSV(mappedHue, 255, 255);
      }
      else
      {
        ledsright[NUM_LEDS - 1 - led] = CHSV(mappedHue, 255, 255);
      }
    }
  }

  else  if (prog == 1)//random colours
  {
    for (int led = 0; led < numLedsToLightleft; led++)
    {
      mappedHue = map(analogRead(colourInPin), 0, 1023, 0, 240);
      if (direction == FORWARD)
      {
        ledsleft[led] = randomColour();
      }
      else
      {
        ledsleft[NUM_LEDS - 1 - led] = randomColour();
      }
    }
    for (int led = 0; led < numLedsToLightright; led++)
    {
      mappedHue = map(analogRead(colourInPin), 0, 1023, 0, 240);
      if (direction == FORWARD)
      {
        ledsright[led] = randomColour();
      }
      else
      {
        ledsright[NUM_LEDS - 1 - led] = randomColour();
      }
    }
  }

  else  if (prog == 2)//normal + rainbow/scrollable
  {
    mappedHueleft = map(analogRead(colourInPin), 0, 1023, 0, 240);
    mappedHueright = map(analogRead(colourInPin), 0, 1023, 0, 240);
    for (int led = 0; led < (numLedsToLightleft); ++led)
    {
      if (direction == FORWARD)
      {
        ledsleft[led] = (led - NUM_LEDS - 1, led, 0, CHSV(mappedHueleft, 240, 240));
        mappedHueleft += 5;
      }
      else
      {
        ledsleft[NUM_LEDS - 1 - led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueleft, 240, 240));
        mappedHueleft += 5;
      }
    }
    for (int led = 0; led < (numLedsToLightright); ++led)
    {
      if (direction == FORWARD)
      {
        ledsright[led] = (led - (NUM_LEDS) - 1, led, 0, CHSV(mappedHueright, 240, 240));
        mappedHueright += 5;
      }
      else
      {
        ledsright[NUM_LEDS - 1 - led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueright, 240, 240));
        mappedHueright += 5;
      }
    }
  }

  else if (prog == 3) //outer->> middle & middle --> outer
  {

    int Mvalleft = analogRead(A2); //audio left
    int Mvalright = analogRead(A3); //audio right

    int MnumLedsToLightleft = map(Mvalleft, 0, 600, 0, NUM_LEDS);
    int MnumLedsToLightright = map(Mvalright, 0, 600, 0, NUM_LEDS);

    mappedHueleft = map(analogRead(colourInPin), 0, 1023, 0, 240);
    mappedHueright = map(analogRead(colourInPin), 0, 1023, 0, 240);

    if (direction == FORWARD)
    {
      for (int led = 0; led < (MnumLedsToLightleft); ++led)
      {
        ledsleft[led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueleft, 240, 240));
        ledsleft[NUM_LEDS - 1 - led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueleft, 240, 240));
        mappedHueleft += 5;
      }
      for (int led = 0; led <  MnumLedsToLightright; ++led)
      {
        ledsright[led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueright, 240, 240));
        ledsright[NUM_LEDS - 1 - led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueright, 240, 240));
        mappedHueright += 5;
      }
    }
    else
    {
      for (int led = 15; led < (MnumLedsToLightleft + 15); ++led)
      {
        ledsleft[led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueleft, 240, 240));
        ledsleft[NUM_LEDS - 1 - led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueleft, 240, 240));
        mappedHueleft += 5;
      }

      for (int led = 15; led <  (MnumLedsToLightright + 15); ++led)
      {
        ledsright[led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueright, 240, 240));
        ledsright[NUM_LEDS - 1 - led] = (led - (NUM_LEDS / 2) - 1, led, 0, CHSV(mappedHueright, 240, 240));
        mappedHueright += 5;
      }
    }
  }

  else if (prog == 4)//single
  {
    //audio left-----------------------------------------------

    const int sampleWindowleft = 12.5; // Sample window width in mS (50 mS = 20Hz)
    unsigned int sampleleft;

    int gHue = map(analogRead(colourInPin), 0, 1023, 0, 240);

    unsigned long startMillisleft = millis(); // Start of sample window
    unsigned int peakToPeakleft = 0;   // peak-to-peak level

    unsigned int signalMaxleft = 0;
    unsigned int signalMinleft = 1023;

    // collect data for sample window time (ms)
    while (millis() - startMillisleft < sampleWindowleft)
    {
      sampleleft = analogRead(A2);
      if (sampleleft < 1023)  // toss out spurious readings
      {
        if (sampleleft > signalMaxleft)
        {
          signalMaxleft = sampleleft;  // save just the max levels
        }
        else if (sampleleft < signalMinleft)
        {
          signalMinleft = sampleleft;  // save just the min levels
        }
      }
    }
    peakToPeakleft = signalMaxleft - signalMinleft;  // max - min = peak-peak amplitude
    int levelleft = (peakToPeakleft * 54  / 1023);

    if (direction == FORWARD)
    {
      ledsleft[levelleft] = (levelleft - (NUM_LEDS / 2) - 1, levelleft, 0, CHSV(gHue, 240, 240));
    }
    else
    {
      ledsleft[levelleft] = (levelleft - (NUM_LEDS / 2) - 1, levelleft, 0, CHSV(gHue, 240, 240));
      ledsleft[NUM_LEDS - 1 - levelleft] = (levelleft - (NUM_LEDS / 2) - 1, levelleft, 0, CHSV(gHue, 240, 240));
    }


    //audio right-----------------------------------------------

    const int sampleWindowright = 12.5; // Sample window width in mS (50 mS = 20Hz)
    unsigned int sampleright;

    unsigned long startMillisright = millis(); // Start of sample window
    unsigned int peakToPeakright = 0;   // peak-to-peak level

    unsigned int signalMaxright = 0;
    unsigned int signalMinright = 1023;

    // collect data for sample window time (ms)
    while (millis() - startMillisright < sampleWindowright)
    {
      sampleright = analogRead(A3);
      if (sampleright < 1023)  // toss out spurious readings
      {
        if (sampleright > signalMaxright)
        {
          signalMaxright = sampleright;  // save just the max levels
        }
        else if (sampleright < signalMinright)
        {
          signalMinright = sampleright;  // save just the min levels
        }
      }
    }
    peakToPeakright = signalMaxright - signalMinright;  // max - min = peak-peak amplitude
    int levelright = (peakToPeakright * 54 / 1023);

    if (direction == FORWARD)
    {
      ledsright[levelright] = (levelright - (NUM_LEDS / 2) - 1, levelright, 0, CHSV(gHue, 240, 240));
    }
    else
    {
      ledsright[levelright] = (levelright - (NUM_LEDS / 2) - 1, levelright, 0, CHSV(gHue, 240, 240));
      ledsright[NUM_LEDS - 1 - levelright] = (levelright - (NUM_LEDS / 2) - 1, levelright, 0, CHSV(gHue, 240, 240));
    }
  }

  else if ( prog == 5) //lamp
  {
    for (int led = 0; led < NUM_LEDS; led++)
    {
      mappedHue = map(analogRead(colourInPin), 0, 1023, 0, 240);
      ledsleft[led] = CHSV(mappedHue, 255, 255);
      ledsright[led] = CHSV(mappedHue, 255, 255);
    }
  }

  else if (prog == 6) // fire
  {
    int firehue = map(analogRead(colourInPin), 0, 1023, 0, 240);

    CRGB darkcolour  = CHSV(firehue, 255, 192); // pure hue, three-quarters brightness
    CRGB lightcolour = CHSV((firehue - 25), 128, 255); // half 'whitened', full brightness

    gPal = CRGBPalette16( CRGB::Black, darkcolour, lightcolour, CRGB::White);

    // Add entropy to random number generator
    random16_add_entropy( random());

    Fire2012WithPalette(); // run simulation using palette colors

    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }

  else if (prog == 7)  //breath
  {
    //breath colour
    for (int led = 0; led < NUM_LEDS; led++)
    {
      int breathColour = map(analogRead(colourInPin), 0, 1023, 0, 240);
      ledsleft[led] = CHSV(breathColour, 255, 255);
      ledsright[led] = CHSV(breathColour, 255, 255);
    }

    int BrightnessMaxVal = map(analogRead(brightnessPin), 0, 1023, 0, 130);

    float breathlight = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;
    FastLED.setBrightness(constrain(breathlight, 0, BrightnessMaxVal));
    // FastLED.setBrightness(breathlight);
    FastLED.show();
  }

  else if (prog == 8)//juggle
  {
    // Several colored dots, weaving in and out of sync with each other

    uint8_t numdots = 4;                                                       // Number of dots in use.
    uint8_t faderate = 0.5;                                                      // How long should the trails be. Very low value = longer trails.
    uint8_t hueinc = 64;                                                       // Incremental change in hue between each dot.
    uint8_t thishue = 0;                                                       // Starting hue.
    uint8_t curhue = map(analogRead(colourInPin), 0, 1023, 0, 240);            // The current hue
    uint8_t thissat = 255;                                                     // Saturation of the colour.
    uint8_t thisbright = 255;                                                    // How bright should the LED/display be.
    uint8_t basebeat = 10;                                                      // Higher = faster movement.

    // Reset the hue values.
    fadeToBlackBy(ledsleft, NUM_LEDS, faderate);
    fadeToBlackBy(ledsright, NUM_LEDS, faderate);
    for ( int i = 0; i < numdots; i++)
    {
      ledsleft[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
      ledsright[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
      curhue += hueinc;
    }

    // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
    uint8_t secondHand = (millis() / 1000) % 30;                // IMPORTANT!!! Change '30' to a different value to change duration of the loop.
    static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
    if (lastSecond != secondHand)
    { // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
      if (secondHand ==  0)
      {
        numdots = 1;  // You can change values here, one at a time , or altogether.
        faderate = 2;
      }
      if (secondHand == 10)
      {
        numdots = 4;
        thishue = 128;
        faderate = 8;
      }
      if (secondHand == 20)
      {
        hueinc = 48;  // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
        thishue = random8();
      }
    }


  }
  else if (prog == 9) //colour wipe
  { //speed control
    int speed = map(analogRead(colourInPin), 0, 1023, 0, 150);
    for (int i = 0; i < 1; i++) // i < X (where X is number of cycles before reset) without interrupt on switches cannot cycle programs outside of reset time
    {
      colourWipe(randomColour(), speed, direction);
      direction = !direction;
    }
  }

  else if (prog == 10)//colour wipe  random colour bar
  {
    int speed = map(analogRead(colourInPin), 0, 1023, 0, 150);

    for (int i = 0; i < 1 ; i++) // i < X (where X is number of cycles before reset)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        if (direction == FORWARD)
        {
          ledsleft[i] = randomColour();
          ledsright[i] = randomColour();
        }
        else
        {
          ledsleft[NUM_LEDS - 1 - i] = randomColour();
          ledsright[NUM_LEDS - 1 - i] = randomColour();
        }
        FastLED.show();
        delay(speed);
      }
    }
  }
  else if (prog == 11)//cylon
  {
    int speed = map(analogRead(colourInPin), 0, 1023, 0, 150);
    cylon(randomColour(), 5, speed);
  }



  else if (prog == 12) //mood lighting
  {
    int speed = 0;
    int address = 0;
    byte range = 0;

    if (direction == FORWARD)
    {
      range = map(analogRead(colourInPin), 0, 1023, 0, 50);
      EEPROM.write(address, range);
    }

    else
    {
      speed = map(analogRead(colourInPin), 0, 1023, 0, 256);
      range = EEPROM.read(address);
      address = address + 1;
      if (address == EEPROM.length())
      {
        address = 0;
      }
    }

    for (int j = 0; j < 256; j++)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        ledsleft[i] = Wheel(((i * range / NUM_LEDS) + j) & 255);
        ledsright[i] = Wheel(((i * range / NUM_LEDS) + j) & 255);
      }
      FastLED.show();
      delay(speed);
    }
  }

  FastLED.show();//leave at end of program
}//program()
//direction----------------------------------------------------------------------------------------------------------------------------
void DIRECTION()
{

  if (dire == 0)
  {
    direction = FORWARD;
  }

  else if (dire == 1)
  {
    direction = !FORWARD;
  }

  else if (dire >= 2)
  {
    dire = 0;
  }

}//DIRECTION()

//wheel & randomColour----------------------------------------------------------------------------------------------------------------------
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}//Wheel()

CRGB randomColour() {
  return Wheel(random(256));
}//randomColour()


//cylon----------------------------------------------------------------------------------------------------------------------

void cylon(CRGB c, int width, int speed)
{
  // First slide the leds in one direction
  for (int i = 0; i <= NUM_LEDS - width; i++)
  {
    for (int j = 0; j < width; j++)
    {
      ledsleft[i + j] = c;
      ledsright[i + j] = c;
    }
    FastLED.show();
    // now that we've shown the leds, reset to black for next loop
    for (int j = 0; j < 5; j++)
    {
      ledsleft[i + j] = CRGB::Black;
      ledsright[i + j] = CRGB::Black;
    }
    delay(speed);
  }
  // Now go in the other direction.
  for (int i = NUM_LEDS - width; i >= 0; i--)
  {
    for (int j = 0; j < width; j++)
    {
      ledsleft[i + j] = c;
      ledsright[i + j] = c;
    }
    FastLED.show();
    for (int j = 0; j < width; j++)
    {
      ledsleft[i + j] = CRGB::Black;
      ledsright[i + j] = CRGB::Black;
    }
    delay(speed);
  }
}//cylon()

//colour wipe----------------------------------------------------------------------------------------------------------------------

// Wipes colour from end to end
void colourWipe(CRGB c, int speed, int direction)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (direction == FORWARD)
    {
      ledsleft[i] = c;
      ledsright[i] = c;

    }
    else {
      ledsleft[NUM_LEDS - 1 - i] = c;
      ledsright[NUM_LEDS - 1 - i] = c;

    }
    FastLED.show();
    delay(speed);
  }
}

//fire animation----------------------------------------------------------------------------------------------------------------------

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heatleft[NUM_LEDS];
  static byte heatright[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++)
  {
    heatleft[i] = qsub8( heatleft[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    heatright[i] = qsub8( heatright[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--)
  {
    heatleft[k] = (heatleft[k - 1] + heatleft[k - 2] + heatleft[k - 2] ) / 3;
    heatright[k] = (heatright[k - 1] + heatright[k - 2] + heatright[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING )
  {
    int y = random8(7);
    heatleft[y] = qadd8( heatleft[y], random8(160, 255) );
    heatright[y] = qadd8( heatright[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++)
  {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with colour palettes.
    byte colourindexleft = scale8( heatleft[j], 240);
    byte colourindexright = scale8( heatright[j], 240);

    CRGB colourleft = ColorFromPalette( gPal, colourindexleft);
    CRGB colourright = ColorFromPalette( gPal, colourindexright);

    int pixelnumberleft, pixelnumberright;

    if ( direction == !FORWARD )
    {
      pixelnumberleft = (NUM_LEDS - 1) - j;
      pixelnumberright = (NUM_LEDS - 1) - j;
    }
    else
    {
      pixelnumberleft = j;
      pixelnumberright = j;
    }
    ledsleft[pixelnumberleft] = colourleft;
    ledsright[pixelnumberright] = colourright;
  }
}

//todo
// interrupts
// memory for mood lights - SRAM not eeprom

//EXTRA
// 24HR CLOCK -- 30 leds on right for minutes -- 24 on left for hour -- flash top 5 on left for second -//- requires external real time clock


//------------------------------------------------------------END------------------------------------------------------------------------








