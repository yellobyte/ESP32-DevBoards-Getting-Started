/*
  Test WiFi-Ethernet-Bridge

  The sketch starts a server listening for a client on telnet port 23 on both Ethernet and WiFi
  and simply forwards all incoming data to the opposite side. Only one client per side allowed.

  Last updated 2023-06-08, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Ethernet.h>
#include <WiFi.h>

#define GPIO_STATUS_LED  47                        // onboard status LED connected to GPIO47
#define SERVER_PORT      23                        // Telnet port

// Ethernet settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#define GPIO_W5500_CS  14                          // W5500 CS chip select pin connected to GPIO14
#define GPIO_W5500_RST 21                          // W5500 RST reset pin connected to GPIO21
EthernetServer ethServer(SERVER_PORT);
EthernetClient ethClient, ethClientTmp;

// Wifi settings
const char ssid[] = "MySSID";
const char pass[] = "MyPassword"; 
WiFiServer wifiServer(SERVER_PORT);
WiFiClient wifiClient;

bool ethClientOn = false, wifiClientOn = false;

void setup() {                       
#ifdef W5500_HARD_RESET
  // W5500 hard reset, hardly ever needed
  pinMode(GPIO_W5500_RST, OUTPUT);
  digitalWrite(GPIO_W5500_RST, LOW); 
  delay(1);
  digitalWrite(GPIO_W5500_RST, HIGH);
  pinMode(GPIO_W5500_RST, INPUT);
  delay(10);
#endif  

  pinMode(GPIO_STATUS_LED, OUTPUT);
  digitalWrite(GPIO_STATUS_LED, LOW);              // LED off
	
  Serial.begin(115200);
  // port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini)  
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port 'USB' becomes available
  while (!Serial);     	
#endif	

  delay(1000);
  Serial.println();
  Serial.println("Please connect Ethernet cable between board and switch and make sure DHCP service is available in your LAN.");

  // connect to local network via Ethernet
  Serial.print("Initializing Ethernet...");
  Ethernet.init(GPIO_W5500_CS);
  while (true) {
    if (Ethernet.begin(mac)) {
      Serial.println("DHCP on Ethernet ok.");
      break;
    }
    Serial.println("Ethernet DHCP error, couldn't get IP !");
    delay(2000);
  }
  Serial.print("Local Ethernet IP: ");
  Serial.println(Ethernet.localIP());

  // connecting to local network via WiFi
  Serial.printf("Connecting to WiFi network \"%s\"\n", ssid);
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.printf("\nConnected successfully to \"%s\". IP address: ", ssid);
  Serial.println(WiFi.localIP());

  // connected successfully to both Ethernet and WiFi
  digitalWrite(GPIO_STATUS_LED, HIGH);         // LED on

  // start Ethernet & Wifi server
  ethServer.begin();
  Serial.printf("Server started. Waiting for Telnet client on IP %s, Port %d...\n", Ethernet.localIP().toString().c_str(), SERVER_PORT);
  wifiServer.begin();
  //wifiServer.setNoDelay(true);
  Serial.printf("Server started. Waiting for Telnet client on IP %s, Port %d...\n", WiFi.localIP().toString().c_str(), SERVER_PORT);
}

void loop() {
  if (ethClientTmp = ethServer.accept()) {
    // Ethernet client connection request received
    if (!ethClientOn) {
      ethClient = ethClientTmp;
      Serial.print("New Ethernet client: ");
      Serial.println(ethClient.remoteIP());
      ethClientOn = true;
      if (wifiClientOn) {
        ethClient.println("Ok. Start typing !");
        wifiClient.println("Ok. Start typing !");
      }
      else {
        ethClient.println("Welcome. Still waiting for the other side...");
      }
    }
    else {
      // only one client at a time accepted so reject
      ethClientTmp.stop();
    }
  }

  if (wifiServer.hasClient()) {
    // WiFi client connection request received
    if (!wifiClient || !wifiClient.connected()) {
      if (wifiClient) wifiClient.stop();      // client is not connected anymore
      wifiClient = wifiServer.available();
      //if (!serverClients[i]) Serial.println("available broken");
      Serial.print("New WiFi client: ");
      Serial.println(wifiClient.remoteIP());
      wifiClientOn = true;
      if (ethClientOn) {
        wifiClient.println("Ok. Start typing !");
        ethClient.println("Ok. Start typing !");
      }
      else {
        wifiClient.println("Welcome. Still waiting for the other side...");
      }
    }
    else {
      // only one client at a time accepted so reject
      wifiServer.available().stop();
    }
  }

  if (ethClientOn) {
    if (ethClient && ethClient.connected()) {
      // get data from Ethernet  client and if connected forward it to the WiFi client
      while (ethClient.available()) {
        char c = ethClient.read();
        digitalWrite(GPIO_STATUS_LED, !digitalRead(GPIO_STATUS_LED));     // toggle LED
        Serial.print(c);
        if (wifiClientOn) {
          // pass data on to WiFi client
          wifiClient.write(c);
        }
      }
    }
    else {
      Serial.println("\nEthernet Client is gone");
      ethClient.stop();
      ethClientOn = false;
    }
  }  

  if (wifiClientOn) {
    if (wifiClient && wifiClient.connected()) {
      // get data from WiFi client and if connected forward it to the Ethernet client
      while (wifiClient.available()) {
        char c = wifiClient.read();
        digitalWrite(GPIO_STATUS_LED, !digitalRead(GPIO_STATUS_LED));     // toggle LED
        Serial.print(c);
        if (ethClientOn) {
          // pass data on to Ethernet client
          ethClient.write(c);
        }
      }
    }
    else {
      Serial.println("\nWiFi Client is gone");
      wifiClient.stop();
      wifiClientOn = false;
    }
  }
}
