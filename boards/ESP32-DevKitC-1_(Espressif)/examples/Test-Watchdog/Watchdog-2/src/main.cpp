//
//	Using an IRQ routine to implement a watchdog
//

#include <Arduino.h>

//const int button = 0;         //gpio to use to trigger delay
const int wdtTimeout = 5000;  //time in ms to trigger the watchdog
hw_timer_t *timer = NULL;

void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");     // printf/Serial.print shouldn't be used in a IRQ routine !!
  esp_restart();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("running setup");

  //pinMode(button, INPUT_PULLUP);                    //init control pin
  timer = timerBegin(0, 80, true);                  // Use 1st timer (0), prescaler 80, counting upwards (true)
  timerAttachInterrupt(timer, &resetModule, true);  // Call resetModule on timer alarm, generated on edge (true)
  timerAlarmWrite(timer, wdtTimeout * 1000, false); //set time in us (5000000us = 5s in our case), no reload
  timerAlarmEnable(timer);                          //enable interrupt
}

void loop() {
  Serial.println("running main loop");

  //timerWrite(timer, 0);               //reset timer (feed watchdog)
  long loopTime = millis();
  //while button is pressed, delay up to 3 seconds to trigger the timer
  //while (!digitalRead(button)) {
  //  Serial.println("button pressed");
  //  delay(500);
  //}
  delay(1000); //simulate work
  loopTime = millis() - loopTime;
  
  Serial.print("loop time is = ");
  Serial.println(loopTime); //should be under 3000
}