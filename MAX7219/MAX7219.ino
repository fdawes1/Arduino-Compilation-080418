//Demonstrates
//the use of MAX7219 and 2 of 4x7 Segment display.
//
//The Arduino
//circuit connection for 7219 :
//
//* MAX7219
//DIN pin to digital pin 4
//
//* MAX7219
//LOAD pin to digital pin 3
//
//* MAX7219
//CLOCK pin to digital pin 2

* /

#include

// inputs: DIN pin, CLK pin, LOAD pin. number of
chips

LedControl mydisplay = LedControl(4, 2, 3, 1);

unsigned int count = 0;

unsigned int count_one = 0;

unsigned int count_two = 0;

void setup()

{

  mydisplay.shutdown(0, false);  //
  turns on display
  mydisplay.setIntensity(0, 15); // 15 = brightest
  mydisplay.setDigit(0, 0, 9, false);
  mydisplay.setDigit(0, 1, 8, false);
  mydisplay.setDigit(0, 2, 7, false);
  mydisplay.setDigit(0, 3, 6, false);
  mydisplay.setDigit(0, 4, 5, true);
  mydisplay.setDigit(0, 5, 4, false);
  mydisplay.setDigit(0, 6, 3, false);
  mydisplay.setDigit(0, 7, 2, false);
}

void loop()
{
  count++;
  if ((count * 7) >= 9999)
  {
    count =
      0;
  }

  count_one =
    HexToBCD(count);

  count_two =
    HexToBCD(count * 7);
  mydisplay.setDigit(0, 0, ((count_two >> 12) & 0x0F), false);
  mydisplay.setDigit(0, 1, ((count_two >> 8) & 0x0F), false);
  mydisplay.setDigit(0, 2, ((count_two >> 4) & 0x0F), false);
  mydisplay.setDigit(0, 3, ((count_two >> 0) & 0x0F), false);
  mydisplay.setDigit(0, 4, ((count_one >> 12) & 0x0F), false);
  mydisplay.setDigit(0, 5, ((count_one >> 8) & 0x0F), false);
  mydisplay.setDigit(0, 6, ((count_one >> 4) & 0x0F), false);
  mydisplay.setDigit(0, 7, (count_one & 0x0F), false);
  delay(500);

}

unsigned int HexToBCD(unsigned int number)

{

  unsigned char i = 0;
  unsigned int k = 0;
  while (number)
  {
    k = ( k ) | ((number % 10) << i * 4);
    number = number / 10;
    i++;
  }
  return (k);

}
