/*
  Test-ESP32-CAM_HW-Info

  Show available RAM/PSRAM on ESP32-CAM board.

  Last updated 2022-08-28, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include "soc/efuse_reg.h"

#ifndef EFUSE_RD_CHIP_VER_RESERVE_S
  #define EFUSE_RD_CHIP_VER_RESERVE_S 12
#endif
#ifndef EFUSE_RD_CHIP_VER_RESERVE_V
  #define EFUSE_RD_CHIP_VER_RESERVE_V 7
#endif

uint8_t *buf;

void logRam(bool showRam = true, bool showPsram = true) 
{
  if (showRam) {
    Serial.printf("\nESP32 RAM Size: %d Byte, Used: %d Byte, Free: %d, max alloc: %d Byte (largest free block)", 
                  ESP.getHeapSize(), ESP.getHeapSize() - ESP.getFreeHeap(), ESP.getFreeHeap(), ESP.getMaxAllocHeap());
  }
  if (showPsram) {
    if (psramFound())
      Serial.printf("\nESP32 PSRAM Size: %d Byte, Used: %d Byte, Free: %d Byte", ESP.getPsramSize(), ESP.getPsramSize() - ESP.getFreePsram(), ESP.getFreePsram());
    else
      Serial.print("\nESP32 PSRAM not found !");    
  }
  Serial.println();
  Serial.println();
}

bool allocRAM(size_t size)
{
  size_t i;

  Serial.printf("-> malloc(%d), ", size);
  buf = (uint8_t*)malloc(size);
  if (buf != NULL) {
    // check every single position
    for (i = 0; i < size; i++) {
      buf[i] = i % 0xFF;
      if (i % 0xFF != buf[i]) {
        Serial.printf("w/r error at buf[%d]\n", i);
        break;
      }
    }
    if (i == size) {
      Serial.printf("success, buf = %x (RAM starts from 0x3FF00000, PSRAM starts from 0x3F800000)\n", buf);
      return true;
    }	
  } 
  else {
    Serial.printf("error !\n");
  }
  return false;
}  

void freeBuf()
{
  if (buf != NULL) {
    Serial.printf("-> free(buf=%x)\n", buf); 
    free(buf);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP32 Chip Rev. (official version): ");
  Serial.println((int)(REG_READ(EFUSE_BLK0_RDATA3_REG) >> (EFUSE_RD_CHIP_VER_RESERVE_S) && EFUSE_RD_CHIP_VER_RESERVE_V));
  Serial.print("ESP32 SDK: ");        Serial.println(ESP.getSdkVersion());
  Serial.print("ESP32 CPU FREQ: ");   Serial.print(getCpuFrequencyMhz());             Serial.println(" MHz");
  Serial.print("ESP32 APB FREQ: ");   Serial.print(getApbFrequency() / 1000000.0, 1); Serial.println(" MHz (Advanced Peripheral Bus)");
  Serial.print("ESP32 FLASH SIZE: "); Serial.print(ESP.getFlashChipSize());           Serial.println(" Byte");

  logRam();
  allocRAM(5000 * 1024);
  allocRAM(4200 * 1024);
  allocRAM(4000 * 1024);
  logRam();

  freeBuf();
  logRam();

  allocRAM(1024 * 1024);
  logRam();
  freeBuf();
  logRam();

  allocRAM(1024);
  logRam();
  freeBuf();

  logRam();
}

void loop() {
  // put your main code here, to run repeatedly:
}