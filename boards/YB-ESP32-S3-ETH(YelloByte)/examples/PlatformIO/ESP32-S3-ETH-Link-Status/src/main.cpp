/*
  Link-Status

  The sketch checks the current link status of the W5500 ethernet port integrated on YB-ESP32-S3-ETH board. 
  The status LED should be "ON" when an ethernet cable is properly connected between board and a switch.
	Getting an IP address via DHCP is not tested.

  Last updated 2023-06-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Ethernet.h>

#define GPIO_W5500_CS    14                     // onboard W5500 CS chip select pin connected to GPIO14
#define GPIO_STATUS_LED  47                     // onboard status LED connected to GPIO47

void setup() {
	pinMode(GPIO_STATUS_LED, OUTPUT);
  digitalWrite(GPIO_STATUS_LED, LOW);           // status LED off  
	
  Serial.begin(115200);
  // Port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini).
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  while (!Serial);     	
#endif	

  delay(1000);	
  Ethernet.init(GPIO_W5500_CS);
}

void loop() {
  EthernetLinkStatus status = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (status) {
    case Unknown:
      Serial.println("Unknown");
      digitalWrite(GPIO_STATUS_LED, LOW);       // status LED off  
      break;
    case LinkON:
      Serial.println("ON");
      digitalWrite(GPIO_STATUS_LED, HIGH);      // status LED on  
      break;
    case LinkOFF:
      Serial.println("OFF");
      digitalWrite(GPIO_STATUS_LED, LOW);       // status LED off  
      break;
  }
  delay(1000);
}