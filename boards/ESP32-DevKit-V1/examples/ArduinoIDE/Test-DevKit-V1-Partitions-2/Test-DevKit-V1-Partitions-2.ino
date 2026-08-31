/*
  Test NVS Partition - Preferences

  Test the NVS (non-volatile, data) partition in ESP32 Flash by writing/reading key/value pairs. 
  We use the "Preferences" Library. For further info please hava a look at
  "https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html".

  Preferences data uses a portion of the on-board non-volatile memory (NVS partition) of the ESP32 
  to store data. This data is retained across restarts and/or loss of power events to the system.

  The data in the NVS partition is stored in sections called "namespace". Within each 
  namespace is a set of key-value pairs. The "key" is the name of the data item and the "value" 
  is the value of that piece of data. Namespace and key names are character strings and are 
  limited to a maximum of 15 characters.

  Preferences works best for storing many small values, rather than a few large values. 
  For storing large amounts of data, consider using a file system library such as LitteFS.

  Detailed infos about ESP32 Flash partition tables, types, sizes, custum partition tables, etc:
  "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html".

  Last updated 2026-08-31, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Preferences.h>

#define MY_NAMESPACE "TEST123"  // namespace definition (max. 15 chars)
#define NKEY 30                 // number of keys to test
Preferences nvs;

void setup()
{
  int count = 0;
  char key[20], value[20];
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Running example \"Test NVS Partition - Preferences\"");    

  if (!nvs.begin(MY_NAMESPACE, true)) {      // open MY_NAMESPACE section readonly
    // missing/defect NVS partition or missing MY_NAMESPACE section
    Serial.printf("Opening NVS namespace %s was unsuccessful\n", MY_NAMESPACE);    

    if (!nvs.begin(MY_NAMESPACE, false)) {   // create & open MY_NAMESPACE section writable    
      Serial.printf("Creating NVS namespace %s was unsuccessful\n", MY_NAMESPACE);
      while(true);
    }
    Serial.printf("Creating NVS namespace %s was successful. Free entries: %lu\n", 
                  MY_NAMESPACE, (unsigned long)nvs.freeEntries());

    // not necessary:
    //Serial.printf("clearing NVS namespace %s\n", MY_NAMESPACE);
    //nvs.clear();

    Serial.printf("Writing key-value pairs to NVS namespace %s\n", MY_NAMESPACE);
    for(int i = 0; i < NKEY; i++ ) {
      sprintf(key, "MyKey_%02d", i);
      sprintf(value, "MyValue_%02d", i);
      nvs.putString(key, value) ;            // add key-value pair to MY_NAMESPACE section
    }

    nvs.end();                               // close NVS section
    nvs.begin(MY_NAMESPACE, true) ;          // (re-)open readonly
  }
  else {
    Serial.printf("Opening NVS namespace %s was successful. Free entries: %lu\n", 
                  MY_NAMESPACE, (unsigned long)nvs.freeEntries());    
  }
  
  //
  // printing all key-value pairs in MY_NAMESPACE section of NVS
  //
  Serial.printf("Reading key-value pairs from NVS namespace %s\n", MY_NAMESPACE);    
  for (int i = 0; i < NKEY; i++ ) {
    sprintf(key, "MyKey_%02d", i);
    String val = nvs.getString(key);
    if (val.length()) {                              // does key exist?
      count++ ;                                      // count it
      Serial.printf("%s = %s\n", key, val.c_str());  // print key-value pair 
    }
  }
  if (count < NKEY) {
    Serial.printf("Error: keys found %d != keys expected %d\n", count, NKEY ) ;
  }

  Serial.println("All done.") ;
}

void loop()
{
  ;
}