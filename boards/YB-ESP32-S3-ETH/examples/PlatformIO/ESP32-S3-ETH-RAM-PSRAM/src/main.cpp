/*
  Test_RAM

  Shows available RAM/PSRAM on your variant of YB-ESP32-S3-ETH dev board and tests the PSRAM.
  Depending on the amount of memory allocated with malloc() it will come from RAM or PSRAM.

  Last updated 2023-06-05, ThJ <yellobyte@bluewin.ch>
*/
#include <Arduino.h>

uint8_t *buf;

void logRam(bool showRam = true, bool showPsram = true) 
{
  if (showRam) {
    Serial.printf("ESP32 RAM Size: %d Byte, Used: %d Byte, Free: %d, max alloc: %d Byte (largest free block)", 
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

  Serial.printf("-> malloc(%d): ", size);
  buf = (uint8_t*)malloc(size);
  if (buf != NULL) {
    // simple check of every single byte position
    for (i = 0; i < size; i++) {
      buf[i] = i % 0xFF;
      if (i % 0xFF != buf[i]) {
        Serial.printf("w/r error at buf[%d]\n", i);
        break;
      }
    }
    if (i == size) {
			Serial.printf("success, buf = %x\n", buf);
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
  Serial.begin(115200);
  // port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini)  
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  while (!Serial);     	
#endif	

  delay(1000);
  Serial.println();
  Serial.println("--> Sketch started: Testing RAM/PSRAM on YB-ESP32-S3-ETH dev board:\n"); 
  Serial.print("ESP32 SDK: ");        Serial.println(ESP.getSdkVersion());
  Serial.print("ESP32 CPU FREQ: ");   Serial.print(getCpuFrequencyMhz());             Serial.println(" MHz");
  Serial.print("ESP32 APB FREQ: ");   Serial.print(getApbFrequency() / 1000000.0, 1); Serial.println(" MHz (Advanced Peripheral Bus)");
  Serial.print("ESP32 FLASH SIZE: "); Serial.print(ESP.getFlashChipSize());           Serial.println(" Byte");

  logRam();

  Serial.println("----- allocation of 6MB memory ----->");
  allocRAM(6 * 1024 * 1024);
  logRam();
  freeBuf();
  logRam();

  Serial.println("----- allocation of 2MB memory ----->");
  allocRAM(2 * 1024 * 1024);
  logRam();
  freeBuf();
  logRam();

  Serial.println("----- allocation of 100kB memory ----->");
  allocRAM(100 * 1024);
  logRam();
  freeBuf();
  logRam();

  Serial.println("----- allocation of 1kB memory ----->");
  allocRAM(1024);
  logRam();
  freeBuf();
  logRam();

  Serial.println("\nPress 'r' or 'R' to reset the board.\n"); 
}

void loop() {
  char c;

  if (Serial.read(&c, 1) && (c == 'r' || c == 'R')) {
		// check for terminal input: reboot requested
    Serial.println("\nSystem will restart in 3 seconds.\n\n"); 
    delay(3000);
    ESP.restart();  
  }
}