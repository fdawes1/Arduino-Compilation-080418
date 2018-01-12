
#include "Charliplexing.h"
#include "Myfont.h"
#include "Arduino.h"

/* -----------------------------------------------------------------  */

int leng = 4; //provides the length of the char array
unsigned char test[5];

/* -----------------------------------------------------------------  */
/** MAIN program Setup
*/
void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);
  Serial.println("Serial conection started, waiting for instructions...");

  LedSign::Init();

  for (int i = 0; i < 20 ; i++)
  { //get the length of the text
    if (test[i] == 0)
    {
      leng = i;
    }
  }
}
  /* -----------------------------------------------------------------  */
  /** MAIN program Loop
  */

  void loop()                     // run over and over again
  {
    while (Serial.available() > 0)
    {
      test[10] = Serial.read();
      Serial.print(test[10]);
      Myfont::Banner(leng, test);

    }
  }
