/*
  Play-Radio-Station

  Plays a radio station's audio stream over the 2 MAX98357A I2S amplifiers (left+right audio channels).
  Make sure you have library "ESP32-audioI2S" added to your project or ArduinoIDE. More info about this versatile lib
  are available on its homepage "https://github.com/schreibfaul1/ESP32-audioI2S".

  Jumper bridge DAC_MUTE is open by default and both MAX98357A are active. Closing the jumper bridge will connect pin
  GPIO47 (onboard LED IO47) with the amps SD_MODE pin. Setting GPIO47 to LOW (LED off) will shut down (mute) the amps
  and setting GPIO47 to HIGH (LED on) will activate the amps.

  Last updated 2024-10-15, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <WiFi.h>
#include "Audio.h"

#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://stream.laut.fm/oldies"

#define I2S_BCLK    5  // GPIO 5/6/7 are not wired to a pin, they are exclusively used for the MAX98357A
#define I2S_LRC     6
#define I2S_DOUT    7

const char ssid[] = "MySSID";
const char pass[] = "MyPassword"; 

Audio audio;
 
void audio_info(const char *info) {     // file info
  Serial.print("info        "); 
  Serial.println(info);
}
void audio_id3data(const char *info) {  // id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Running example \"Play-Radio-Station\":");

  // connecting to local WiFi network
  Serial.printf("Connecting to WiFi network \"%s\"\n", ssid);
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.printf("\nConnected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10);                 // 0...21(max)
  audio.setConnectionTimeout(1200,0);  // needed for some stations esp. from around the globe
  audio.connecttohost(RADIO_STREAM);
}

void loop()
{
  audio.loop();                        // play audio stream
}



