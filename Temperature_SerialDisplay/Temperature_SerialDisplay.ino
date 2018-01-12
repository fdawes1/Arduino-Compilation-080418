
//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);
}

void loop()
{
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);

  // converting that reading to voltage, for 3.3v arduino use 3.3, 5.0 for 5v
   float voltage = reading * 5.0;
  voltage /= 1024.0;

  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");

  // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((voltage - 500mV) times 100)

  Serial.print(temperatureC); Serial.println(" degrees C");
  
  
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // print all the available characters
   lcd.setCursor(0,0); lcd.print(temperatureC,DEC); lcd.print("C");
   lcd.setCursor(0,1); lcd.print(voltage,DEC); lcd.print("Volts");
   
      // display each character to the LCD 
   
     
    
  
 delay(5000);

}

