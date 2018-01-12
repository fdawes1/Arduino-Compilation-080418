

#include <avr/pgmspace.h>  //AVR library for writing to ROM
#include <Charliplexing.h> //Imports the library, which needs to be
//Initialized in setup.

//Sets the time each frame is shown (milliseconds)
const unsigned int blinkdelay = 75;
boolean fadeMode = false;
byte brightness = 5; //Brightness goes from 0-7

/*
  The BitMap array is what contains the frame data. Each line is one full frame.
  Since each number is 16 bits, we can easily fit all 14 LEDs per row into it.
  The number is calculated by adding up all the bits, starting with lowest on
  the left of each row. 18000 was chosen as the kill number, so make sure that
  is at the end of the matrix, or the program will continue to read into memory.

  Here PROGMEM is called, which stores the array into ROM, which leaves us
  with our RAM. You cannot change the array during run-time, only when you
  upload to the Arduino. You will need to pull it out of ROM, which is covered
  below. If you want it to stay in RAM, just delete PROGMEM
*/
PROGMEM const uint16_t BitMap[][9] = {
  //Diaganal swipe across the screen


  {0, 1, 2, 3, 4, 5, 6, 7, 8},
    {0, 1, 2, 3, 4, 5, 6, 7, 8},
      {0, 1, 2, 3, 4, 5, 6, 7, 8},
        {0, 1, 2, 3, 4, 5, 6, 7, 8},
          {0, 1, 2, 3, 4, 5, 6, 7, 8},
            {0, 1, 2, 3, 4, 5, 6, 7, 8},
  



  {18000}
};

void setup() {
  LedSign::Init(GRAYSCALE);  //Initializes the screen
}
void loop() {

  if (fadeMode)
    for (uint8_t gray = 1; gray < SHADES; gray++)
      DisplayBitMap(gray);  //Displays the bitmap
  else DisplayBitMap(brightness);  //Displays the bitmap
}

void DisplayBitMap(uint8_t grayscale)
{
  boolean run = true;  //While this is true, the screen updates
  byte frame = 0;      //Frame counter
  byte line = 0;       //Row counter
  unsigned long data;  //Temporary storage of the row data
  unsigned long start = 0;

  while (run == true) {

    for (line = 0; line < 9; line++) {

      //Here we fetch data from program memory with a pointer.
      data = pgm_read_word_near (&BitMap[frame][line]);

      //Kills the loop if the kill number is found
      if (data == 18000) {
        run = false;
      }

      //This is where the bit-shifting happens to pull out
      //each LED from a row. If the bit is 1, then the LED
      //is turned on, otherwise it is turned off.
      else for (byte led = 0; led < 14; ++led) {
          if (data & (1 << led)) {
            LedSign::Set(led, line, grayscale);
          }
          else {
            LedSign::Set(led, line, 0);
          }
        }
    }

    LedSign::Flip(true);

    unsigned long end = millis();
    unsigned long diff = end - start;
    if ( start && (diff < blinkdelay) )
      delay( blinkdelay - diff );
    start = end;

    frame++;
  }
}
