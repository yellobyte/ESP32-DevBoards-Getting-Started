/*
  Test Partitions

  Show actual partitions in ESP32 Flash.

  Detailed infos about ESP32 Flash partition tables, types, sizes, custum partition tables, etc:
  "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html?highlight=partitions".

  Last updated 2024-11-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <ESP.h>

void setup() {
  // read partition table entries from flash
  esp_partition_iterator_t partitionIterator;
  const esp_partition_t *part;
  size_t size;

  Serial.begin(115200);

  size = ESP.getFlashChipSize(); 
  Serial.print("Flash chip size: "); 
  Serial.println(size);

  Serial.println("Partition table:");
  
  // search for partitions holding programs
  partitionIterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);

  if (partitionIterator) {
    Serial.println("App Partition table:");
    do {
      part = esp_partition_get(partitionIterator);
      Serial.printf("Type: %02x SubType %02x Address 0x%06X Size 0x%06X Encryption %i Label %s\r\n", 
                    part->type, part->subtype, part->address, part->size, part->encrypted, part->label);
    } while (partitionIterator = esp_partition_next(partitionIterator));
  }
  esp_partition_iterator_release(partitionIterator);

  // search for partitions holding data
  partitionIterator = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

  if (partitionIterator) {
    Serial.println("Data Partition table:");
    do {
      part = esp_partition_get(partitionIterator);
      Serial.printf("Type: %02x SubType %02x Address 0x%06X Size 0x%06X Encryption %i Label %s\r\n", 
                    part->type, part->subtype, part->address, part->size, part->encrypted, part->label);
    } while (partitionIterator = esp_partition_next(partitionIterator));
  }
  esp_partition_iterator_release(partitionIterator);
}

void loop() {
  // 
}