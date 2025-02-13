/*
  Test Debug

  Debugging a program running on a YB-ESP32-S3-ETH dev board via ESP32-S3's builtin JTAG adapter (on port 'USB').
  Serial output must be directed to port 'UART' for not interfering with JTAG port 'USB'.

  Last updated 2025-02-13, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <SPI.h>

int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  Serial.println("--> Sketch started: Debugging simple program running on YB-ESP32-S3-ETH dev module:"); 
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
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.printf("Loop counter: %d\n", ++counter);
}