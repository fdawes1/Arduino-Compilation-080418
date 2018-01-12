#include <Wire.h>
#include "Sodaq_DS3231.h"
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  rtc.begin(); // Initialize the rtc object
  lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}
void loop()
{
  DateTime now = rtc.now(); //get the current date-time

  lcd.setCursor(0, 0);
  lcd.print("Time:  ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.date(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  lcd.print(" ");
  //  lcd.print(weekDay[now.dayOfWeek()]);
  lcd.display();
  delay(1000);
  lcd.clear();

}



