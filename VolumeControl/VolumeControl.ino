#include <Keyboard.h>

volatile boolean TurnDetected;
volatile boolean up;

int pot;   

#define ledg = 12;
#define ledb = 11;

int virtualposition = 0;

void isr ()  {                    
 if (digitalRead(A0))
   up = digitalRead(ledg);
 else
   up = !digitalRead(ledg);
 TurnDetected = true;
}


void setup() {

 attachInterrupt (1,isr,FALLING);   // interrupt 0 is always connected to pin 2 on Arduino Micro
Serial.begin(9600);
//Keyboard.begin();
}

void loop() {  
if (TurnDetected)  {        // do this only if rotation was detected
   if (up){
  //   virtualPosition = virtualPosition - 4; 
//     Keyboard.press(KEY_LEFT_CTRL);
//     Keyboard.press(KEY_F8); 
//     delay(75);
//     Keyboard.releaseAll();
//     delay(75);
//     Keyboard.release(KEY_LEFT_CTRL);
   }
   else {
  //   virtualPosition = virtualPosition + 4;
//     Keyboard.press(KEY_LEFT_CTRL);
//     Keyboard.press(KEY_F9);
//     delay(75);
//     Keyboard.releaseAll();
//     delay(75);
//     Keyboard.release(KEY_LEFT_CTRL);
   }
   TurnDetected = false;          // do NOT repeat IF loop until new rotation detected
 }
}
