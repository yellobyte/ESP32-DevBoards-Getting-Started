/*
  Test Debug

  Debugging a program running on YB-ESP32-S3-DRV dev board via ESP32-S3's builtin JTAG adapter.

  Last updated 2025-05-21, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(BUILTIN_LED, OUTPUT);  
  digitalWrite(BUILTIN_LED, LOW);
  Serial.println();
  Serial.println("--> Sketch started. \nDebugging a simple program running on YB-ESP32-S3-DRV dev module:"); 
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
  digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
  Serial.printf("Loop counter: %d\n", ++counter);
}
