/*
  Test Watchdog-1

  Testing the task related watchdogs in ESP32 mikrocontroller. 
	
  Some useful info to ESP32 watchdogs: 

  The ESP32 Arduino framework is build around FreeRTOS and therefore provides multiple types of watchdogs. The implementation on the 
  ESP32 is such that each core operates independently with an IDLE task (task scheduler) running in the background. These IDLE tasks 
  run with low priority and must not get starved of execution time. If this happens, there assigned watchdog (if activated) will trigger
  and resets the chip. By default the CPU0 IDLE task watchdog is enabled, the CPU1 IDLE Task watchdog is disabled. Their state can be
  changed with enableCore0WDT()/disableCore0WDT() resp. enableCore1WDT()/disableCore1WDT(). Their default timeout is about 5 secs. 

  All tasks running with higher priority always run first while the IDLE tasks will have to wait. Thus all higher priority tasks must be 
  short enough to avoid triggering the watchdog or insert pauses in sufficient intervals by calling vTaskDelay(), delay(), some blocking 
  I/O functions, etc. Be aware: yield() will only allow higher priority tasks to run, it won't help in this case. 
  If your task has to run without pause then it must have priority "tskIDLE_PRIORITY" to share execution time with the IDLE task.

  All user tasks can have their own watchdog assigned. Calling esp_task_wdt_add() subscribes a task to the task watchdog timer (TWDT).
  Calling delay() will not reset a tasks watchdog if the task is subscribed. Function esp_task_wdt_reset() must be used instead.
  By default the task watchdog timer (TWDT) is enabled, there is no need to call esp_task_wdt_init() in setup(). 

  The loop task is always pinned to CPU1 and the loop task watchdog is disabled by default. Calling enableLoopWDT() will activate it. 
  The watchdog timout would be ~2 seconds. 

  If the CPU1 IDLE task watchdog has been enabled with enableCore1WDT() then all (!) tasks running on CPU1 must insert pauses as 
  explained above, even the loop() task.
  	
  More infos about ESP32 watchdogs: 
  "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/wdts.html?highlight=watchdog#l".

  Last updated 2020-04-06, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <esp_task_wdt.h>

uint32_t loopCount = 0;

void myTask(void *args) {
  Serial.printf("New Task on Core: %d\n", xPortGetCoreID());
  Serial.printf(" esp_task_wdt_add: %s\n", esp_task_wdt_add(NULL) == ESP_OK ? "Ok. Task subscribed to task watchdog timer." : "Error");
  Serial.printf(" esp_task_wdt_status: %s\n", esp_task_wdt_status(NULL) == ESP_OK ? "Ok" : "Error");
  Serial.println(" task starts looping");

  while (true) {
      // if commented out, the chip will reboot after ~5s --> see file "Serial_Output_watchdog_triggers.log"
      esp_task_wdt_reset();
      // delay() does not reset the task watchdog, it will only release the core for other tasks
      // if commented out, all other task on CPU1 (inkl. setup & loop) won't get execution time anymore --> see file "Serial_Output_loop_starved.log"
      delay(1);  
  }
  // it never gets here
  Serial.println(" task stops looping.");
  Serial.printf(" esp_task_wdt_delete: %s\n\n", esp_task_wdt_delete(NULL) == ESP_OK ? "Ok" : "Error");
  Serial.flush();
  delay(50);
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  //Serial.printf("esp_task_wdt_init: %d\n", esp_task_wdt_init(TIMEOUT, false));
  if (pdPASS != xTaskCreatePinnedToCore(&myTask,                        // task function
                                        "myTask",                       // name of task
                                        configMINIMAL_STACK_SIZE * 10,  // stack size
                                        NULL,                           // function parameters (not used)
                                        2 | portPRIVILEGE_BIT,          // higher task priority
                                        NULL,                           // task handle (not used)
                                        APP_CPU_NUM)) {                 // selects core 1 (PRO_CPU_NUM = 0, APP_CPU_NUM = 1)
    Serial.println("Error creating new task"); 
  }
  Serial.printf("Setup finished on core: %d\n", xPortGetCoreID());
}

void loop() {
  delay(1000);
  Serial.printf("Loop on core: %d, loop count: %d\r\n", xPortGetCoreID(), ++loopCount);
}