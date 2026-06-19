/*
  ESP32-Events-2

  Espressifs ESP32 event loop library allows components to declare events to which other
  components can register handlers – code which will execute when those events occur.
  This allows a component to attach desired behavior to changes in the state of
  another component, e.g. a program can react to another task, BT/Wifi or Ethernet events etc.

  Contrary to example "ESP32-Events" here we use our own event loop instead of the
  default system one.

  Last updated 2026-06-19, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <esp_event.h>

// ESP_EVENT_DECLARE_BASE(TASK1_EVENT)   <--- to be put into *.h files if needed
// ESP_EVENT_DECLARE_BASE(TASK2_EVENT)

ESP_EVENT_DEFINE_BASE(TASK1_EVENT);    // definition of event bases
ESP_EVENT_DEFINE_BASE(TASK2_EVENT);

enum {                                 // definition of various events, can be
  EVENT1 = 0,                          // different per event base
  EVENT2,
  EVENT3
};

esp_event_loop_handle_t loopHandle;    // hondle to our own event loop

// Event handler which handles events of all my background tasks
void myEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  if (event_base == TASK1_EVENT) {
    switch (event_id) {
      case EVENT1:
        Serial.println("Task 1: Event 1");
        // do something specific at task1, event 1
        break;
      case EVENT2:
        Serial.println("Task 1: Event 2");
        // do something specific at task1, event 2
        break;
      case EVENT3:
        Serial.println("Task 1: Event 3");
        // do something specific at task1, event 3
        break;
      default:
        break;
    }
  }
  else if (event_base == TASK2_EVENT) {
    switch (event_id) {
      case EVENT1:
        Serial.println("Task 2: Event 1");
        // do something specific at task2, event 1
        break;
      case EVENT2:
        Serial.println("Task 2: Event 2");
        // do something specific at task2, event 2
        break;
      case EVENT3:
        Serial.println("Task 2: Event 3");
        // do something specific at task2, event 3
        break;
      default:
        break;
    }
  }
}

void eventTask1(void *args)
{
  while (1) {
    // create and post an event belonging to event base TASK1_EVENT
    int32_t randomEvent = (int32_t)random(3);  // 0...2
    esp_event_post_to(loopHandle, TASK1_EVENT, randomEvent, NULL, 0, 0);
    // random delay between 0.5s to 5s
    delay(random(500, 5000));
  }
  vTaskDelete(NULL);           // will never get here
}

void eventTask2(void *args)
{
  while (1) {
    // create and post an event belonging to event base TASK2_EVENT
    int32_t randomEvent = (int32_t)random(3);  // 0...2
    esp_event_post_to(loopHandle, TASK2_EVENT, randomEvent, NULL, 0, 0);
    // random delay between 0.5s to 5s
    delay(random(500, 5000));
  }
  vTaskDelete(NULL);           // will never get here
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-Events-2 example running...");

  // we create our own event loop
  esp_event_loop_args_t loopArgs = {
    .queue_size = 5,
    .task_name = "loopTask",  // a task will get created
    .task_priority = uxTaskPriorityGet(NULL),
    .task_stack_size = 3072,
    .task_core_id = tskNO_AFFINITY
  };
  esp_event_loop_create(&loopArgs, &loopHandle);

  // subscribe to any events from TASK1_EVENT and TASK2_EVENT base
  esp_event_handler_register_with(loopHandle, TASK1_EVENT, ESP_EVENT_ANY_ID, myEventHandler, NULL);
  esp_event_handler_register_with(loopHandle, TASK2_EVENT, ESP_EVENT_ANY_ID, myEventHandler, NULL);

  // create two background tasks which generate events
  xTaskCreate(eventTask1, "eventTask1", 2048, NULL, 1, NULL);
  xTaskCreate(eventTask2, "eventTask2", 2048, NULL, 1, NULL);
}

void loop() {
  ;
}
