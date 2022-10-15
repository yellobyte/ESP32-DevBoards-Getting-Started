#include <Arduino.h>

#define TIMEOUT_MS 3000    // 5s
hw_timer_t * timer;

SemaphoreHandle_t syncSemaphore;

void IRAM_ATTR onTimer() {
  xSemaphoreGiveFromISR(syncSemaphore, NULL);
}

void setup() {

  Serial.begin(115200);
  syncSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(0, 80, true);                  // Use 1st timer (0), prescaler 80, counting upwards (true)
  timerAttachInterrupt(timer, &onTimer, true);      // Call onTimer on timer alarm, generated on edge (true)
  timerAlarmWrite(timer, TIMEOUT_MS * 1000, true);  // Set Timeout: 3s in our case, reload
  timerAlarmEnable(timer);
}

void loop() {
  static int count = 0;
  xSemaphoreTake(syncSemaphore, portMAX_DELAY);     // portMAX_DELAY makes sure we wait indefinitely
  Serial.print("Semaphore was released ");
  Serial.println(++count);
}