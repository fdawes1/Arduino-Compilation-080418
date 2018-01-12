
float input_voltage = 0.0;
float temp = 0.0;
float r1 = 90900.0;   // adjust r1 & r2 for used potential divider
float r2 = 10000.0;   // here p/d made up of a 90.9 kohm resistor and a 10 kohm resistor connected to the input.

// temperature of panel (RTD)
// humidity  (Humidity Sensor)
// Light Intensity ( LDR on panel )
// Attach bulb with brightness proportinal to panel voltage? or go directly from panel to bulb? 


void setup()
{
  Serial.begin(9600);

}
void loop()
{

  //Voltage reading conversion formula and display to serial monitor

  int analog_value = analogRead(A0);
  temp = (analog_value * 5.0) / 1024.0;         //change 5.0 to maximum output volatge of solar panel
  input_voltage = temp / (r2 / (r1 + r2));

  if (input_voltage < 0.1)
  {
    input_voltage = 0.0;
  }
  Serial.print("voltage = ");
  Serial.println(input_voltage);

  delay(300); //stability
}


