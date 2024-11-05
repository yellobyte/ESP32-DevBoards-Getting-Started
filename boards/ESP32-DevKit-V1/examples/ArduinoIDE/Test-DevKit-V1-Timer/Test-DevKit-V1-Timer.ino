/*
  Test Timer

  We set a timer and attach an interrupt routine. The routine is called at timer alarm and
  toggles the onboard LED. The timer gets reloaded at each alarm.
  
  More infos about semaphores & their usage: 
    "https://docs.espressif.com/projects/arduino-esp32/en/latest/api/timer.html".

  Last updated 2024-11-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

#define GPIO_LED   2       // onboard status LED at GPIO2
#define TIMEOUT_MS 2000    // timer alarm after 2000ms

hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  digitalWrite(GPIO_LED, !digitalRead(GPIO_LED));     // toggle LED
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Running example \"Test Timer\"");	
  pinMode(GPIO_LED, OUTPUT);  

  // Arduino ESP32 API 2.x       
  //timer = timerBegin(0, 80, true);                  // use 1st timer (0), set prescaler to 80, count upwards = 1uSec    
  //timerAttachInterrupt(timer, &onTimer, true);      // call onTimer on timer alarm, generated on edge (true)  
  //timerAlarmWrite(timer, TIMEOUT_MS * 1000, true);  // set timeout, reload timer when expired
  //timerAlarmEnable(timer);                          // enable timer

  // Arduino ESP32 API 3.0                                              
  timer = timerBegin(1000000);                      // set timer clock to 1uSec (1MHz)  
  timerAttachInterrupt(timer, &onTimer);            // call function onTimer on timer alarm
  timerAlarm(timer, TIMEOUT_MS * 1000, true, 0);    // set timeout, reload timer when expired
}

void loop() {
  delay(1);
}
