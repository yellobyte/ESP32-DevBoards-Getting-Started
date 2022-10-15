//
//	Testing the task related watchdogs in ESP32
//
/*
	The implementation on the ESP32 is such that each core operates independently with a task scheduler running (IDLE task)
  - The Loop Watchdog is disabled by default and if needed has to be enabled with enableLoopWDT().
  - The loop() task is always pinned to CPU1.
  - By default the CPU0 IDLE Task (task switcher) watchdog is enabled: no need to call enableCore0WDT()
  - By default the CPU1 IDLE Task (task switcher) watchdog is disabled. If enableCore1WDT() is called in
    setup() then all tasks running on CPU1 must call delay(), even the loop() task !!!
  - If the IDLEx task is not allowed to run periodically then the IDLE (task switcher) watchdog on CPUx will be triggered
    in case the IDLE task switcher watchdog for CPUx is enabled.
  - By default the CPU0/CPU1 Task watchdogs are enabled: no need to call esp_task_wdt_init() in setup()
  - The (IDLE) task watchdogs default timeout is about 5 secs
  - Yield() will only allow higher priority tasks to run, but the IDLE watchdogs run in the idle task (lower priority) 
    so it won't run with just a yield() -> call delay() instead.
  - Delay() will allow all other tasks to run, including the IDLEx Tasks (task switcher) and resets the IDLE task WDTs
  - Delay() will not reset task watchdogs if the tasks are subscribed: use esp_task_wdt_reset() for it
*/		

#include <Arduino.h>
#include <esp_task_wdt.h>

#define TIMEOUT 3
//TaskHandle_t myTaskHandle = NULL;
bool taskRunning = false;

void setup() {
    Serial.begin(115200);
    Serial.printf("esp_task_wdt_init: %d\n", esp_task_wdt_init(TIMEOUT, false));
    //enableLoopWDT();      // Enables watchdog only for loop() task !!
    Serial.printf("Setup finished on Core: %d\n", xPortGetCoreID());
    //disableCore0WDT();  // Explanation see below !
}

void doOnCore1(void *args) {
    Serial.printf("New Task on Core: %d\n", xPortGetCoreID());
    Serial.printf("esp_task_wdt_add: %d\n", esp_task_wdt_add(NULL));
    Serial.printf("esp_task_wdt_status: %d\n", esp_task_wdt_status(NULL));

    long increment = 5000000;
    while (increment > 0) {
        increment--;
        //delay(1);    // <--- HERE, Why is this required for ESP32 to proper reset the WD timer?
                       // Answer: The IDLE-task on CPU 0 is complaining about not getting any time to run !
                       //         The IDLE-task watchdog can be disabled with disableCore0WDT().
        //esp_task_wdt_reset();
    }

    Serial.printf("esp_task_wdt_delete: %d\n\n", esp_task_wdt_delete(NULL));
    Serial.flush();
    delay(50);
    taskRunning = false;
    vTaskDelete(NULL);
}

void loop() {
    delay(4000);  // max value of 2000 if enableLoopWDT() was called !
    Serial.printf("Loop on Core: %d\n", xPortGetCoreID());
    if (taskRunning == false && pdPASS != xTaskCreatePinnedToCore(&doOnCore1, "doOnCore1", configMINIMAL_STACK_SIZE * 10, 
                                                                  NULL, 2 | portPRIVILEGE_BIT, NULL, PRO_CPU_NUM)) {
      Serial.println("Error creating new task"); 
      taskRunning = false;
    }
    else {
      taskRunning = true;
    }
    Serial.printf("Free System Memory %d, Loop Stack Minimum %d\r\n", ESP.getFreeHeap(), uxTaskGetStackHighWaterMark(NULL));
}