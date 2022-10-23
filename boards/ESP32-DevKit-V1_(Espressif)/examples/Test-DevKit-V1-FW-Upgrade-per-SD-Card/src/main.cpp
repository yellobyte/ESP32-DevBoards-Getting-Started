/*
  Test Update-per-SD

  This example demonstrates the possibility to flash ESP32 devices with new software per attached SD card.
  Important for devices without WiFi or USB-to-PC connections.  

  The program scans an attached SD card for a file "firmware.bin" and if found, flashes it onto the ESP32.
  The ESP32 will reboot afterwards and run the new program which only lets the onboard LED blink. If defined,
  the update file on SD card gets deleted after upgrade.

  So to get started:
  1) connect a SD card module to the dev board as explained below
  2) build this example and flash it onto the dev board per USB, the program will not detect an upgrade file 
  3) copy the provided file "firmware.bin" onto SD card and insert the SD card into the SD card module

  Finally, press the reset button on the dev board and the ESP32 gets re-flashed from SD card. If successful, 
  the LED on GPIO2 will blink. 

  The (micro)SD-Card Adapter needs to be connected to the ESP32 dev board as follows:
    SD MOSI  -  GPIO23
    SD MISO  -  GPIO19
    SD CLK   -  GPIO18
    SD CS    -  GPIO17

  More info about ESP32 Arduino SD lib is provided here:
  "https://github.com/espressif/arduino-esp32/tree/master/libraries/SD".

  Last updated 2020-04-18, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Update.h>
#include <SD.h>

#define SD_CS_PIN        17
#define UPDATE_FILE_NAME "/firmware.bin"

// uncomment if update file on SD card should get deleted after upgrade is done
//#define DELETE_UPDATE_FILE
#ifdef DELETE_UPDATE_FILE
#define TEST_FILE_NAME   "/testfile.txt"
#endif

size_t updateSize;
File   myTestFile,
       myUpdateFile;

//
// check for update file on SD card and write access to SD card (if needed)
//
bool checkUpdate()
{
  Serial.printf("check for update file %s on SD card...", UPDATE_FILE_NAME);
  myUpdateFile = SD.open(UPDATE_FILE_NAME);
  if (!myUpdateFile) {
    Serial.println("no update file detected");
    return false;
  }
  updateSize = myUpdateFile.size();
  bool isDir = myUpdateFile.isDirectory();
  if (isDir || updateSize == 0) {
    Serial.println("error, entry is a directory or update file is empty");
    myUpdateFile.close();
    return false;
  }
  myUpdateFile.close();
  Serial.printf("ok, update file (size: %d Byte) detected\n", updateSize);

#ifdef DELETE_UPDATE_FILE
  // check write access to SD-Card
  Serial.print("test write access to SD card...");
  myTestFile = SD.open(TEST_FILE_NAME, FILE_WRITE);
  if (myTestFile) {
    myTestFile.close();
    SD.remove(TEST_FILE_NAME);
    if (SD.exists(TEST_FILE_NAME)) {
      Serial.println("error deleting test file");
      return false;
    }
  }
  else {
    Serial.println("error creating test file");
    return false;
  }
  Serial.println("ok");
#endif

  return true;
}

//
// flash ESP32 with update file
//
bool performUpdate() 
{
  bool ret = false;

  myUpdateFile = SD.open(UPDATE_FILE_NAME);
  if (Update.begin(updateSize)) {  
    size_t written = Update.writeStream(myUpdateFile);

    if (written != updateSize) {
      Serial.printf("error, only %d/%d bytes written to flash\n", written, updateSize);
    }
    if (Update.end()) {
      if (Update.isFinished()) {
        ret = true;
      }
    }
    else {
      Serial.printf("error, update returned error code %d\n", Update.getError());
    }
  }
  else {
    Serial.println("error, flash to small for update");
  }
  myUpdateFile.close();

  return ret;
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Example Update-per-SD started:");

  Serial.print("initialize SD card...");  
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("error");
    return;
  }

  if (SD.cardType() == CARD_NONE) {
    Serial.println("error, wrong SD card type");
    SD.end();
    return;
  }
  Serial.println("ok");

  if (!checkUpdate()) {
    // no update file found or SD card problem (no write access)
    SD.end();
    return;
  }

  Serial.println("flash update file to ESP32, please wait...");
  bool ret = performUpdate();

  if (ret) {
    Serial.println("UPDATE SUCCESSFULLY FLASHED TO ESP32");
  }
  else {
    Serial.println("UPDATE NOT SUCCESSFUL");
  }

#ifdef DELETE_UPDATE_FILE
  // remove file from SD card to prevent possible update loop
  SD.remove(UPDATE_FILE_NAME);  
  delay(100);
  if (SD.exists(UPDATE_FILE_NAME)) {
    Serial.println("error deleting update file on SD");
  }
  else {  
    Serial.println("update file on SD successfully deleted");
  }  
#endif
  SD.end();

  if (ret) {
    // update was successful, reboot to run the new program
    Serial.println("system will restart in 3 seconds"); 
    delay(3000);
    ESP.restart();   
  }

  // program will not get here when update was successful
  return;
}

void loop() {
  //
}