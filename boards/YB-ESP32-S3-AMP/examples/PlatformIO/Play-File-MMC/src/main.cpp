/*
  Play-File-MMC

  Plays a single mp3-file from microSD card over the two MAX98357A I2S amplifiers (left + right audio channels).
  
  The "SD_MMC" library does not use the CS signal. So the solder bridge SD_CS can be opened to free GPIO10 for other usage.
  Please add library "ESP32-audioI2S" to your project or ArduinoIDE.

  Jumper bridge DAC_MUTE is open by default and both MAX98357A are active. Closing the jumper bridge will connect pin
  GPIO47 (onboard LED IO47) with the amps SD_MODE pin. Setting GPIO47 to LOW (LED off) will shut down (mute) the amps
  and setting GPIO47 to HIGH (LED on) will activate the amps.

  Last updated 2025-04-06, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <SD_MMC.h>
#include "Audio.h"

#define FILE_ON_SD "/File1.mp3"

Audio audio;
File  root;

void audio_info(const char *info) {     // file info
  Serial.print("info: "); 
  Serial.println(info);
}
void audio_id3data(const char *info) {  // id3 metadata
  Serial.print("id3: ");
  Serial.println(info);
}
void audio_eof_mp3(const char *info) {  // end of file
  Serial.print("eof: ");
  Serial.println(info);
}

void setup()
{
  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: only needed for board revision 3.x
  while (!Serial);     	
#endif	  
  Serial.println();
  Serial.println("running example \"Play-File-MMC\":");

  // the SD_MMC default definitions for CMD(MOSI), SCK & D0(MISO) are 11, 12 & 13 
  // (see pins_arduino.h for more info) and match the boards wiring
  SD_MMC.setPins(SCK, MOSI, MISO);

  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("error mounting SD in onboard microSD card slot");
    return;
  }

  audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
  audio.setVolume(8);                  // 0...21(max)
  audio.connecttoFS(SD_MMC, FILE_ON_SD);
}

void loop()
{
  audio.loop();                        // play mp3 audio file
  vTaskDelay(1);                       // needed !
}



