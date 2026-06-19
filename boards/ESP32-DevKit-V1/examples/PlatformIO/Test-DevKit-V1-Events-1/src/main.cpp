/*
  ESP32-Events

  Espressifs ESP32 event loop library allows components to declare events to which other
  components can register handlers – code which will execute when those events occur.
  This allows a component to attach desired behavior to changes in the state of
  another component, e.g. a program can react to another task, Wifi/BT or Ethernet events etc.

  We use the default event loop & our event handler catches user & system events.
  User events get created by our background task & system events by the WiFi subsystem.

  Last updated 2026-06-19, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <esp_event.h>
#include <WiFi.h>

// ESP_EVENT_DECLARE_BASE(MYTASK_EVENT)   <--- to be put into *.h files if needed

ESP_EVENT_DEFINE_BASE(MYTASK_EVENT);    // definition of user event base
enum {                                  // definition of various user events
  MYEVENT1 = 0,
  MYEVENT2,
  MYEVENT3
};

// Event handler which handles all events (user & system)
void myEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  if (event_base == MYTASK_EVENT) {
    switch (event_id) {
      case MYEVENT1:
        Serial.println("MyTask: Event 1");
        // do something specific at my task, event 1
        break;
      case MYEVENT2:
        Serial.println("MyTask: Event 2");
        // do something specific at my task, event 2
        break;
      case MYEVENT3:
        Serial.println("MyTask: Event 3");
        // do something specific at my task, event 3
        break;
      default:
        break;
    }
  }
  else if (event_base == WIFI_EVENT) {
    switch (event_id) {
      case WIFI_EVENT_AP_START:
        Serial.println("WiFi: WIFI_EVENT_AP_START");
        // do something specific at this event
        break;
      case WIFI_EVENT_AP_STOP:
        Serial.println("WiFi: WIFI_EVENT_AP_STOP");
        // do something specific at this event
        break;
      case WIFI_EVENT_AP_STACONNECTED:
        Serial.println("WiFi: WIFI_EVENT_AP_STACONNECTED");
        // do something specific at this event
        break;
      case WIFI_EVENT_AP_STADISCONNECTED:
        Serial.println("WiFi: WIFI_EVENT_AP_STADISCONNECTED");
        // do something specific at this event
        break;
      default:
        // Serial.print("Wifi: Event ");
        // Serial.println(event_id);
        break;
    }
  }
}

void myEventTask(void *args)
{
  while (1) {
    // create and post user events belonging to event base MYTASK_EVENT
    int32_t randomEvent = (int32_t)random(3);  // 0...2
    esp_event_post(MYTASK_EVENT, randomEvent, NULL, 0, 0);
    // random delay between 0.5s to 5s
    delay(random(500, 5000));
  }
  vTaskDelete(NULL);           // will never get here
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-Event example running...");

  // create a default event loop
  esp_event_loop_create_default();

  // subscribe to user events from MYTASK_EVENT base
  esp_event_handler_register(MYTASK_EVENT, ESP_EVENT_ANY_ID, myEventHandler, NULL);

  // subscribe to system events from e.g. WIFI_EVENT base
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, myEventHandler, NULL);

  delay(1000);

  // create background task which generates user events
  xTaskCreate(myEventTask, "myEventTask", 2048, NULL, 1, NULL);

  WiFi.softAP("ESP32-Soft-AP", "12345678");  // this will generate some WiFi events and
                                             // connecting/disconnecting a device to this Soft-AP even more
}

void loop() {
  ;
}
