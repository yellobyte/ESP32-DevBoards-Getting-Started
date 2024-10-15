/*
  Test-Webserver

  The sketch lets you switch on/off the status LED via Webserver listening on RJ45 Ethernet port 
  integrated on the YB-ESP32-S3-ETH board. 

  Last updated 2023-06-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Ethernet.h>

#define GPIO_STATUS_LED  47                        // onboard status LED connected to GPIO47

// Ethernet settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#define GPIO_W5500_CS  14                          // onboard W5500 CS chip select pin connected to GPIO14

EthernetServer server(80);
bool ledOn = false;

void setup() {                       
  pinMode(GPIO_STATUS_LED, OUTPUT);
  digitalWrite(GPIO_STATUS_LED, LOW);              // LED off
  
  Serial.begin(115200);
  // port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini)  
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  while (!Serial);
  delay(100);
#endif	

  Serial.println();
  Serial.print("Initializing Ethernet...");
  Ethernet.init(GPIO_W5500_CS);

  while (true) {
    if (Ethernet.begin(mac)) {
      Serial.println("DHCP ok.");
      digitalWrite(GPIO_STATUS_LED, HIGH);         // LED on
      ledOn = (bool)digitalRead(GPIO_STATUS_LED);
      break;
    }
    Serial.println("DHCP error, couldn't get IP !");
    Serial.println("Please connect Ethernet cable between board and switch and make sure DHCP service is available in your LAN.");
    delay(2000);
  }

  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());

  // start webserver
  server.begin();
  Serial.print("Waiting for client...");
}

void loop(){
  EthernetClient client = server.available();      // listen for http clients

  if (client) {
    String received;
    bool reply = true;

    Serial.println();
    Serial.println("Client connected");
    while (client.connected()) {                   // loop while the client's connected
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        received += c;
        if (c == '\n') {
          if (reply) {
            // last incoming line was empty, send response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/html; charset=\"utf-8\"");
            client.println("Connection: close");
            client.println();                      // empty line separates http header from following html content
            
            // evaluate command & set LED
            if (received.indexOf("GET /ledOn") >= 0) {
              digitalWrite(GPIO_STATUS_LED, HIGH);
              Serial.println("LED is now ON");
            } 
            else if (received.indexOf("GET /ledOff") >= 0) {
              digitalWrite(GPIO_STATUS_LED, LOW);
              Serial.println("LED is now OFF");
            }
            ledOn = (bool)digitalRead(GPIO_STATUS_LED);

            // send HTML header
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; text-align: center;}");
            client.println(".buttonGreen {background-color: #559955; border: none; color: white;  font-size: 30px; padding: 20px 30px;}");
            client.println(".buttonGray {background-color: #444444; border: none; color: white; font-size: 30px; padding: 20px 30px;}</style>");
            client.println("</head>");
            
            // send HTML body (On/Off button)
            client.println("<body><h2>ESP32-S3-ETH Switching GPIO47 LED On/Off</h2>");
            if (ledOn) {
              client.println("<p><a href=\"/ledOff\"><button class=\"buttonGreen\">Switch LED OFF</button></a></p>");
            } 
            else {
              client.println("<p><a href=\"/ledOn\"><button class=\"buttonGray\">Switch LED ON</button></a></p>");
            } 
            client.println("</body></html>");
            client.println("");                    // trailing empty line represents end of http response
            break;
          } 
          else {
            reply = true;                          // a following incoming empty line will trigger response
          }
        } 
        else if (c != '\r') {
          reply = false;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
    Serial.print("Waiting for client...");
  }
  delay(500);
  Serial.print(".");
}
