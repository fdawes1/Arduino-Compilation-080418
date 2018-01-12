
#include <Wire.h>

char weekDay[][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
}

  void loop () {

    // Make a string for assembling the data to log:
    String dataString = “”;

    // RTC Time Function
    DateTime now = rtc.now();

    // Read the water pushbutton input pin
    buttonStateWater = digitalRead(buttonWater);

    // compare the buttonState to its previous state
    if (buttonStateWater != lastButtonStateWater) {
      if (buttonStateWater == HIGH) {
        Serial.println(“Button one is pressed”);

        // Create the file for writing the data
        dataFile = SD.open(“datalog.txt”, FILE_WRITE);

        // Assemble Strings to log data
        String theyear = String(now.year(), DEC);
        String mon = String(now.month(), DEC);
        String theday = String(now.day(), DEC);
        String thehour = String(now.hour(), DEC);
        String themin = String(now.minute(), DEC);
        //Put all the time and date strings into one String
        dataString = String(“water, ” + theyear + “ / ” + mon + “ / ” + theday + “, ” + thehour + “: ” + themin);

        // Open the datafile and write the dataString to the microSD card
        if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
          Serial.println(dataString);
        }

        // if the file isn’t open, register an error:
        else {
          Serial.println(“error opening datalog.txt”);

        }
      }
    }

    // save the current state as the last state,
    //for next time through the loop
    lastButtonStateWater = buttonStateWater;

  }
