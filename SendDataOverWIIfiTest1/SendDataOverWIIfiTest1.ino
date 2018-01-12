#include "uartWIFI.h"
#include <SPI.h>
#include <SoftwareSerial.h>


#define SSID       "123ulujddsfya654xcvgd10" // SSID name for WiFi connection
#define PASSWORD   "12365410" // Password




//Variable for WiFi
extern int chlID;  //client id(0-4)
String s;
const int led = 12;
char buf[100];
String Sbuf;


String CurrentS;

// ------------------- ## Set up ## -------------------------------------------
void setup() 

{
  //Serial.begin(9600);
  wifi.begin();

// -------------------- Start connecting WiFi --------------------------
  
  bool b = wifi.Initialize(STA, SSID, PASSWORD);
  delay(8000);  //Give a time for WiFi shield to connecting.
  wifi.confMux(1);
  delay(200);
  
  if(wifi.confServer(1,8888))
  { 
    // When WiFi connected, LED will display 2 sec.
    digitalWrite(led,1);
    delay(2000);
    digitalWrite(led,0);
    
    s = wifi.showIP();
    s.remove(0,14);
    s.remove(11,40);
    
    delay(1000);


    pinMode(led,OUTPUT);
    digitalWrite(led,0);

   
    
   }
   else 
   {
    }

}

// ---------------------- Main looping program -----------------------

void loop() 

{

// -------------- WiFi : Get order from client -----------------
  int iLen = wifi.ReceiveMessage(buf);
  if(iLen > 0)
  {
    //For control from browser
    Sbuf = String(buf);
    Sbuf.remove(8);
    
    if (Sbuf=="GET /ON ")
    {
        unsigned long takttime = millis();
        digitalWrite(led,1);
        wifi.Send(chlID,"\n\nUnder reading sensor data...");
        takttime = millis()-takttime;
        
        wifi.Send(chlID, "\nRH% = ");
        wifi.Send(chlID, hs);
        wifi.Send(chlID, " %");
        wifi.Send(chlID, "\nT = ");
        wifi.Send(chlID, ht);
        wifi.Send(chlID, " deg.C");
        wifi.Send(chlID, "\nCurrent (Is) = ");
        wifi.Send(chlID, CurrentS);
        wifi.Send(chlID, " mA");
        wifi.Send(chlID, "\n\n Sensor Conversion time  ");
        wifi.Send(chlID, String(takttime));
        wifi.Send(chlID, " ms");
      }
      
      else if (Sbuf=="GET /OFF")
      {
        digitalWrite(led,0);
        wifi.Send(chlID,"\n\nStopped reading sensor data");
        wifi.Send(chlID, "\n Thank you sir :) ");      
      }
      wifi.Send(chlID,"\n- End session -");

    }
}

// ---------------------- Sub function ------------------------------





