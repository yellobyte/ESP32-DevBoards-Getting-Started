/*
  Test Semaphore-1

  Semaphores are useful in many situations, especially for thread synchronisation and control.
	This example demonstrates the usage of a binary semaphore (also called mutual-exclusion semaphore, mutex, boolean semaphore) 
  for triggering a waiting threat.
	
	The first call to xSemaphoreTake() in loop() is successful, the next calls will make the loop pause because 
	a binary semaphore can only be taken once. Only when the ISR in the background periodically releases the semaphore
  the loop can obtain it again and carry on.
	
  More infos about semaphores & their usage: "https://www.freertos.org/a00113.html".

  Last updated 2020-07-21, ThJ <yellobyte@bluewin.ch>
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
  timer = timerBegin(0, 80, true);                  // use 1st timer (0), set prescaler to 80, count upwards
  timerAttachInterrupt(timer, &onTimer, true);      // call onTimer on timer alarm, generated on edge (true)
  timerAlarmWrite(timer, TIMEOUT_MS * 1000, true);  // set timeout, reload timer when expired
  timerAlarmEnable(timer);
  Serial.println();
  Serial.println("Semaphore created, timer started. setup() has finished.");
}

void loop() {
  static int count = 0;
  xSemaphoreTake(syncSemaphore, portMAX_DELAY);     // wait to obtain semaphore when it becomes available
  Serial.print("Semaphore was released. Loop count = ");
  Serial.println(++count);
	//
	// do some work...
	//
}