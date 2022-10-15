/*
  SD card basic file example "Files"

 This example shows how to create and destroy an SD card file
 
 The circuit:
 * microSD-Card Adapter (Catalex, China) attached to ESP32 as follows:
 ** MOSI  -   GPIO23
 ** MISO  -   GPIO19
 ** CLK   -   GPIO18
 ** CS    -   GPIO5
 ** GND   -   GND
 ** VCC   -   VIN (5V)
*/

#include <Arduino.h>
//#include <SPI.h>
#include <SD.h>

File myFile;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.print("\r\nInitializing SD card...");

  if (!SD.begin(/*4*/)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("/example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating example.txt...");
  myFile = SD.open("/example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("/example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }

  // delete the file:
  Serial.println("Removing example.txt...");
  SD.remove("/example.txt");

  if (SD.exists("/example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }
}

void loop()
{
  // nothing happens after setup finishes.
}



