/*
  Listfiles
 
 This example shows how print out the files in a 
 directory on a SD card 
 	
 The circuit:
 * microSD-Card Adapter (Catalex, China) attached to ESP32 as follows:
 ** MOSI  -   GPIO23
 ** MISO  -   GPIO19
 ** CLK   -   GPIO18
 ** CS    -   GPIO5
 ** GND   -   GND
 ** VCC   -   VIN (5V)
*/

//#include <SPI.h>
#include <Arduino.h>
#include <SD.h>

File root;

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     //Serial.print(entry.path());
     Serial.print(entry.name()));
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.print("\r\nInitializing SD card...");

  if (!SD.begin(17)) {
//  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}



