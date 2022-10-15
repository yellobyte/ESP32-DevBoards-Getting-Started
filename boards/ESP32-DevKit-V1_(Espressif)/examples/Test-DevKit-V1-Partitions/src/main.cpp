/*
  Test Partitions

  Show actual partitions in ESP32 Flash.

  Detailed infos about ESP32 Flash partition tables, types, sizes, custum partition tables, etc:
  "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html?highlight=partitions".

  Last updated 2020-04-17, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <esp_ota_ops.h>

void setup() {
  // put your setup code here, to run once:

  // read partition table entries
  size_t ul;
  esp_partition_iterator_t _mypartiterator;
  const esp_partition_t *_mypart;

  Serial.begin(115200);

  ul = spi_flash_get_chip_size(); 
  Serial.print("Flash chip size: "); 
  Serial.println(ul);

  Serial.println("Partition table:");
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator) {
    Serial.println("App Partition table:");
    do {
      _mypart = esp_partition_get(_mypartiterator);
      Serial.printf("Type: %02x SubType %02x Address 0x%06X Size 0x%06X Encryption %i Label %s\r\n", 
                    _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->encrypted, _mypart->label);
    } while (_mypartiterator = esp_partition_next(_mypartiterator));
  }
  esp_partition_iterator_release(_mypartiterator);
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator) {
    Serial.println("Data Partition table:");
    do {
      _mypart = esp_partition_get(_mypartiterator);
      Serial.printf("Type: %02x SubType %02x Address 0x%06X Size 0x%06X Encryption %i Label %s\r\n", 
                    _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->encrypted, _mypart->label);
    } while (_mypartiterator = esp_partition_next(_mypartiterator));
  }
  esp_partition_iterator_release(_mypartiterator);
}

void loop() {
  // put your main code here, to run repeatedly:
}