/*
  AccelStepper-1

  Controls a stepper motor of common type 28BYJ-48 attached to connector X.
  Please add library "AccelStepper" to your project or ArduinoIDE.
  
  Last updated 2025-05-21, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <AccelStepper.h>

AccelStepper stepperX(AccelStepper::FULL4WIRE, X1, X3, X2, X4);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);          // status LED off

  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available
  while (!Serial);
#endif  
  delay(2000);
  Serial.println();
  Serial.println("running example \"AccelStepper-1\"");

  stepperX.setMaxSpeed(1000.0);
  stepperX.setAcceleration(100.0);
  stepperX.setSpeed(100);
  stepperX.moveTo(1024);
}

void loop()
{
  // change direction when motor reaches target position
  if (stepperX.distanceToGo() == 0) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println("dist=0, change of direction...");
    stepperX.moveTo(-stepperX.currentPosition());
  }

  // move the motor one step
  stepperX.run();
}



