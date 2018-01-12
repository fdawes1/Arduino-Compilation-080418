#include <Keyboard.h>

#define DESKTOPPIN	8
#define USER1PIN	15
#define USER2PIN	10
#define PRVTRKPIN	16
#define PLAYPIN		14
#define NXTTRKPIN	7
#define MUTEPIN		4
#define GREEN 5
#define BLUE 6
#define RED 9
#define delayTime 10

// ------- Variables -------

volatile boolean TurnDetected;
volatile boolean up;

const int PinCLK=2;                   // Used for generating interrupts using CLK signal
const int PinDT=3;                    // Used for reading DT signal
int redVal;
int blueVal;
int greenVal;

int virtualPositionTemp = 0;

int state, stateDesktop, stateUser1, stateUser2, statePrvtrk, stateNxttrk, statePlay = HIGH;      // the current state of the output pin
int reading, readingDesktop, readingUser1, readingUser2, readingPrvtrk, readingNxttrk, readingPlay;           // the current reading from the input pin
int previous, previousDesktop, previousUser1, previousUser2, previousPrvtrk, previousNxttrk, previousPlay = LOW;    // the previous reading from the input pin
boolean muted = false;

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time, timeDesktop, timeUser1, timeUser2, timePrvtrk, timeNxttrk, timePlay = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void redLED(){
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, HIGH); 
}

void greenLED(){
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, LOW) ;
}

void blueLED(){
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, LOW); 
}

void offLEDS(){
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, LOW);
}

void isr ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
 if (digitalRead(PinCLK))
   up = digitalRead(PinDT);
 else
   up = !digitalRead(PinDT);
 TurnDetected = true;
}


void setup() { //This runs only once

pinMode(DESKTOPPIN, INPUT_PULLUP);   // Show/Hide Desktop Button
pinMode(USER1PIN, INPUT_PULLUP);   // User 1
pinMode(USER2PIN, INPUT_PULLUP);   // User 2
pinMode(PRVTRKPIN, INPUT_PULLUP);   // Previous track 
pinMode(PLAYPIN, INPUT_PULLUP);   // Play/Pause
pinMode(NXTTRKPIN, INPUT_PULLUP);   // Next track
pinMode(MUTEPIN, INPUT_PULLUP);   // Mute/unmute button

 pinMode(PinCLK,INPUT);
 pinMode(PinDT,INPUT);  
 pinMode(MUTEPIN,INPUT);

  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, HIGH);
  delay(1000); 
  offLEDS();
 attachInterrupt (1,isr,FALLING);   // interrupt 0 is always connected to pin 2 on Arduino Micro
 Keyboard.begin();
}

void loop() { //Runs continuously
 static long virtualPosition=0;    // without STATIC it does not count correctly!!!
 


// --------------------- MUTE BUTTON ---------------------
   reading = digitalRead(MUTEPIN); 
   
   if(reading == HIGH && previous == LOW && millis() - time > debounce)
    {
      if (state == HIGH){
       state = LOW;
       virtualPosition =0;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F10);
       delay(200);
       Keyboard.releaseAll();  
      }
    else{
      state = HIGH;
      virtualPosition =0;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F10);
       delay(200);
       Keyboard.releaseAll();  
    }
    time = millis(); 
    }
    
   previous = reading;


// --------------------- ROTARY ENCODER  ---------------------
// --------------------- VOLUME UP/DOWN  ---------------------
 
 if (TurnDetected)  {		    // do this only if rotation was detected
   if (up){
     virtualPosition = virtualPosition - 4; 
     Keyboard.press(KEY_LEFT_CTRL);
     Keyboard.press(KEY_F8);
     delay(75);
     Keyboard.releaseAll();
     delay(75);
     Keyboard.release(KEY_LEFT_CTRL);
   }
   else {
     virtualPosition = virtualPosition + 4;
     Keyboard.press(KEY_LEFT_CTRL);
     Keyboard.press(KEY_F9);
     delay(75);
     Keyboard.releaseAll();
     delay(75);
     Keyboard.release(KEY_LEFT_CTRL);
   }
   TurnDetected = false;          // do NOT repeat IF loop until new rotation detected
 }
 

// --------------------- DESKTOP BUTTON ---------------------

readingDesktop = digitalRead(DESKTOPPIN); 
   
   if(readingDesktop == HIGH && previousDesktop == LOW && millis() - timeDesktop > debounce)
    {
      if (stateDesktop == HIGH){
       stateDesktop = LOW;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F11);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    else{
       stateDesktop = HIGH;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F11);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    timeDesktop = millis();     
   }
previousDesktop = readingDesktop;


// --------------------- USER1 BUTTON ---------------------
// --------------------- OPEN PROJECTS  ---------------------

readingUser1 = digitalRead(USER1PIN); 
   
   if(readingUser1 == HIGH && previousUser1 == LOW && millis() - timeUser1 > debounce)
    {
      if (stateUser1 == HIGH){
       stateUser1 = LOW;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F12);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    else{
       stateUser1 = HIGH;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F12);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    timeUser1 = millis();     
   }
previousUser1 = readingUser1;


// --------------------- USER2 BUTTON ---------------------
// --------------------- RUN TSKMGR   ---------------------

readingUser2 = digitalRead(USER2PIN); 
   
   if(readingUser2 == HIGH && previousUser2 == LOW && millis() - timeUser2 > debounce)
    {
      if (stateUser2 == HIGH){
       stateUser1 = LOW;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F4);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    else{
       stateUser2 = HIGH;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F4);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    timeUser2 = millis();     
   }
previousUser2 = readingUser2;



// --------------------- PLAY BUTTON ---------------------

readingPlay = digitalRead(PLAYPIN); 
   
   if(readingPlay == HIGH && previousPlay == LOW && millis() - timePlay > debounce)
    {
      if (statePlay == HIGH){
       statePlay = LOW;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F6);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    else{
       statePlay = HIGH;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F6);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    timePlay = millis();     
   }
previousPlay = readingPlay;



// --------------------- PRVTRK BUTTON ---------------------

readingPrvtrk = digitalRead(PRVTRKPIN); 
   
   if(readingPrvtrk == HIGH && previousPrvtrk == LOW && millis() - timePrvtrk > debounce)
    {
      if (statePrvtrk == HIGH){
       statePrvtrk = LOW;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F5);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    else{
       statePrvtrk = HIGH;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F5);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    timePrvtrk = millis();     
   }
previousPrvtrk = readingPrvtrk;


// --------------------- NXTTRK BUTTON ---------------------

readingNxttrk = digitalRead(NXTTRKPIN); 
   
   if(readingNxttrk == HIGH && previousNxttrk == LOW && millis() - timeNxttrk > debounce)
    {
      if (stateNxttrk == HIGH){
       stateNxttrk = LOW;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F7);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    else{
       stateNxttrk = HIGH;
       Keyboard.press(KEY_LEFT_CTRL);
       Keyboard.press(KEY_F7);
       delay(100);
       Keyboard.releaseAll();
       delay(100);
       Keyboard.release(KEY_LEFT_CTRL);
    }
    timeNxttrk = millis();     
   }
previousNxttrk = readingNxttrk;

if (virtualPosition<=100 && virtualPosition >=1){
virtualPositionTemp = map((int)virtualPosition, 0, 100, 0, 255);
analogWrite(BLUE, virtualPositionTemp);
digitalWrite(RED,LOW);
}
else if(virtualPosition<=0){
digitalWrite(BLUE,LOW);
digitalWrite(RED,HIGH);
}

if (virtualPosition <0){
  virtualPosition = 0;
}

if (virtualPosition > 100){
  virtualPosition = 100;
}

}
