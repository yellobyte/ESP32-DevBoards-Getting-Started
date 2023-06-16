/*
  Ping Example

  This example sends an ICMP pings every 500 milliseconds, sends the human-readable
  result over the serial port.

  Requires Adafruit Ethernet 2 library for W5500
  
  Circuit:
  Ethernet shield attached to hardware SPI
  Ethernet CS pin to pin 8 and reset pin to pin 7

*/

#include <SPI.h>
#include <Ethernet2.h>
#include <ICMPPing.h>

#define ETH_CS_PIN 8
#define ETH_RST_PIN 7

const byte mac[] = { 0x36, 0xF2, 0xCC, 0xF2, 0xE5, 0xC0 };
IPAddress pingAddr(192, 168, 0, 102); // ip address to ping

SOCKET pingSocket = 0;

char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

void setup()
{
  // start Ethernet
  Serial.begin(115200);

  initEthernet();
  Serial.println("init success");

}

void loop()
{
  ICMPEchoReply echoReply = ping(pingAddr, 4);
  if (echoReply.status == SUCCESS)
  {
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply.data.seq,
            echoReply.addr[0],
            echoReply.addr[1],
            echoReply.addr[2],
            echoReply.addr[3],
            REQ_DATASIZE,
            millis() - echoReply.data.time,
            echoReply.ttl);
  }
  else
  {
    sprintf(buffer, "Echo request failed; %d", echoReply.status);
  }
  Serial.println(buffer);
  delay(500);
}


bool initEthernet() {

  Serial.println(F("Obtaining Local IP..."));
  bool DHCPsuccess = false;

  while (!DHCPsuccess) {
    digitalWrite(ETH_RST_PIN, LOW);
    pinMode(ETH_RST_PIN, OUTPUT);
    delay(100);
    pinMode(ETH_RST_PIN, INPUT);
    digitalWrite(ETH_RST_PIN, HIGH);
    delay(200);

    Ethernet.init(ETH_CS_PIN);

    if (Ethernet.begin(mac)) {
      Serial.print(F("DHCP IP: "));
      Serial.println(Ethernet.localIP());

      DHCPsuccess = true;
    } else {
      //timed out 60 secs.
      Serial.println(F("Timeout."));
      Serial.println(F("Check Ethernet cable."));
      Serial.println(F("Retring DHCP..."));

    }

  }




}
