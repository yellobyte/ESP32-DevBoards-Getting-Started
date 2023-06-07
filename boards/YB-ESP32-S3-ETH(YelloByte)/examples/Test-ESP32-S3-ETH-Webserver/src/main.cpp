/*
  Test-DHCP

  The sketch tests getting an IP address via DHCP on the W5500 ethernet port 
  integrated on YB-ESP32-S3-ETH dev board. 

  Last updated 2023-06-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Ethernet.h>

#define GPIO_STATUS_LED  47                 // onboard status LED connected to GPIO47

// Ethernet settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#define GPIO_W5500_CS  14                   // onboard W5500 CS pin is connected to GPIO14

void setup() {
  Serial.begin(115200);

  // port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini)  
  while (!Serial);                                 
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  //
  delay(1000);
	
	Serial.println();
  Serial.println("Please make sure Ethernet cable is connected between board and switch and DHCP service is available in your LAN.");
  Serial.print("\nInitializing Ethernet...");

  pinMode(GPIO_STATUS_LED, OUTPUT);
  digitalWrite(GPIO_STATUS_LED, LOW);       // status LED off  

  Ethernet.init(GPIO_W5500_CS);

  if (Ethernet.begin(mac)) {
    Serial.println("\nDHCP ok.");
    digitalWrite(GPIO_STATUS_LED, HIGH);    // LED off
  }
  else {
    Serial.println("\nDHCP error !");
    while (true) {
      // no point to continue
    }
  }

  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
	//
}