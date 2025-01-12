/*
  Test RAM/PSRAM

  1) Shows available RAM/PSRAM on the YB-ESP32-S3-XXX dev board.
  2) Allocates memory chunks as big as possible and shows where the allocated memory comes from.
     According to the ESP32-S3 spec the address mapping structure is as follows:
       PSRAM:        0x3C000000-0x3DFFFFFF, 0x42000000-0x43FFFFFF 
       SRAM:         0x3FC88000-0x3FCFFFFF, 0x40370000-0x403DFFFF
       RTC FAST MEM: 0x600FE000-0x600FFFFF
  3) When almost no memory is left then even printing serial output might get hampered.
  4) Finally all allocated memory is released.

  Last updated 2025-01-12, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

#define ORIGIN(p)                                                                         \
        ((p >= (uint8_t*)0x3C000000 && p <= (uint8_t*)0x3DFFFFFF) ||                      \
         (p >= (uint8_t*)0x42000000 && p <= (uint8_t*)0x43FFFFFF)) ? "PSRAM" :            \
         ((p >= (uint8_t*)0x3FC88000 && p <= (uint8_t*)0x3FCFFFFF) ||                     \
          (p >= (uint8_t*)0x40370000 && p <= (uint8_t*)0x403DFFFF)) ? "SRAM" :            \
          (p >= (uint8_t*)0x600FE000 && p <= (uint8_t*)0x600FFFFF) ? "RTC FAST MEM" : ""

uint8_t *p, *pArray[30], cnt = 0;

void logRam(bool showRam = true, bool showPsram = true) 
{
  if (showRam) {
    Serial.printf("ESP32 RAM Size: %d Byte, Used: %d Byte, Free: %d, max alloc: %d Byte (largest free block)", 
                  ESP.getHeapSize(), ESP.getHeapSize() - ESP.getFreeHeap(), ESP.getFreeHeap(), ESP.getMaxAllocHeap());
  }
  if (showPsram) {
    if (psramFound())
      Serial.printf("\nESP32 PSRAM Size: %d Byte, Used: %d Byte, Free: %d Byte", 
                    ESP.getPsramSize(), ESP.getPsramSize() - ESP.getFreePsram(), ESP.getFreePsram());
    else
      Serial.print("\nESP32 PSRAM not found !");    
  }
  Serial.println();
  Serial.println();
}

uint8_t *allocMaxRAM(size_t startSize)
{
  uint8_t *p;
  size_t i, size;

  for (size = startSize, p = NULL; size > 0 && p == NULL; size--) {
    p = (uint8_t*)malloc(size);
  }
  if (p == NULL) {
    Serial.printf("-> error !\n");
    return NULL;
  }
  Serial.printf("-> malloc(%d): success, p = %x(...%x), %s\n", size, p, p + size, ORIGIN(p));

  // very simple check of every single byte in allocated memory
  // write pattern
  for (i = 0; i < size; i++) {
    p[i] = i % 0xFF;
  }
    // read pattern
  for (i = 0; i < size; i++) {
    if (i % 0xFF != p[i]) {
      Serial.printf("w/r error at p[%d]\n", i);
      break;
    }
  }

  return p;
} 

uint8_t *allocRAM(size_t size)
{
  uint8_t *p;
  size_t i;

  Serial.printf("-> malloc(%d): ", size);
  p = (uint8_t*)malloc(size);
  if (p == NULL) {
    Serial.printf("error !\n");
    return NULL;
  }
  Serial.printf("success, p = %x(...%x), %s\n", p, p + size, ORIGIN(p));

  // very simple check of every single byte in allocated memory
  // write pattern
  for (i = 0; i < size; i++) {
    p[i] = i % 0xFF;
  }
    // read pattern
  for (i = 0; i < size; i++) {
    if (i % 0xFF != p[i]) {
      Serial.printf("w/r error at p[%d]\n", i);
      break;
    }
  }

  return p;
}  

void setup() {
  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: only needed for board revision 3.x
  while (!Serial);     	
#endif	

  delay(1000);
  Serial.println();
  Serial.println("--> Sketch started: Testing RAM/PSRAM on YB-ESP32-S3-XXX dev board:\n"); 
  Serial.print("ESP32 SDK: ");        Serial.println(ESP.getSdkVersion());
  Serial.print("ESP32 CPU FREQ: ");   Serial.print(getCpuFrequencyMhz());             Serial.println(" MHz");
  Serial.print("ESP32 APB FREQ: ");   Serial.print(getApbFrequency() / 1000000.0, 1); Serial.println(" MHz (Advanced Peripheral Bus)");
  Serial.print("ESP32 FLASH SIZE: "); Serial.print(ESP.getFlashChipSize());           Serial.println(" Byte\n");

  logRam();
  Serial.println("----- allocate biggest possible chunk, please wait ----->");
  p = allocMaxRAM(2 * 1024 * 1024);   // 2MB start size
  if (!p) return;
  pArray[cnt++] = p;
  logRam();

  Serial.println("----- allocate more chunks of memory ----->");
  for (;ESP.getMaxAllocHeap() > 0;) {
    p = allocRAM(ESP.getMaxAllocHeap());
    if (!p) break;
    pArray[cnt++] = p;
    logRam();
  }

  Serial.println("----- all memory used up ----->");

  Serial.println("\n----- free all allocated memory ----->");
  for (uint8_t i = cnt; i > 0; i--)
    free(pArray[i - 1]);
  logRam();

  Serial.println("\nAll done."); 
}

void loop() {
 //
}