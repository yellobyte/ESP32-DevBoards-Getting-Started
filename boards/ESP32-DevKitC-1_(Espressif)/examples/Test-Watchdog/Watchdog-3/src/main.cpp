/*
  Testing ESP32 Loop Watchdog & IDLE Task Watchdogs & Task Watchdogs
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

  Serial Messages when enableLoopWDT() is called in setup() and LOOP_TIMEOUT > 4500
  E (12221) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
  E (12221) task_wdt:  - loopTask (CPU 1)
  E (12221) task_wdt: Tasks currently running:
  E (12221) task_wdt: CPU 0: Task1
  E (12221) task_wdt: CPU 1: Task2

  Serial Messages when IDLE0 task (task switcher) is not allowed to run (delay() was not called periodically)
  E (10218) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
  E (10218) task_wdt:  - IDLE0 (CPU 0)
  E (10218) task_wdt: Tasks currently running:
  E (10218) task_wdt: CPU 0: Task1
  E (10218) task_wdt: CPU 1: Task2

  Serial Messages when CPU0 task watchdog is triggered (esp_task_wdt_reset() was not called periodically)
  E (12218) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
  E (12218) task_wdt:  - Task1 (CPU 0)
  E (12218) task_wdt: Tasks currently running:
  E (12218) task_wdt: CPU 0: Task1
  E (12218) task_wdt: CPU 1: loopTask
*/
#include <Arduino.h>
#include <esp_task_wdt.h>

#define LOOP_TIMEOUT  10000
#define TASK1_TIMEOUT 1000
#define TASK2_TIMEOUT 1000
//#define USE_TASK_WDTS

TaskHandle_t Task1;
TaskHandle_t Task2;

void Task1code( void * pvParameters ){
  static long lastTime = millis();
  for(;;){
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    while (millis() - lastTime < TASK1_TIMEOUT) {
      delay(1);  // does not reset task watchdog TWDT (when subscribed) but allows IDLE task to run (important!)
    }
    lastTime = millis();    
#ifdef USE_TASK_WDTS
    esp_task_wdt_reset();   // It does only reset task watchdog if the task was subscribed to the TWDT. It does not
                            // allow the idle task to run !
#endif                            
    //yield();                // Not necessary if delay() is called. It does not allow the IDLE task to run !
  } 
}

void Task2code( void * pvParameters ){
  static long lastTime = millis();
  for(;;){
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());
    while (millis() - lastTime < TASK2_TIMEOUT) {
      //delay(1);             // does not reset task watchdog TWDT (when subscribed) but allows IDLE task to run (important!)
    }
    lastTime = millis();
#ifdef USE_TASK_WDTS
    esp_task_wdt_reset();   // It does only reset task watchdog if the task was subscribed to the TWDT. It does not
                            // allow the idle task to run !
#endif                            
    //yield();              // Not necessary if delay() is called. It does not allow the IDLE task to run !
  }
}

void setup() {
#ifdef USE_TASK_WDTS
  int err;
#endif
  // put your setup code here, to run once:
  Serial.begin(115200); 
#ifdef USE_TASK_WDTS
  //esp_task_wdt_init(10, true);   // Task WDT set to 10 secs & reboot system
#endif  
  //disableCore0WDT();    // Disables only CPU0 IDLE task (task switcher) watchdog. Another task subscribed to TWDT must
                        // still call esp_task_wdt_reset() for not triggering the task watchdog !
  //disableCore1WDT();    // Causes error message: "disableCore1WDT(): Failed to remove Core1 IDLE task from WDT"
                          // Reason: CPU1 IDLE task (task switcher) watchdog is not enabled by default !!
  //enableCore1WDT();
  //enableLoopWDT();      // Enables watchdog only for loop() task !!
                        // If called, LOOP_TIMEOUT must not be bigger than 4800 (circa)
  //disableLoopWDT();   // Not needed because the loop watchdog is disabled by default
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
#ifdef USE_TASK_WDTS
  if ( (err = esp_task_wdt_add(Task1)) != ESP_OK )  // Subscribe task to TWDT
    Serial.printf("esp_task_wdt_add(Task1) returned error %d\n", err);
#endif    
  delay(100); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
#ifdef USE_TASK_WDTS
  if ( (err = esp_task_wdt_add(Task2)) != ESP_OK )  // Subscribe task to TWDT
    Serial.printf("esp_task_wdt_add(Task2) returned error %d\n", err);
#endif    
  delay(100); 
}

void loop() {
  static long lastTime = millis();
  // put your main code here, to run repeatedly:
  while (millis() - lastTime < LOOP_TIMEOUT) {}
  lastTime = millis();
  //delay(1);
  Serial.printf("Loop on core%d\n",xPortGetCoreID());
}