/*
The setup function should only carry on when there are as many units in the semaphore as there are tasks. 
Since there will be the tasks incrementing  the semaphore, we should guarantee that the Arduino setup function 
will only finish after all the tasks finish.
*/

#include <Arduino.h>

int nTasks=5;
SemaphoreHandle_t barrierSemaphore = xSemaphoreCreateCounting(nTasks, 0);
 
void genericTask (void *parameter ) {
 
    String taskMessage = "Task number:";
    taskMessage = taskMessage + ((int)parameter);
    Serial.println(taskMessage);
    xSemaphoreGive(barrierSemaphore);   // Increment counting semaphore by 1
    vTaskDelete(NULL);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //delay(1000);
  Serial.print("\nStarting to launch tasks 1...");
  Serial.println(nTasks);

  int i;
  for(i= 0; i< nTasks; i++ ){
    xTaskCreatePinnedToCore(
                    genericTask,    /* Function to implement the task */
                    "genericTask",  /* Name of the task */
                    10000,          /* Stack size in words */
                    (void *)i,      /* Task input parameter */
                    0,              /* Priority of the task */
                    NULL,           /* Task handle. */
                    1);             /* Core where the task should run */
  }    
 
  for(i= 0; i< nTasks; i++) {
    xSemaphoreTake(barrierSemaphore, portMAX_DELAY);  // portMAX_DELAY makes sure we wait indefinitely if necessary
  }
 
  Serial.println("All Tasks launched and semaphore passed...");
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskSuspend(NULL);
}