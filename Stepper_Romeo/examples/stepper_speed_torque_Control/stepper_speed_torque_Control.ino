#include <Stepper_Romeo.h>
Stepper_Romeo myStepper(200,D7,D6,D4,D5);

void setup()
{
  
}

void loop()
{
  //init        
  //set speed
  myStepper.setSpeed(200);
  //set the torque of the stepper
  myStepper.setTorque(170);
  //REV 1000 steps
  myStepper.step(1000);
  //delay 1 second
  delay(1000);
  //FWD 1000 steps
  myStepper.step(-1000);
  //delay 1 second
  delay(1000);
  //stop the stepper without torque
  myStepper.stop();
  //delay 1 second
  delay(1000);          
}

