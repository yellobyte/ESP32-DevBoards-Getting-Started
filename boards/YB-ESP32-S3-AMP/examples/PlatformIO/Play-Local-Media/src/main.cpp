/*
  Play-Local-Media

  Plays a media file from a local DLNA server over the 2 integrated MAX98357A I2S amplifiers (left+right audio channels).
  Please add library "ESP32-audioI2S" to your project or ArduinoIDE. More info about this versatile lib you find on
  its homepage "https://github.com/schreibfaul1/ESP32-audioI2S".

  Jumper bridge DAC_MUTE is open by default and both MAX98357A are active. Closing the jumper bridge will connect pin
  GPIO47 (onboard LED IO47) with the amps SD_MODE pin. Setting GPIO47 to LOW (LED off) will then shut down (mute) both
  amps and setting GPIO47 to HIGH (LED on) will activate them.

  Last updated 2025-04-06, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <WiFi.h>
#include "Audio.h"

// media files hosted on a local DLNA server, e.g.: 
#define MEDIA_FILE "http://192.168.1.11:9000/disk/DLNA-PNMP3-OP01-FLAGS01700000/O0$1$8I1867788.mp3"
//#define MEDIA_FILE "http://192.168.1.40:9790/*/H*c3*b6rb*c3*bccher/A01/A*20C*20M/1-01*20Titel*2001.mp3"

const char ssid[] = "MySSID";
const char pass[] = "MyPassword"; 

Audio audio;
 
void audio_info(const char *info) {     // file info
  Serial.print("info: "); 
  Serial.println(info);
}
void audio_id3data(const char *info) {  // id3 metadata
  Serial.print("id3: ");
  Serial.println(info);
}

void setup()
{
  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available (board revision 3.x), uncomment if needed
  //while (!Serial);
  //delay(3000);   	
#endif	  
  Serial.println();
  Serial.println("running example \"Play-Local-Media\":");

  // connecting to local WiFi network
  Serial.printf("connecting to WiFi network \"%s\"\n", ssid);
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.printf("\nconnected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

  audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
  audio.setVolume(8);                  // 0...21(max)
  audio.setConnectionTimeout(500,0);   // needed for some servers
  audio.connecttohost(MEDIA_FILE);
}

void loop()
{
  audio.loop();                        // play audio stream
  vTaskDelay(1);                       // needed !
}



