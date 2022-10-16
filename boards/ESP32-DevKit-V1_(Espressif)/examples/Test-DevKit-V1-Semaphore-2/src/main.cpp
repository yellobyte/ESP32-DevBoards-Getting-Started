/*
  Test Semaphore-2

  Semaphores are useful in many situations, especially for thread synchronisation and control.
  This example demonstrates the usage of a counting semaphore.
  
  The setup function should only carry on when there are as many units in the semaphore as there are tasks/threads. 
  Since there will be the tasks incrementing the semaphore, we should guarantee that the setup function will
  only finish after all the tasks have finished.
  
  More infos about counting semaphores & their usage: "https://www.freertos.org/Real-time-embedded-RTOS-Counting-Semaphores.html".

  Last updated 2020-07-21, ThJ <yellobyte@bluewin.ch>
*/
#include <Arduino.h>

#define NUMBER_OF_TASKS 5

int numberOfTasks = NUMBER_OF_TASKS;
SemaphoreHandle_t barrierSemaphore = xSemaphoreCreateCounting(numberOfTasks, 0);
 
void simpleTask (void *parameter) {
  Serial.printf("Task %d started.\n", (int)parameter + 1);
  delay(1000 * (int)random(3,15));            // random delay from 3 to 15 seconds
  Serial.printf("Task %d finished.\n", (int)parameter + 1);
  xSemaphoreGive(barrierSemaphore);           // counting semaphore incremented by 1
  vTaskDelete(NULL);                          // finishes the task/thread
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.print("Launching tasks 1 to ");
  Serial.println(numberOfTasks);

  int i;
  for(i = 0; i < numberOfTasks; i++ ) {
    xTaskCreatePinnedToCore(simpleTask,      // task function
                            "simpleTask",    // name of task
                            10000,           // stack size in words
                            (void *)i,       // task input parameter
                            0,               // priority of task
                            NULL,            // task handle (not used)
                            1);              // on which core the task should run
  }    
 
  for(i = 0; i < numberOfTasks; i++) {
    xSemaphoreTake(barrierSemaphore, portMAX_DELAY);    // portMAX_DELAY makes sure we wait indefinitely if necessary
  }
 
  Serial.println("All tasks have finished. Setup will finish.");
}

void loop() {
  // put your main code here, to run repeatedly:
  //
}