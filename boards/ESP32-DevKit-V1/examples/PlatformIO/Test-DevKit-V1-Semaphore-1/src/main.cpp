/*
  Test Semaphore-1

  Semaphores are useful in many situations, especially for thread synchronisation and control.
  This example demonstrates the usage of a binary semaphore (also called mutual-exclusion semaphore, mutex or
  boolean semaphore) for triggering a waiting threat.
  
  The first call to xSemaphoreTake() in loop() is successful, the next calls will make the loop pause because 
  a binary semaphore can only be taken once. Only when the ISR in the background periodically releases the semaphore
  the loop can obtain it again and carry on.
  
  More infos about semaphores & their usage: "https://www.freertos.org/a00113.html".

  Last updated 2024-11-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

#define TIMEOUT_MS 3000    // 3000ms

hw_timer_t        *timer;
SemaphoreHandle_t  syncSemaphore;

void IRAM_ATTR onTimer() {
  xSemaphoreGiveFromISR(syncSemaphore, NULL);       // release semaphore
}

void setup() {
  Serial.begin(115200);
  syncSemaphore = xSemaphoreCreateBinary();

  // Arduino ESP32 API 2.x   
  timer = timerBegin(0, 80, true);                  // use 1st timer (0), set prescaler to 80, count upwards
  timerAttachInterrupt(timer, &onTimer, false);     // on timer alarm call onTimer, generated on level (false)
  timerAlarmWrite(timer, TIMEOUT_MS * 1000, true);  // set timeout, reload timer when expired
  timerAlarmEnable(timer);

  // Arduino ESP32 API 3.x                                              
  //timer = timerBegin(1000000);                      // set timer clock to 1uSec (1MHz)  
  //timerAttachInterrupt(timer, &onTimer);            // on timer alarm call function onTimer 
  //timerAlarm(timer, TIMEOUT_MS * 1000, true, 0);    // set timeout, reload timer when expired

  Serial.println();
  Serial.println("Semaphore created, timer started. setup() has finished.");
}

void loop() {
  static int count = 0;

  xSemaphoreTake(syncSemaphore, portMAX_DELAY);     // wait to obtain semaphore when it becomes available
  Serial.print("Semaphore was released. Loop count = ");
  Serial.println(++count);
  //
  // ...
  //
}