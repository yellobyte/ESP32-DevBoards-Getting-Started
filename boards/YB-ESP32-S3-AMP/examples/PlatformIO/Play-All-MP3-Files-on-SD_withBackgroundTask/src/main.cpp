/*
  Play-All-MP3-Files-on-SD_withBackgroundTask

  This example plays all mp3-files from the microSD card over the two integrated MAX98357A I2S amplifiers 
  (left + right audio channels).
  
  Since we use "SD" library which requires the cards CS signal (GPIO10) the solder bridge SD_CS must be closed [default].
  Please add libraries "SD" and "ESP32-audioI2S" to your project or ArduinoIDE. 

  Jumper bridge DAC_MUTE is open by default and both MAX98357A are active. Closing the jumper bridge will connect pin
  GPIO47 (onboard LED IO47) with the amps SD_MODE pin. Setting GPIO47 to LOW (LED off) will shut down (mute) the amps
  and setting GPIO47 to HIGH (LED on) will activate the amps.

  Last updated 2025-04-06, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "Audio.h"

SPIClass *spi_onboardSD = new SPIClass(FSPI);
Audio     audio;
File      root;
String    s = "";
bool      playing = false;

void searchAudioFiles(File dir) 
{
  File entry;
  while (true) {
    do { delay(1); } while (playing);  // wait and reset idle task watchdog while file is playing 

    entry = dir.openNextFile();
    if (!entry) break;                 // no more files, function returns

    if (entry.isDirectory()) {
      searchAudioFiles(entry);
    }
    else if (String(entry.name()).endsWith("mp3")) {
      s = entry.path();
    } 
    entry.close();

    if (s.length()) {
      Serial.print("now playing: ");
      Serial.println(s);
      audio.connecttoFS(SD, s.c_str());
      playing = true;
      s = "";
    }
  }
}

void backgroundTask(void *parameter)
{
  root = SD.open("/");
  searchAudioFiles(root);
  root.close();
  vTaskDelete(NULL);                  // task finishes when all files were played
}

void audio_eof_mp3(const char *info)  // called at end of each file
{
  Serial.print("eof_mp3: ");
  Serial.println(info);
  playing = false;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available (board revision 3.x), uncomment if needed
  //while (!Serial);
  //delay(3000);	
#endif	
  Serial.println();
  Serial.println("running example \"Play-All-MP3-Files-on-SD_withBackgroundTask\":");

  // Arduino FSPI default definitions for SS, MOSI, SCK & MISO are 10, 11, 12 & 13
  // and by default match the boards wiring, so no parameters given
  spi_onboardSD->begin();

  if (!SD.begin(SS, *spi_onboardSD)) {
    Serial.println("error mounting microSD");
    return;
  }
  digitalWrite(LED_BUILTIN, HIGH);      // status LED On

  audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
  audio.setVolume(8);                  // 0...21(max)

  xTaskCreate(backgroundTask, "bgTask", 8192, NULL, 1, NULL);
}

void loop()
{
  audio.loop();                        // play mp3 audio file
  vTaskDelay(1);                       // needed !
}


