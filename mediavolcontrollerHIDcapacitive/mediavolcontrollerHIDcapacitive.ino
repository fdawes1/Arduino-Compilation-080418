// Media Volume Controller V1.0
// By Iqbal Ibrahim 2015
// For Instructables



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

const int PinCLK=2;                   
const int PinDT=3;                    
int redVal;
int blueVal;
int greenVal;

long total1, total2, total3, total4, total5, total6;


int virtualPositionTemp = 0;

int state, stateDesktop, stateUser1, stateUser2, statePrvtrk, stateNxttrk, statePlay = HIGH;      // the current state of the output pin
int reading, readingDesktop, readingUser1, readingUser2, readingPrvtrk, readingNxttrk, readingPlay;           // the current reading from the input pin
int previous, previousDesktop, previousUser1, previousUser2, previousPrvtrk, previousNxttrk, previousPlay = LOW;    // the previous reading from the input pin
boolean muted = false;


long time, timeDesktop, timeUser1, timeUser2, timePrvtrk, timeNxttrk, timePlay = 0;         
long debounce = 200;  



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

void isr ()  {                    
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
                   
// Serial.begin(9600);
 Keyboard.begin();
}

void loop() { //Runs continuously
 static long virtualPosition=0;    // without STATIC it does not count correctly!!!
 
    long start = millis();
    total1 =  cs_4_2.capacitiveSensor(2);
    total2 =  cs_4_3.capacitiveSensor(2);
    total3 =  cs_4_5.capacitiveSensor(2);
    total4 =  cs_4_6.capacitiveSensor(2);
    total5 =  cs_4_7.capacitiveSensor(2);
    total6 =  cs_4_8.capacitiveSensor(2);

 
 


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

//readingDesktop = digitalRead(DESKTOPPIN); 
   
   if(total1 > 45 && previousDesktop < 45)
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
previousDesktop = total1;


// --------------------- USER1 BUTTON ---------------------
// --------------------- OPEN PROJECTS  ---------------------

//readingUser1 = digitalRead(USER1PIN); 
   
   if(total2 > 45 && previousUser1 < 45)
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
previousUser1 = total2;


// --------------------- USER2 BUTTON ---------------------
// --------------------- RUN TSKMGR   ---------------------

//readingUser2 = digitalRead(USER2PIN); 
   
   if(total3 > 45 && previousUser2 < 45)
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
previousUser2 = total3;



// --------------------- PLAY BUTTON ---------------------

//readingPlay = digitalRead(PLAYPIN); 
   
   if(total4 > 45 && previousPlay < 45)
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
previousPlay = total4;



// --------------------- PRVTRK BUTTON ---------------------

//readingPrvtrk = digitalRead(PRVTRKPIN); 
   
   if(total5 > 45 && previousPrvtrk < 45)
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
previousPrvtrk = total5;


// --------------------- NXTTRK BUTTON ---------------------

//readingNxttrk = digitalRead(NXTTRKPIN); 
   
   if(total6 > 45 && previousNxttrk < 45)
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
previousNxttrk = total6;

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

//    Serial.print(millis() - start);        // check on performance in milliseconds
//    Serial.print("\t");                    // tab character for debug windown spacing
//
//    Serial.print(total1);                  // print sensor output 1
//    Serial.print("\t");
//    Serial.print(total2);                  // print sensor output 2
//    Serial.print("\t");
//    Serial.print(total3);                  // print sensor output 3
//    Serial.print("\t");
//    Serial.print(total4);                  // print sensor output 4
//    Serial.print("\t");
//    Serial.print(total5);                  // print sensor output 5
//    Serial.print("\t");
//    Serial.print(total6);                  // print sensor output 6
//    Serial.println("\t");

}
