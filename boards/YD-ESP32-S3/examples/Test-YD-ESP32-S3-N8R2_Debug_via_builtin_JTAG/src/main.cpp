/*
  Test Debug

  Debugging a program running on a YD-ESP32-S3 dev board via ESP32-S3's builtin JTAG adapter.

  Last updated 2022-11-19, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

#define GPIO_USED 2
int counter = 0;

void setup() {
  pinMode(GPIO_USED, OUTPUT);  
  digitalWrite(GPIO_USED, LOW);
  delay(2000);
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println("--> Sketch started: Debugging program running on YD-ESP32-S3-N8R2 module:"); 
}

void loop() {
  char c;

  if (Serial.read(&c, 1) && (c == 'r' || c == 'R')) {
    // check for terminal input: reboot requested
    Serial.println("\nSystem will restart in 3 seconds.\n\n"); 
    delay(3000);
    ESP.restart();  
  }
  delay(1000);
  digitalWrite(GPIO_USED, !digitalRead(GPIO_USED));
  Serial.printf("Loop counter: %d\n", ++counter);
}