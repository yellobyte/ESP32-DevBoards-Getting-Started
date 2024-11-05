/*
  Test Watchdog-4

  Testing the task related watchdogs in ESP32 mikrocontroller with a user thread running on both cores.
  Both threads/tasks have been added to the task watchdog. One task will stop feeding the watchdog after a while.
  
  Some useful info about ESP32 watchdogs: 

  The ESP32 Arduino framework is build around FreeRTOS and therefore provides multiple types of watchdogs. The implementation on the 
  ESP32 is such that each core operates independently with an IDLE task (task scheduler) running in the background. These IDLE tasks 
  run with low priority and must not get starved of execution time. If this happens, there assigned watchdog (if activated) will trigger
  and resets the chip. By default the CPU0 IDLE task watchdog is enabled, the CPU1 IDLE Task watchdog is disabled. Their state can be
  changed with enableCore0WDT()/disableCore0WDT() resp. enableCore1WDT()/disableCore1WDT(). Their default timeout is ~5 secs. 

  All tasks running with higher priority always run first while the IDLE tasks will have to wait. Thus all higher priority tasks must be 
  short enough to avoid triggering the watchdog or insert pauses in sufficient intervals by calling vTaskDelay(), delay(), some blocking 
  I/O functions, etc. Be aware: yield() will only allow higher priority tasks to run, it won't help in this case. 
  If your task has to run without pause then it must have priority "tskIDLE_PRIORITY" to share execution time with the IDLE task.

  All user tasks can have their own watchdog assigned. Calling esp_task_wdt_add() subscribes a task to the task watchdog timer (TWDT).
  Calling delay() will not reset a tasks watchdog if the task is subscribed. Function esp_task_wdt_reset() must be used instead.
  By default the task watchdog timer (TWDT) is enabled, there is no need to call esp_task_wdt_init() in setup(). 

  The loop task is always pinned to CPU1 and the loop task watchdog is disabled by default. Calling enableLoopWDT() will activate it. 
  The watchdog timout would be ~5 seconds. 

  If the CPU1 IDLE task watchdog has been enabled with enableCore1WDT() then all (!) tasks running on CPU1 must insert pauses as 
  explained above, even the loop() task.
    
  More infos about ESP32 watchdogs: 
  "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/wdts.html?highlight=watchdog#l".

  Last updated 2020-04-06, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <esp_task_wdt.h>

TaskHandle_t hTask1;
TaskHandle_t hTask2;
int err;

void myTask(void *pvParameters) {
  int count = 0, feedWatchdog = 1;
  int parameter = *(int *)pvParameters;

  Serial.printf("user task %s started on core %d, parameter: %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), parameter);
  for(;;){
    delay(1000);                             // does not reset task watchdog but allows IDLE task on CPUx to run
    if (parameter == 1 && count == 10) {
      Serial.printf("user task %s on core %d stops feeding task watchdog !\n", pcTaskGetTaskName(NULL), xPortGetCoreID());
      feedWatchdog = 0;
    }
    if (feedWatchdog) {
       esp_task_wdt_reset();                 // only resets task watchdog if subscribed
    }                  
    Serial.printf("user task %s still running on core %d, count: %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), count);
    count++;
  } 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 

  // call not needed unless you want to change watchdog settings 
  //esp_task_wdt_init(3, true);                        // task watchdog timeout 3 secs, execute panic handler at timeout

  int stopFeedingWatchdog = 0;
  // create a user task running on CPU0
  xTaskCreatePinnedToCore(
                    myTask,                            // task function
                    "myTask1",                         // name of task
                    10000,                             // stack depth of task
                    (void *)&stopFeedingWatchdog,      // task input parameter
                    1,                                 // task priority
                    &hTask1,                           // task handle to keep track of created task
                    0);                                // pin task to core 0               

  if ( (err = esp_task_wdt_add(hTask1)) != ESP_OK) {   // subscribe task to TWDT
    Serial.printf("esp_task_wdt_add(myTask1) returned error %d\n", err);
  }  

  delay(5000);
  stopFeedingWatchdog = 1; 
  // create a second user task running on CPU1
  xTaskCreatePinnedToCore(
                    myTask,                            // Task function
                    "myTask2",                         // name of task
                    10000,                             // stack depth of task
                    (void *)&stopFeedingWatchdog,      // task input parameter
                    1,                                 // task priority
                    &hTask2,                           // task handle to keep track of created task
                    1);                                // pin task to core 1    

  if ( (err = esp_task_wdt_add(hTask2)) != ESP_OK) {   // subscribe task to TWDT
    Serial.printf("esp_task_wdt_add(myTask2) returned error %d\n", err);
  }  
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int loopCount = 0;

  delay(1000);
  loopCount++;
  Serial.printf("Loop running on core: %d, loop count: %d\n", xPortGetCoreID(), loopCount);
}