

#include "Charliplexing.h"
#include "Myfont.h"
#include "Arduino.h"

String inData;
int leng=0 ; //provides the length of the char array

unsigned char* TEXT[];

void setup() {
  Serial.begin(9600);
  Serial.println("Serial conection started, waiting for instructions...");
  
  LedSign::Init();
  
}

void loop() {


  while (Serial.available() > 0)
  {
    char received = Serial.read();
    TEXT[] = (unsigned char*)received;
    inData += received;
    
    for (int i = 0; ; i++)
  { //get the length of the text
    if (inData[i] == 0)
    {
      leng = i;
      break;
    }
  }

   Myfont::Banner(leng, TEXT);

    // Process message when new line character is received
    if (received == '\n')
    {
      Serial.print("Arduino received : ");
      Serial.print(inData);

      // You can put some if and else here to process the message juste like that:

     // inData = ""; // Clear received buffer
    }


   
  }

}
