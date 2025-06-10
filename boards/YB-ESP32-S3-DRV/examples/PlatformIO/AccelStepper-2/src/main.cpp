/*
  AccelStepper-2

  Controls 2 stepper motors of common type 28BYJ-48 attached to connectors X and Y.
  The motor on connector X will rotate 180° only, the other one on Y a full 360°.
  
  Last updated 2025-05-21, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <AccelStepper.h>

unsigned long now, last = millis();
AccelStepper stepperX(AccelStepper::HALF4WIRE, X1, X3, X2, X4);
AccelStepper stepperY(AccelStepper::FULL4WIRE, Y1, Y3, Y2, Y4);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);          // status LED off

  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // only continue when serial port becomes available
  while (!Serial);
#endif  
  delay(2000);
  Serial.println();
  Serial.println("running example \"AccelStepper-2\"");

  stepperX.setMaxSpeed(1000.0);
  stepperX.setAcceleration(200.0);
  stepperX.setSpeed(200);
  stepperX.moveTo(1024);  // start clockwise

  stepperY.setMaxSpeed(1000.0);
  stepperY.setAcceleration(50.0);
  stepperY.setSpeed(200);
  stepperY.moveTo(-1024);  // start counterclockwise
}

void loop()
{
  // every 250ms the actual status gets printed 
  now = millis();
  if ((now - last) >= 250) {
    last = now;
    Serial.printf("X: pos=%d, dist=%d, speed=%d\n", 
                  stepperX.currentPosition(), stepperX.distanceToGo(), (int)stepperX.speed());
    Serial.printf("Y: pos=%d, dist=%d, speed=%d\n", 
                  stepperY.currentPosition(), stepperY.distanceToGo(), (int)stepperY.speed());
  }

  // change direction when motor reaches target position
  if (stepperX.distanceToGo() == 0) {
    Serial.println("X: dist=0, change of direction...");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    stepperX.moveTo(-stepperX.currentPosition());
  }
  if (stepperY.distanceToGo() == 0) {
    Serial.println("Y: dist=0, change of direction...");
    stepperY.moveTo(-stepperY.currentPosition());
  }

  // move each motor one step
  stepperX.run();
  stepperY.run();
}



