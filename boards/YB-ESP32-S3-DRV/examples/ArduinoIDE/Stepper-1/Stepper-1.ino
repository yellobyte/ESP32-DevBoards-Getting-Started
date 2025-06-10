/*
  Stepper-1

  Controls a single stepper motor of common type 28BYJ-48 attached to connector X.
  Please add library "Stepper" to your project or ArduinoIDE.
  
  Last updated 2025-05-21, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Stepper.h>

#define STEPS_PER_REVOLUTION 2048
Stepper stepperX(STEPS_PER_REVOLUTION, X1, X3, X2, X4);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);    // status LED off
  
  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available
  while (!Serial);
#endif  
  delay(2000);
  Serial.println();
  Serial.println("running example \"Stepper-1\":");

  stepperX.setSpeed(10);             // set the speed at 10 rpm
}

void loop()
{
  // half a revolution in one direction
  digitalWrite(LED_BUILTIN, HIGH);   // status LED On
  Serial.println("180° clockwise");
  stepperX.step(STEPS_PER_REVOLUTION / 2);
  delay(1000);

  // a quarter revolution in the opposite direction
  digitalWrite(LED_BUILTIN, LOW);    // status LED Off
  Serial.println("90° counterclockwise");
  stepperX.step(-(STEPS_PER_REVOLUTION / 4));
  delay(1000);    
}



