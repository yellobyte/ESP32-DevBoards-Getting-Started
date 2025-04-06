/*
  Play-All-MP3-Files-on-SD

  This example plays all mp3-files from the microSD card over the two integrated
  MAX98357A I2S amplifiers (left + right audio channels).

  Since we use "SD" library which requires the cards CS signal (GPIO10) the
  solder bridge SD_CS must be closed [default]. Please add libraries "SD" and
  "ESP32-audioI2S" to your project or ArduinoIDE.

  Jumper bridge DAC_MUTE is open by default and both MAX98357A are active.
  Closing the jumper bridge will connect pin GPIO47 (onboard LED IO47) with the
  amps SD_MODE pin. Setting GPIO47 to LOW (LED off) will shut down (mute) the
  amps and setting GPIO47 to HIGH (LED on) will activate the amps.

  Last updated 2025-04-06, ThJ <yellobyte@bluewin.ch>
*/

#include "Audio.h"
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define MAX_PATH_DEPTH 5

SPIClass *spi_onboardSD = new SPIClass(FSPI);
Audio audio;
File root, entry;
bool playing = false;
std::vector<File> dirChain;

void searchAudioFiles() {
  while (!playing && dirChain.size()) {
    entry = dirChain[dirChain.size() - 1].openNextFile();
    if (!entry) {
      // no (more) files in this directory
      dirChain[dirChain.size() - 1].close();
      dirChain.pop_back();
      break; 
    }
    if (entry.isDirectory()) {
      if (dirChain.size() < MAX_PATH_DEPTH) {
        dirChain.push_back(entry);  // dir entry stays open while member of chain
        break;
      }
    } 
    else if (String(entry.name()).endsWith("mp3")) {
      Serial.print("now playing: ");
      Serial.println(String(entry.path()));
      audio.connecttoFS(SD, entry.path());
      entry.close();
      playing = true;
      break;
    }
    entry.close();
  }
}

void audio_eof_mp3(const char *info) // called at end of each file
{
  Serial.print("eof_mp3: ");
  Serial.println(info);
  playing = false;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: only needed for board revision 3.x
  while (!Serial);
  delay(3000);
#endif
  Serial.println();
  Serial.println("running example \"Play-All-MP3-Files-on-SD\":");

  // Arduino FSPI default definitions for SS, MOSI, SCK & MISO are 10, 11, 12 & 13
  // and by default match the boards wiring, so no parameters given
  spi_onboardSD->begin();

  if (!SD.begin(SS, *spi_onboardSD)) {
    Serial.println("error mounting microSD");
    return;
  }
  digitalWrite(LED_BUILTIN, HIGH); // status LED On

  root = SD.open("/");
  dirChain.push_back(root);

  audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
  audio.setVolume(6); // 0...21(max)
}

void loop() {
  if (!playing) {
    searchAudioFiles();
  }
  audio.loop();  // play mp3 audio file
  vTaskDelay(1); // needed !
}
