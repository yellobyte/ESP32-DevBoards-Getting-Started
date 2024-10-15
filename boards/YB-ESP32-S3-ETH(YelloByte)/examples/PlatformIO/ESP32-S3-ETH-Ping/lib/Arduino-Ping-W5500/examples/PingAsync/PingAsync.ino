/*
   PingAsync.cpp
   This example uses the asynchronous methods of ICMPPing to send a ping,
   do "other stuff" and check back for results periodically.


   See the basic "Ping" example, on which this is based, for simpler
   (but synchronous--meaning code will be frozen while awaiting responses)
   usage.

   Setup: Configure the various defines in the Configuration section, below,
   to select whether to use DHCP or a static IP and to choose the remote host
   to ping.


  Requires Adafruit Ethernet 2 library for W5500

  Circuit:
  Ethernet shield attached to hardware SPI
  Ethernet CS pin to pin 8 and reset pin to pin 7




*/
#include <SPI.h>
#include <Ethernet2.h>
#include <ICMPPing.h>


/* ************ Configuration ************** */


// NOTE: Use *COMMAS* (,) between IP values, as we're
// using these to init the constructors



#define ETH_CS_PIN 8
#define ETH_RST_PIN 7

const byte mac[] = { 0x36, 0xF2, 0xCC, 0xF2, 0xE5, 0xC0 };
IPAddress pingAddr(192, 168, 0, 102); // ip address to ping

// PING_REQUEST_TIMEOUT_MS -- timeout in ms.  between 1 and 65000 or so
// save values: 1000 to 5000, say.
#define PING_REQUEST_TIMEOUT_MS     2500


#ifndef ICMPPING_ASYNCH_ENABLE
#error "Asynchronous functions only available if ICMPPING_ASYNCH_ENABLE is defined -- see ICMPPing.h"
#endif

SOCKET pingSocket = 0;

char buffer [256];
ICMPPing ping(pingSocket, 1);

void dieWithMessage(const char * msg)
{

  for (;;)
  {
    Serial.println(msg);
    delay(500);
  }
}

void setup()
{
  // start Ethernet
  Serial.begin(115200);

  initEthernet();
  Serial.println("init success");


  // increase the default time-out, if needed, assuming a bad
  // connection or whatever.
  ICMPPing::setTimeout(PING_REQUEST_TIMEOUT_MS);

}


// lastPingSucceeded -- just a flag so we don't drown in
// output when things are going wrong...
bool lastPingSucceeded = false;

// someCriticalStuffThatCantWait is just a toy function
// to represent the *reason* you want to use asynchronous
// calls rather than wait around for pings to come home.
// Here, we just print out some chars...
void someCriticalStuffThatCantWait()
{
  for (int i = 0; i < 10; i++)
  {
    if (lastPingSucceeded) {
      Serial.print('.');
    }
  }
  Serial.print('_');
}

void loop()
{

  lastPingSucceeded = false;
  ICMPEchoReply echoResult;  // we'll get the status here
  Serial.println("Starting async ping.");

  // asynchStart will return false if we couldn't
  // even send a ping, though you could also check the
  // echoResult.status with should ==
  if (! ping.asyncStart(pingAddr, 3, echoResult))
  {
    Serial.print("Couldn't even send our ping request?? Status: ");
    Serial.println((int)echoResult.status);
    delay(500);
    return;

  }

  // ok the ping started out well...
  Serial.println("Ping sent ");
  while (! ping.asyncComplete(echoResult))
  {
    // we have critical stuff we wish to handle
    // while we wait for ping to come back
    someCriticalStuffThatCantWait();
  }

  // async is done!  let's see how it worked out...
  if (echoResult.status != SUCCESS)
  {
    // failure... but whyyyy?
    sprintf(buffer, "Echo request failed; %d", echoResult.status);
  } else {
    // huzzah
    lastPingSucceeded = true;
    sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoResult.data.seq,
            echoResult.addr[0],
            echoResult.addr[1],
            echoResult.addr[2],
            echoResult.addr[3],
            REQ_DATASIZE,
            millis() - echoResult.data.time,
            echoResult.ttl);
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
