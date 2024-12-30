/*
  Link-Status

  The sketch checks the current link status of the W5500 ethernet port integrated on YB-ESP32-S3-ETH board. 
  The status LED should be "ON" when an ethernet cable is properly connected between board and a switch.
	Getting an IP address via DHCP is not tested.

  Last updated 2024-12-29, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <EthernetESP32.h>

#define W5500_SS         14                     // W5500 CS chip select pin connected to GPIO14
#define W5500_RST        21                     // W5500 RST reset pin connected to GPIO21
#define GPIO_STATUS_LED  47                     // status LED connected to GPIO47

W5500Driver driver(W5500_SS);

void setup() {
	pinMode(GPIO_STATUS_LED, OUTPUT);
  digitalWrite(GPIO_STATUS_LED, LOW);           // status LED off  
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
  Ethernet.init(driver);
  Ethernet.begin(1000);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Error: W5500 not found.");
    while (true) delay(1);                      // do nothing
  }
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