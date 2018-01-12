#include <Wire.h>
#include "RTClib.h"
#include <avr/pgmspace.h>  
#include <Charliplexing.h> 
int digits[][15] = {
  {1,1,1,1,0,1,1,0,1,1,0,1,1,1,1}
  ,{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}
  ,{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1}
  ,{1,1,1,0,0,1,1,1,1,0,0,1,1,1,1}
  ,{1,0,1,1,0,1,1,1,1,0,0,1,0,0,1}
  ,{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1}
  ,{1,0,0,1,0,0,1,1,1,1,0,1,1,1,1}
  ,{1,1,1,0,0,1,0,0,1,0,0,1,0,0,1}
  ,{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1}
  ,{1,1,1,1,0,1,1,1,1,0,0,1,0,0,1}
};

RTC_DS1307 RTC;

void setup () {
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  LedSign::Init(DOUBLE_BUFFER);
}

void loop () {

  DateTime now = RTC.now();
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  int h = now.hour();
  if (h>12)
  {
    h=h-12; 
  }
  // first digit of hours
  if (h>9)
  {
    for (int y=2;y<7;y++)
    {
      LedSign::Set(1,y,1);
    } 
  }
  // second digit of hours
  for (int i=0;i<15;i++)
  {
    LedSign::Set((i % 3)+3,(i/3)+2,digits[h%10][i]);
  }
  // first digit of minutes
  for (int i=0;i<15;i++)
  {
    LedSign::Set((i % 3)+7,(i/3)+2,digits[now.minute()/10][i]);
  }
  //second digit of minutes
  for (int i=0;i<15;i++)
  {
    LedSign::Set((i % 3)+11,(i/3)+2,digits[now.minute()%10][i]);
  }
  LedSign::Flip();
  delay(1000);
}
