/* 
 Name:      SD_Update.ino
 Created:   12.09.2017 15:07:17
 Author:    Frederik Merz <frederik.merz@novalight.de>
 Purpose:   Update firmware from SD card

 Steps:
   1. Flash this image to the ESP32 an run it
   2. Copy update.bin to a SD-Card, you can basically
      compile this or any other example
      then copy and rename the app binary to the sd card root
   3. Connect SD-Card as shown in SD_MMC example, 
      this can also be adapted for SPI
   3. After successfull update and reboot, ESP32 shall start the new app
*/

#include <Arduino.h>
#include <Update.h>
#include <SD.h>
//#include <FS.h>
//#include <SD_MMC.h>

void rebootEspWithReason(String reason){
    SD.end();
    Serial.println(reason);
    delay(3000);
    ESP.restart();
}

// perform the actual update from a given stream
void performUpdate(Stream &updateSource, size_t updateSize) {
   if (Update.begin(updateSize)) {      
      size_t written = Update.writeStream(updateSource);
      if (written == updateSize) {
         Serial.println("Written : " + String(written) + " successfully");
      }
      else {
         Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
      }
      if (Update.end()) {
         Serial.println("OTA done!");
         if (Update.isFinished()) {
            Serial.println("Update successfully completed. Rebooting.");
            delay(1000);
            ESP.restart();
         }
         else {
            Serial.println("Update not finished? Something went wrong!");
         }
      }
      else {
         Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }

   }
   else
   {
      Serial.println("Not enough space to begin OTA");
   }
}

// check given FS for valid update.bin and perform update if available
void updateFromFS(fs::SDFS &fs) {
   File updateFile = fs.open("/firmware.bin");
   if (updateFile) {
      if(updateFile.isDirectory()){
         Serial.println("Error, firmware.bin is not a file");
         updateFile.close();
         return;
      }

      size_t updateSize = updateFile.size();

      if (updateSize > 0) {
         Serial.println("Try to start update");
         performUpdate(updateFile, updateSize);
      }
      else {
         Serial.println("Error, file update.bin is empty!");
      }

      updateFile.close();
    
      // whe finished remove the binary from sd card to indicate end of the process
      //fs.remove("/update.bin");      
   }
   else {
      Serial.println("Could not load update.bin from SD root");
   }
}

void setup() {
  uint8_t cardType;
  Serial.begin(115200);
  Serial.println("\nWelcome to the SD-Update example!");

  // You can uncomment this and build again
  // Serial.println("Update successfull");

  //first init and check SD card
  if (!SD.begin()) {
    rebootEspWithReason("Card Mount Failed");
  }
  cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    rebootEspWithReason("No SD card attached");
  }
  else {
    Serial.println("Found SD-Card.");
    // Check if SD-Card is writable (for deleting update.bin file afterwards, to prevent a loop)
    Serial.print("Creating rwtest.txt...");
    File myFile = SD.open("/rwtest.txt", FILE_WRITE);
    if (myFile) {
      myFile.close();
      if (SD.exists("/rwtest.txt")) {
        Serial.print("...ok. Now remove it...");
        SD.remove("/rwtest.txt");
        if (SD.exists("/rwtest.txt")) {
          Serial.println("error!");
        }
        else {
          Serial.println("ok. SD-Card is writable.");        
          updateFromFS(SD);
        }      
      }  
    }
  }
  SD.end();
}

//will not be reached
void loop() {
  
}