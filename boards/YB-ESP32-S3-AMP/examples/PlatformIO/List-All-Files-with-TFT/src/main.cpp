/*
  List-All-Files-with-TFT

  This example prints all files on the microSD card to serial output and TFT display. We use "SD" library which 
  requires the SD-Card's CS signal (GPIO10) and therefore solder bridge SD_CS must be closed [default]. 
  
  More info about SD library is provided here: "https://github.com/espressif/arduino-esp32/tree/master/libraries/SD"

  The microSD card slot on the YB-ESP32-S3-AMP DevBoard is connected to the fast SPI bus (FSPI) as follows:
    CS - GPIO10, MOSI - GPIO11, SCK - GPIO12, MISO - GPIO13, 

  Additional info:
  The ESP32-S3 includes four SPI controllers: SPI0, SPI1, SPI2(Fast SPI) and SPI3. SPI0/1 are reserved for
  Flash and PSRAM (if available) and should be left alone. The remaining two are available for the public. 
  In Arduino SPI2 & SPI3 are named FSPI & HSPI. FSPI by default is assigned to GPIOs 10-15 (IO MUX). However, 
  both can get pinned to any available GPIO pin if needed but will be slower if routed through GPIO Matrix.

  Last updated 2024-11-29, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// The ILI9341 TFT uses the following GPIO pins:
#define TFT_CS    38   //  HSPI CS0
#define TFT_MOSI  39   //  HSPI D
#define TFT_SCLK  40   //  HSPI CLK
#define TFT_MISO  41   //  HSPI Q
#define TFT_DC    42
#define TFT_RST    1

// wrapper class to provide a very basic "scrolling text" feature on the TFT display,
// with scrolling only supported in portrait mode !
class myTFT : public Adafruit_ILI9341 {
public:
  myTFT(SPIClass *spiClass, int8_t dc, int8_t cs = -1, int8_t rst = -1) 
    : Adafruit_ILI9341(spiClass, dc, cs, rst) {}; // Constructor
  void println(const char* str)
  {
    char buf[strlen(str) + 3];
    static uint16_t dummy1, textHeight = 0;
    static int16_t dummy2, rotation, scrollValue = 0;
    static bool bScroll = false;

    snprintf(buf, sizeof(buf), "%s\r\n", str);
    rotation = getRotation();
    if (rotation == 0 || rotation == 2) {
      if (wrap || (!wrap && !textHeight))
        getTextBounds(buf, 0, 0, &dummy2, &dummy2, &dummy1, &textHeight);
      if (getCursorY() + textHeight > height()) {
        setCursor(0, 0);
        bScroll = true;
      }  
      if (bScroll) {
        scrollValue = (rotation == 0) ? 
                      ((scrollValue + textHeight) > height() ? textHeight : scrollValue + textHeight) :
                      ((scrollValue - textHeight) < 0 ? height() - textHeight : scrollValue - textHeight);
        scrollTo(scrollValue);
        setCursor(0, getCursorY() % height());
        fillRect(0, getCursorY(), width(), textHeight, ILI9341_BLACK);      
      }
    }
    print(buf);
  }
};

SPIClass *spi_onboardSD = new SPIClass(FSPI);
SPIClass *spi_tft       = new SPIClass(HSPI);
myTFT tft = myTFT(spi_tft, TFT_DC, TFT_CS, TFT_RST);
File root;

void printDirectory(File dir, int numTabs) {
  String str;
  File entry;
  while(true) {
    entry = dir.openNextFile();
    if (!entry) {
      break;  // no more files
    }
    str = "";
    for (uint8_t i = 0; i < numTabs; i++) {
      str += ("  ");
    }

    str += entry.name();
    if (entry.isDirectory()) {
      str += "/";
      Serial.println(str);
      tft.println(str.c_str());
      printDirectory(entry, numTabs + 1);
    } 
    else {
      Serial.println(str);
      tft.println(str.c_str());
    }
    entry.close();
    delay(500);  // for better reading on TFT display
  }
}

void setup()
{
  Serial.begin(115200);
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available (board revision 3.x), uncomment if needed
  //while (!Serial);
  //delay(3000);  	
#endif    
  Serial.println();
  Serial.println("Running example \"List-All-Files-with-TFT\":");

  // Arduino FSPI default definitions for SS, MOSI, SCK & MISO are 10, 11, 12 & 13
  // and by default match the boards wiring, so no parameters given
  spi_onboardSD->begin();

  // HSPI will work on any available GPIO (routed through GPIO matrix)
  spi_tft->begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextWrap(true);
  tft.setTextSize(0);
  tft.setRotation(2);

  Serial.print("trying to mount SD in onboard microSD card slot...");
  if (!SD.begin(SS, *spi_onboardSD)) {
    Serial.println("error!");
    return;
  }
  Serial.println("successful");
  
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("microSD Card Size: %lluMB\n", cardSize);

  Serial.println();
  Serial.println("scanning files on SD card:");

  root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("all done");
}

void loop()
{
  // 
}



