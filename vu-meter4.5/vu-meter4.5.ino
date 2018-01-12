#include <FastLED.h>
#include <stdlib.h>




#define NUM_LEDS 120
#define DATA_PIN_left 2
#define DATA_PIN_right 6

#define COLOUR_ORDER  RGB
#define CHIPSET  WS2812B

#define FORWARD 0

CRGBArray<NUM_LEDS> ledsleft;
CRGBArray<NUM_LEDS> ledsright;      //initialising like this allows use of FastLed Functions


#define MAX_BRIGHTNESS 120     // Thats full on, watch the power!
#define MIN_BRIGHTNESS 32       // set to a minimum of 25%


int hourLight ;
int minLight ;
int secondLight ;

int brightnessValue;

int prog = 0;
int dire = 0;

boolean direction = FORWARD;

int timer1_counter;

String src_msg = "B0P0H0SDJXYE";

int msgHue;
int msgSens;
int msgBrightness;
int msgDire;
int msgBallCount;
int msgX;
int msgY;

int sensitivity;

int mappedHue;
int mappedHueleft;
int mappedHueright;
int fireHue;
int speed;
int breathColour;
int breathRate = 3000.0;
int randHue;
int x;
int y;

uint8_t numdots = 1;
int newHue = 0;

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
  FastLED.setDither(0);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  

  // The following lines can be uncommented to set the date and time
  //rtc.setTime(12, 02, 0);     // Set the time to hr:min:sec (24hr format)


  Serial.begin(9600);

  pinMode(13, OUTPUT);

  noInterrupts();
  TCCR1A = 0; // DISABLE ALL INTTERUPTS
  TCCR1B = 0;

  timer1_counter = 34286; // preload timer time at 2Hz ------ 64911 = @ 100Hz ------ 64286 = @ 50Hz

  TCNT1 = timer1_counter; //preload timer
  TCCR1B |= (1 << CS12); //256 prescaler
  TIMSK1 |= (1 << TOIE1); //enable timer overflow interrupt
  interrupts(); //RE-ENABLE ALL INTERRUPTS
}

// --------------------------------------  ISR Routine ---------------------------------------------------------------------
ISR(TIMER1_OVF_vect) // isr = interrupt service routine
{
  TCNT1 = timer1_counter; //start timer
//  digitalWrite(13, !digitalRead(13)); // flash board led to check isr working
  serialEvent(); // check string at 2 Hz

}
//main----------------------------------------------------------------------------------------------------------------------

void loop() //main
{
     Serial.println("  Serial COM PORT Issue  ");
  DIRECTION();
  program();
}


