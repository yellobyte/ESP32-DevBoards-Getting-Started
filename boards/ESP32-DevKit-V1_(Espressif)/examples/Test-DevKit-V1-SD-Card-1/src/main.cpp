/*
  Test-SD-1

  This example prints the files on a connected SD card.

  The (micro)SD-Card Adapter is attached to ESP32 dev board as follows:
    SD MOSI  -  GPIO23
    SD MISO  -  GPIO19
    SD CLK   -  GPIO18
    SD CS    -  GPIO17

  More info about ESP32 Arduino SD lib is provided here:
  "https://github.com/espressif/arduino-esp32/tree/master/libraries/SD".

  Last updated 2020-04-15, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <SD.h>

File root;

void printDirectory(File dir, int numTabs) {
  while(true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print("   ");
    }

    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } 
    else {
      Serial.print("   ");
      // entry.size() returns file size in bytes and 0 for directories
      Serial.println(entry.size());
    }
    entry.close();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("initialize SD card...");

  if (!SD.begin(17)) {
    Serial.println("initialization failed!");
    while (true);
  }
  Serial.println("initialization successful.");
  Serial.println("scan SD card:");
  Serial.println();

  // print SD content starting with root
  root = SD.open("/");
  printDirectory(root, 0);

  Serial.println();
  Serial.println("done!");
}

void loop()
{
  // 
}



