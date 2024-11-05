/*
  Test-SD-2

  This example creates and destroys a file "example.txt" and a directory "myDir" on the attached SD card.

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

File myFile;

void setup()
{
  Serial.begin(115200);
  Serial.print("\ninitialize SD card...");

  if (!SD.begin(17)) {
    Serial.println("initialization failed!");
    while(true);
  }
  Serial.println("initialization done.");

  // check if file /example.txt exists on SD
  if (SD.exists("/example.txt")) {
    Serial.println("file /example.txt already exists");
  }
  else {
    Serial.print("create file /example.txt ...");
    myFile = SD.open("/example.txt", FILE_WRITE);
    myFile.close();

    // check if the file has been created
    if (SD.exists("/example.txt")) {
      Serial.println("success.");
    }
    else {
      Serial.println("error.");
      while(true);
    }
  }

  // delete the file
  Serial.print("delete file /example.txt ...");
  SD.remove("/example.txt");

  if (SD.exists("/example.txt")) {
    Serial.println("error.");
  }
  else {
    Serial.println("success.");
  }

  // check if directory /myDir already exists on SD
  if (SD.exists("/myDir")) {
    Serial.println("directory /myDir already exists");
  }
  else {
    Serial.print("create directory /myDir ...");
    bool err = SD.mkdir("/myDir");
    // check if directory has been created
    if (SD.exists("/myDir")) {
      Serial.println("success.");
    }
    else {
      Serial.println("error.");
      while(true);
    }
  }

  // delete the directory
  Serial.print("delete directory /myDir ...");
  SD.rmdir("/myDir");

  if (SD.exists("/myDir")) {
    Serial.println("error.");
  }
  else {
    Serial.println("success.");
  }

  Serial.println("all done.");
}

void loop()
{
  // nothing happens after setup finishes.
}



