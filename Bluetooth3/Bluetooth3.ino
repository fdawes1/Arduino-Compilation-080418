#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
Serial.begin(9600);
Serial.println("Goodnight moon!");
//mySerial.begin(115200);
mySerial.begin(9600);


delay(1000);
mySerial.print("AT");
delay(1000);
mySerial.print("AT+VERSION");
delay(1000);
mySerial.print("AT+PIN1342"); // Set pin to 1342
delay(1000);
mySerial.print("AT+NAMEFresaBT"); // Set the name to FresaBT
delay(1000);
//mySerial.print("AT+BAUD8"); // Set baudrate to 115200
mySerial.print("AT+BAUD4"); // Set baudrate to 9600
delay(1000);
}

void loop() // run over and over
{
if (mySerial.available())
Serial.write(mySerial.read());
if (Serial.available())
mySerial.write(Serial.read());
}
