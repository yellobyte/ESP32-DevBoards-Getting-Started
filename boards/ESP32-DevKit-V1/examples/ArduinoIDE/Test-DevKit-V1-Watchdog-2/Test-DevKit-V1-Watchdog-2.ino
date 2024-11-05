/*
  Test Watchdog-2

  Using an IRQ routine to implement a watchdog instead of framework provided watchdogs. 
	
  Last updated 2024-11-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

const int wdtTimeout = 5000;            // watchdog timeout in ms
hw_timer_t *timer = NULL;
long loopTime = millis();

void IRAM_ATTR resetModule() {
  esp_restart();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("running setup");

  // Arduino ESP32 API 2.x       
  //timer = timerBegin(0, 80, true);                    // use 1st timer (0), set prescaler to 80 (1uSec), count upwards (true)
  //timerAttachInterrupt(timer, &resetModule, false);   // call resetModule() on timer alarm, generated on LEVEL (false)
  //timerAlarmWrite(timer, wdtTimeout * 1000, false);   // set watchdog timeout (5000000us = 5s in our case), no reload
  //timerAlarmEnable(timer);                            // enable timer interrupt

  // Arduino ESP32 API 3.0                                              
  timer = timerBegin(1000000);                        // setup timer for 1uSec (1MHz)  
  timerAttachInterrupt(timer, &resetModule);          // call function resetModule on timer alarm
  timerAlarm(timer, wdtTimeout * 1000, false, 0);     // set timeout, do not reload timer when expired
}

void loop() {
  long now = millis();
  Serial.print("running main loop, loop time (ms): ");  // should be well under 2000
  Serial.println(now - loopTime);
  loopTime = now;

  // reset timer (feed watchdog)
  // comment out to see the watchdog kicking in
  timerWrite(timer, 0);

  // simulate some work
  int rand = random(500, 1500);         
  delay(rand);
}