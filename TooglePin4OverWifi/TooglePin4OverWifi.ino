//zoomkat 12-18-10
//routerbot code
//for use with IDE 0021
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html 
//address will look like http://192.168.1.102:84/ when submited
//for use with W5100 based ethernet shields

#include <SPI.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
const char *ssid = "Matin Router King";
const char *password = "Broodmother21";
ESP8266WebServer server(80);
Servo pcServo;
String readString; 

//////////////////////
void handleRoot() {
  server.send(200, "text/html", "<h1>Looking Good!</h1>"); 
pcServo.attach(4);
}


void setup(){

  pinMode(4, OUTPUT); //pin selected to control
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  
  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("servertest1"); // so I can keep track of what is loaded
}

void loop(){
  // Create a client connection
  server.handleClient();
  
} 

