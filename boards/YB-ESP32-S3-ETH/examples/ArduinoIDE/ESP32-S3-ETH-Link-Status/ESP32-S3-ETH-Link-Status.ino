/*
  Link-Status

  The sketch checks the current link status of the W5500 ethernet port integrated on YB-ESP32-S3-ETH board. 
  The status LED should be "ON" when an ethernet cable is properly connected between board and a switch.
  Getting an IP address via DHCP is not tested.

  Last updated 2025-02-13, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Ethernet.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);           // status LED off  
  
  Serial.begin(115200);
  // Port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini).
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  while (!Serial);      
#endif  

  delay(1000);  
  Ethernet.init(W5500_SS);
}

void loop() {
  EthernetLinkStatus status = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (status) {
    case Unknown:
      Serial.println("Unknown");
      digitalWrite(LED_BUILTIN, LOW);       // status LED off  
      break;
    case LinkON:
      Serial.println("ON");
      digitalWrite(LED_BUILTIN, HIGH);      // status LED on  
      break;
    case LinkOFF:
      Serial.println("OFF");
      digitalWrite(LED_BUILTIN, LOW);       // status LED off  
      break;
  }
  delay(1000);
}