//program----------------------------------------------------------------------------------------------------------------------
void program ()
{
  FastLED.clear(); //refresh strips

  brightnessValue = msgBrightness + 10; //(cannot turn lights off as min = 10) 
  FastLED.setBrightness(constrain(brightnessValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  int valleft = analogRead(A1); //audio left
  int valright = analogRead(A0); //audio right

  sensitivity = msgSens * 20; // careful with max sensitivity --> causes too much power to be pulled and crashes system.

  int numLedsToLightleft = map(valleft, 0, sensitivity, 0, NUM_LEDS);     //sensitivity was 450 (increasing makes less sensitive)
  int numLedsToLightright = map(valright, 0, sensitivity, 0, NUM_LEDS);   //sensitivity was 450 (increasing makes less sensitive)

  if (prog == 0)//normal
  {
    for (int led = 0; led < numLedsToLightleft; led++)
    {
      mappedHue = msgHue;
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
      mappedHue = msgHue;
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
    static uint8_t hue;
    for (int led = 0; led < numLedsToLightleft; led++)
    {
      mappedHue = msgHue;
      if (direction == FORWARD)
      {
        ledsleft.fadeToBlackBy(40); // hold colour for longer making it more visually appealing
        ledsleft[led] = CHSV(hue++, 255, 255);
      }
      else
      {
        ledsleft.fadeToBlackBy(40);
        ledsleft[NUM_LEDS - 1 - led] =  CHSV(hue++, 255, 255);
      }
    }
    for (int led = 0; led < numLedsToLightright; led++)
    {
      mappedHue = msgHue;
      if (direction == FORWARD)
      {
        ledsright.fadeToBlackBy(40);
        ledsright[led] = CHSV(hue++, 255, 255);
      }
      else
      {
        ledsright.fadeToBlackBy(40);
        ledsright[NUM_LEDS - 1 - led] =  CHSV(hue++, 255, 255);
      }
    }
  }

  else  if (prog == 2)//normal + rainbow/scrollable
  {
    mappedHueleft = msgHue;
    mappedHueright = msgHue;

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

    int Mvalleft = analogRead(A1); //audio left
    int Mvalright = analogRead(A0); //audio right

    sensitivity = msgSens * 20; // careful with max sensitivity --> causes too much power to be pulled and crashes system.

    int MnumLedsToLightleft = map(Mvalleft, 0, sensitivity, 0, NUM_LEDS);  //sensitivity was 600
    int MnumLedsToLightright = map(Mvalright, 0, sensitivity, 0, NUM_LEDS); //sensitivity was 600

    mappedHueleft = msgHue;
    mappedHueright = msgHue;

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

  else if (prog == 4) // Beat Detection -- single
  {
    static uint8_t hue;
    sensitivity = msgSens * 15;
    int valleft = analogRead(A1); //audio left
    int valright = analogRead(A0); //audio left
    int numLedsToLightleft = map(valright, 0, sensitivity, 0, NUM_LEDS);
    int numLedsToLightright = map(valright, 0, sensitivity, 0, NUM_LEDS);
    int faderate = msgBallCount ;
    mappedHueleft = msgHue;
    mappedHueright = msgHue;



    if ((numLedsToLightleft > (NUM_LEDS / 2) - 1) && (numLedsToLightright > (NUM_LEDS / 2) - 1)) // using both audio channels removes pulses from unwanted instruments in only 1 ear
    {
      if (direction == FORWARD)
      {
        for (int i = 0; i  < (NUM_LEDS); i++)
        {
          // fade everything out
          ledsleft.fadeToBlackBy(faderate);
          ledsright.fadeToBlackBy(faderate);


          //setled value
          ledsleft[i] = CHSV(mappedHueleft, 255, 255);
          ledsright[i] = CHSV(mappedHueright, 255, 255);
          mappedHueleft += 5;
          mappedHueright += 5;
          FastLED.delay(15);
        }
      }
      else {
        for (int i = 0; i  < (NUM_LEDS); i++)
        {
          // fade everything out
          ledsleft.fadeToBlackBy(faderate);
          ledsright.fadeToBlackBy(faderate);


          //setled value
          ledsleft[NUM_LEDS - 1 - i] = CHSV(mappedHueleft, 255, 255);
          ledsright[NUM_LEDS - 1 - i] = CHSV(mappedHueright, 255, 255);
          mappedHueleft += 5;
          mappedHueright += 5;
          FastLED.delay(15);
        }
      }
    }


  }

  else if (prog == 5) //beat detection -- double
  {

    static uint8_t hue;
    sensitivity = msgSens * 15;
    int valleft = analogRead(A1); //audio left
    int valright = analogRead(A0); //audio left
    int numLedsToLightleft = map(valright, 0, sensitivity, 0, NUM_LEDS);     //sensitivity was 450
    int numLedsToLightright = map(valright, 0, sensitivity, 0, NUM_LEDS);     //sensitivity was 450
    int faderate =   msgBallCount ;
    mappedHueleft = msgHue;
    mappedHueright = msgHue;

    if ((numLedsToLightleft > (NUM_LEDS / 2) - 1) && (numLedsToLightright > (NUM_LEDS / 2) - 1)) // using both audio channels removes pulses from unwanted instruments in only 1 ear
    {
      if (direction == FORWARD)
      {
        for (int i = 0; i  < (NUM_LEDS / 2 ); i++)
        {
          // fade everything out
          ledsleft.fadeToBlackBy(faderate);
          ledsright.fadeToBlackBy(faderate);


          //setled value
          ledsleft[(NUM_LEDS / 2) - 1 - i] = CHSV(mappedHueleft, 255, 255);
          ledsright[(NUM_LEDS / 2) - 1 - i] = CHSV(mappedHueright, 255, 255);
          mappedHueleft += 5;
          mappedHueright += 5;

          // copy bottom 15 leds to the top 15 leds,
          ledsleft(NUM_LEDS / 2 , NUM_LEDS - 1) = ledsleft(NUM_LEDS / 2 - 1 , 0);
          ledsright(NUM_LEDS / 2 , NUM_LEDS - 1) = ledsright(NUM_LEDS / 2 - 1 , 0);
          FastLED.delay(15);
        }
      }
      else
      {
        for (int i = 0; i < NUM_LEDS / 2; i++)
        {
          // fade everything out
          ledsleft.fadeToBlackBy(faderate);
          ledsright.fadeToBlackBy(faderate);

          ledsleft[i] = CHSV(mappedHueleft, 255, 255);
          ledsright[i] = CHSV(mappedHueright, 255, 255);
          mappedHueleft += 5;
          mappedHueright += 5;

          ledsleft(NUM_LEDS / 2 , NUM_LEDS - 1) = ledsleft(NUM_LEDS  / 2 - 1 , 0);
          ledsright(NUM_LEDS / 2 , NUM_LEDS - 1) = ledsright(NUM_LEDS  / 2 - 1 , 0);
          FastLED.delay(15);
        }
      }
    }
  }

  else if (prog == 6) // beat detection random position
  {
    mappedHue = msgHue;
    sensitivity = msgSens * 15;
    int faderate = 128;
    int width = 5;
    int posMax = NUM_LEDS - width;
    int posMin = 0 + width;
    int pos = constrain(NUM_LEDS, posMin, posMax);
    int startPoint = random(pos);

    if ((numLedsToLightleft > (NUM_LEDS / 2) - 1) && (numLedsToLightright > (NUM_LEDS / 2) - 1)) // using both audio channels removes pulses from unwanted instruments in only 1 ear
    {
      if (direction == FORWARD) // middle --> out
      {
        int i = startPoint;
        int j = startPoint;
        while ((i <= (startPoint + width)) && (j >= (startPoint - width)) && (j >= 0))
        {
          ledsleft.fadeToBlackBy(faderate);
          ledsright.fadeToBlackBy(faderate);
          ledsleft[i] = CHSV(mappedHue, 255, 255);
          ledsright[i] = CHSV(mappedHue, 255, 255);

          ledsleft[j] = CHSV(mappedHue, 255, 255);
          ledsright[j] = CHSV(mappedHue, 255, 255);

          i++;
          j--;
          mappedHue += 15;
          FastLED.delay(25);
        }
      }
      else // out --> middle
      {
        int i = startPoint + width;
        int j = startPoint - width;
        while ((i >= (startPoint)) && (j <= (startPoint)) && (j >= 0))
        {
          ledsleft.fadeToBlackBy(faderate);
          ledsright.fadeToBlackBy(faderate);
          ledsleft[i] = CHSV(mappedHue, 255, 255);
          ledsright[i] = CHSV(mappedHue, 255, 255);

          ledsleft[j] = CHSV(mappedHue, 255, 255);
          ledsright[j] = CHSV(mappedHue, 255, 255);

          i--;
          j++;
          mappedHue += 15;
          FastLED.delay(25);
        }
      }
    }
  }
  else if (prog == 7)//single
  {
    //audio left-----------------------------------------------

    const int sampleWindowleft = 12.5; // Sample window width in mS (50 mS = 20Hz)
    unsigned int sampleleft;

    int gHue = msgHue;

    unsigned long startMillisleft = millis(); // Start of sample window
    unsigned int peakToPeakleft = 0;   // peak-to-peak level

    unsigned int signalMaxleft = 0;
    unsigned int signalMinleft = 1023;

    // collect data for sample window time (ms)
    while (millis() - startMillisleft < sampleWindowleft)
    {
      sampleleft = analogRead(A1);
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
      sampleright = analogRead(A0);
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

  else if ( prog == 8) //lamp
  {
    for (int led = 0; led < NUM_LEDS; led++)
    {
      mappedHue = msgHue;
      ledsleft[led] = CHSV(mappedHue, 255, 255);
      ledsright[led] = CHSV(mappedHue, 255, 255);
    }
  }

  else if (prog == 9) //breathe
  {
    breathColour = msgHue;
    breathRate = msgBallCount * 1000.0; //(rate * Delay)

    if (direction == FORWARD)
    {
      for (int led = 0; led < NUM_LEDS; led++)
      {
        ledsleft[led] = CHSV(breathColour, 255, 255);
        ledsright[led] = CHSV(breathColour, 255, 255);
      }
    }
    else
    {
      for (int led = 0; led < NUM_LEDS; led++)
      {
        ledsleft[led] = CHSV(randHue, 255, 255);
        ledsright[led] = CHSV(randHue, 255, 255);
      }
    }

    int BrightnessMaxVal = msgBrightness * 1.4; //max 140
    float breathlight = (exp(sin(millis() / (msgBallCount * 1000.0) * PI)) - 0.36787944) * 108.0;

    if (breathlight <= 1 )
    {
      randHue = random(230);
    }

    FastLED.setBrightness(constrain(breathlight, 0, BrightnessMaxVal));
    FastLED.show();
  }

  else if (prog == 10)  //fire
  {
    fireHue = msgHue;

    CRGB darkcolour  = CHSV(fireHue, 255, 192); // pure hue, three-quarters brightness
    CRGB lightcolour = CHSV((fireHue - 25), 128, 200); // half 'whitened', full brightness

    gPal = CRGBPalette16( CRGB::Black, darkcolour, lightcolour, CRGB::White);

    // Add entropy to random number generator
    random16_add_entropy( random());

    Fire2012WithPalette(); // run simulation using palette colors

    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }

  else if (prog == 11)//juggle
  {
    // Several colored dots, weaving in and out of sync with each other

    numdots = msgBallCount;                                                       // Number of dots in use.
    uint8_t hueinc = 60;                                                       // Incremental change in hue between each dot.
    uint8_t thishue = 0;                                                       // Starting hue.
    uint8_t curhue = msgHue;                                                           // The current hue
    uint8_t thissat = 255;                                                     // Saturation of the colour.
    uint8_t thisbright = 255;                                                    // How bright should the LED/display be.
    uint8_t basebeat = 10;                                                      // Higher = faster movement.

    if (direction == FORWARD)
    {
      thissat = 255; //full colours
    }
    else
    {
      thissat = 125; //pastel colours
    }

    for ( int i = 0; i < numdots; i++)
    {
      ledsleft[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
      ledsright[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
      curhue += hueinc;
    }
  }

  else if (prog == 12) // rainbow
  {
    speed = msgHue * 0.58824; //scales msgHue form 0-255 to 0-150
    fill_rainbow(ledsleft, NUM_LEDS, newHue, 7);
    fill_rainbow(ledsright, NUM_LEDS, newHue, 7);

    if (direction == FORWARD )
    {
      newHue += 5;
    }
    else
    {
      newHue -= 5;
    }
    delay(speed);
  }


  else if (prog == 13) //bpm -- work in progress
  { // coloured stripes pulsing at a (currently) defined bpm

    CRGBPalette16 palette = RainbowColors_p;

    newHue = msgHue;
    int sensitvity = msgSens;
    int beatValleft = analogRead(A1);
    int beatValright = analogRead(A1);

    /*add a buffer to sample the audio and determine the current bpm.

       possibly use buffer? sample window?

    */
    uint8_t BeatsPerMinute = msgBallCount; // 62; //needs to be to be adjustable in accoradance to current audio bpm
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);


    if (direction == FORWARD) {
      for ( int i = 0; i < NUM_LEDS; i++)
      { int testVar =  beat - newHue + (i * 10);

        ledsleft[i] = ColorFromPalette(palette, newHue + (i * 2), testVar);
        ledsright[i] = ColorFromPalette(palette, newHue + (i * 2), testVar);
      }
    }
    else
    {
      for ( int i = 0; i < NUM_LEDS; i++)
      {
        ledsleft[NUM_LEDS - 1 - i] = ColorFromPalette(palette, newHue + (i * 2), beat - newHue + (i * 10));
        ledsright[NUM_LEDS - 1 - i] = ColorFromPalette(palette, newHue + (i * 2), beat - newHue + (i * 10));
      }
    }
  }


  else if (prog == 14)//LED controlled by Mouse Position
  {
    newHue = msgHue;
    mappedHueleft = newHue;
    mappedHueright = newHue;
    x = msgX;
    y = msgY;

    int max_x = 1920 + 5; //allow values to over extend -- these values are for screen resolution 1920*1080
    int max_y = 1080 + 5;
    if (direction == FORWARD)
    {
      int numLedsToLightleft = map(x, 0, max_x, 0, NUM_LEDS);
      int numLedsToLightright = map(y, 0,  max_y, 0, NUM_LEDS);

      ledsleft[numLedsToLightleft] = CHSV((newHue + (2 * numLedsToLightleft)), 255, 255);
      ledsright[NUM_LEDS - 1 - numLedsToLightright] = CHSV((newHue + (2 * numLedsToLightright)), 255, 255);
    }
    else
    {
      int numLedsToLightleft = map(x, 0, max_x, 0, NUM_LEDS + 1);
      int numLedsToLightright = map(y, 0,  max_y, 0, NUM_LEDS + 1);

      for (int led = 0; led < numLedsToLightleft; led++)
      {
        ledsleft[led] = CHSV(mappedHueleft, 255, 255);
        mappedHueleft += 5;
      }
      for (int led = 0; led < numLedsToLightright; led++)
      {
        ledsright[NUM_LEDS - 1 - led] = CHSV(mappedHueright, 255, 255);
        mappedHueright += 5;
      }

    }
  }
  else if (prog == 15)
  {
 
    int mappedHueHour = hour() * 8;
    int mappedHueMin = minute() * 12;
    int mappedHueSec = second() * 4;
    int secWidth = 2;
    secWidth = msgBallCount;

    if (direction == FORWARD)
    {
      //-----------Hours
      ledsleft[ hour()] = CHSV(mappedHueHour, 255, 255);
      mappedHueHour += 5;
      //-----------Minutes
      int minLeds = map(minute(), 0, 60, 0, NUM_LEDS);
      ledsright[minLeds] = CHSV(mappedHueMin, 255, 255);
      mappedHueMin += 5;
      //-----------Seconds
      if ((second() % 2) == 0)
      {
        for (int i = (NUM_LEDS - secWidth); i < NUM_LEDS; i++)
        {
          ledsleft[i] =  CHSV(mappedHueSec, 255, 255);
          mappedHueSec += 8;
        }
      }
      else
      { for (int i = (NUM_LEDS - secWidth); i < NUM_LEDS; i++)
        {

          ledsleft[i] = CRGB::Black;
        }
      }
    }
    else
    {
      for (int i = 0; i < hour(); i++)
      {
        ledsleft[i + 1] = CHSV(mappedHueHour, 255, 255);
        mappedHueHour += 5;
      }
      //-----------Minutes
      int minLeds = map(minute(), 0, 60, 0, NUM_LEDS);
      for (int j = 0; j < minLeds; j++)
      {
        ledsright[j] = CHSV(mappedHueMin, 255, 255);
        mappedHueMin += 5;
      }
      //-----------Seconds
      if ((second() % 2) == 0)
      {
        for (int i = (NUM_LEDS - secWidth); i < NUM_LEDS; i++)
        {
          ledsleft[i] =  CHSV(mappedHueSec, 255, 255);
          mappedHueSec += 8;
        }
      }
      else
      { for (int i = (NUM_LEDS - secWidth); i < NUM_LEDS; i++)
        {
          ledsleft[i] = CRGB::Black;
        }
      }
    }
  }

  FastLED.show();                     //leave at end of 'program'
} //program()

//--------------------------------------------------------------------------------------------------------------------------------------------------------------


//direction----------------------------------------------------------------------------------------------------------------------------
void DIRECTION()
{
  dire = msgDire;

  if (dire == 0)
  {
    direction = FORWARD;
  }

  else if (dire == 1)
  {
    direction = !FORWARD;
  }

}//DIRECTION()


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

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    ledsleft[ random16(NUM_LEDS) ] += CRGB::White;
    ledsright[ random16(NUM_LEDS) ] += CRGB::White;
  }
}



//----------------------------------- Visual Studio Code ----------------------------------------------------------------------------------------------------------------------


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
    { // B # P # H # S # D # J # X # Y # E
      src_msg += inChar;

      msgBrightness = getSrcValue(src_msg, 'B', 'P');

      prog =   getSrcValue(src_msg, 'P', 'H');

      msgHue = getSrcValue(src_msg, 'H', 'S');

      msgSens = getSrcValue(src_msg, 'S', 'D');

      msgDire = getSrcValue(src_msg , 'D', 'J');

      msgBallCount = getSrcValue (src_msg, 'J', 'X');

      msgX = getSrcValue (src_msg, 'X', 'Y');

      msgY = getSrcValue (src_msg, 'Y', 'E');

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
//------------------------------------------------------------END------------------------------------------------------------------------



//todo
// 24HR CLOCK -- 30 leds on right for minutes -- 24 on left for hour -- flash top 5 on left for second ¦¦ requires external real time clock
//map keyboard ascii values to lights -- each key (choose 30) turns on each LED and then dims after 1/2/3 seconds



