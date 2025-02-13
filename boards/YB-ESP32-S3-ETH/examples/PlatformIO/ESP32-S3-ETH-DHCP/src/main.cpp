/*
  Test-DHCP

  The sketch gets an Ethernet IP address from DHCP server on RJ45 Ethernet port. 
  The LED will: blink very fast - with no Ethernet cable attached/no link to a switch
                blink normal    - with Ethernet cable attached to a switch and the link is up
                blink very slow - local IP has been obtained from DHPC service       

  Last updated 2025-02-13, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Ethernet.h>

// Ethernet settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
uint32_t tdelay = 100;                      // blink delay in ms

void blinkTask (void *parameter) {
  Serial.println("blinkTask has started.");
  while (true) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(*((uint32_t*)parameter));
  }
  // will never get here
  Serial.println("blinkTask has finished.");
  vTaskDelete(NULL);                        // finishes the task/thread
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);           // status LED off  
#ifdef W5500_RESET  
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, LOW);  
  delay(500);
  pinMode(W5500_RST, INPUT);
#endif

  Serial.begin(115200);
  // Port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini).  
#if ARDUINO_USB_CDC_ON_BOOT == 1  
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  while (!Serial);                                 
#endif    

  delay(1000);
  // start the backgound task responsible for letting the status LED blink
  xTaskCreatePinnedToCore(blinkTask,        // task function
                          "blinkTask",      // name of task
                          2000,             // stack size in words
                          (void *)&tdelay,  // task input parameter is a pointer to variable holding the delay time
                          0,                // priority of task
                          NULL,             // task handle (not used)
                          1);               // core the task should run on

  Serial.println();
  Serial.println("Please make sure Ethernet cable is connected between board and switch and DHCP service is available in your LAN.");
  Ethernet.init(W5500_SS);
}

void loop() {
  // only continue when Ethernet link is up
  while (Ethernet.linkStatus() != LinkON) { 
    Serial.println("Ethernet link is down.");
    delay(1000);
    tdelay = 100;                           // blink very fast
  };
  delay(1000);

  // Ethernet link is up
  Serial.println("Ethernet link is up.");
  
  // get local IP from DHCP service if not yet done
  if (!Ethernet.localIP()) {
    tdelay = 500;                           // blink slower
    // send IP request to DHCP service
    if (Ethernet.begin(mac)) {
      Serial.print("DHCP successful. Local IP: ");
      Serial.println(Ethernet.localIP());
      tdelay = 2000;                        // blink very slow
    }
    else {
      Serial.println("\nDHCP error !");
    }
  }  
  else {
    Serial.print("Local IP: ");
    Serial.println(Ethernet.localIP());
    tdelay = 2000;                          // blink very slow
  }
}
