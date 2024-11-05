/*
  Test Watchdog-3

  Function loop() stops feeding the active loop task watchdog timer after a while. The watchdog gets triggered eventually.
  
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

extern bool loopTaskWDTEnabled;
extern TaskHandle_t loopTaskHandle;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 

  // call not needed unless you want to change watchdog settings 
  //esp_task_wdt_init(3, true);                      // task watchdog timeout 3 secs, execute panic handler at timeout

  enableLoopWDT();                                   // subscribes loop task to task watchdog timer (TWDT), timeout is ~5s
  if (loopTaskWDTEnabled) {
    Serial.print("loop task added to WDT, ");
  }
  else {
    Serial.print("failed to add loop task to WDT, ");
  }
  Serial.printf("loopTaskWDTEnabled: %d, loopTaskHandle: %d\n", loopTaskWDTEnabled, loopTaskHandle);            
}

void loop() {
  // put your main code here, to run repeatedly:
  static int loopCount = 1;
  static long lastTime = millis();

  // since loop() would not finish in time (<5s) it must feed the loop watchdog regularly
  while (millis() - lastTime < 10000) {              // 10s
    delay(1000);
    // stop feeding watchdog after a few loops
    if (loopCount <= 3) {
      feedLoopWDT();
    }  
  }
  lastTime = millis();  

  Serial.printf("Loop runs on core: %d, loop count: %d\n", xPortGetCoreID(), loopCount);
  loopCount++;
}