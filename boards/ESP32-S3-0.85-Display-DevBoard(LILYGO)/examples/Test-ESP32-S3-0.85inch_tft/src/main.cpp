/*
  Test TFT

  Testing the TFT functionality on T-QT ESP32-S3 DevBoard with 0.85inch display (Manufacturer LILYGO).
  Show LILYGO logo followed by some text in various colors, different font sizes & screen rotation.

  IMPORTANT:
  As of TFT_eSPI lib version 2.4.79 there is a problem with screen to CGRAM offset for LILYGO T-QT. 
  Please replace the file ".pio\libdeps\ESP32-S3-DevKitC-1\TFT_eSPI\TFT_Drivers\GC9A01_Rotation.h" with 
  the provided file "GC9A01_Rotation.h".

  Last updated 2022-11-06, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include "TFT_eSPI.h"
#include "image_logo.h"
#include "pin_config.h"

// in case you want to check
#define SHOW_CONFIG

#define MYDELAY 5000
#define FONT_SIZE_2 2
#define FONT_SIZE_4 4

TFT_eSPI tft = TFT_eSPI();

void setup() {
  delay(300);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Test TFT on Lilygo T-QT started:");  

  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, LOW);              // TFT backlight on

  // initial settings
  tft.begin();
  tft.setRotation(2);
  tft.invertDisplay(true);

#ifdef SHOW_CONFIG
  setup_t tftSetup;

  tft.getSetup(tftSetup);
  Serial.println("Settings:");
  Serial.printf(" Display driver = %x\n", tftSetup.tft_driver);
  Serial.printf(" Transactions = %s\n", tftSetup.trans  ==  1 ? "Yes" : "No");
  Serial.printf(" Display width = %d, height = %d\n", tftSetup.tft_width, tftSetup.tft_height);
  // screen display area to CGRAM area coordinate offsets
  Serial.printf(" Offsets (x,y): R0 = %d,%d R1 = %d,%d R2 = %d,%d R3 = %d,%d\n", 
                 tftSetup.r0_x_offset, tftSetup.r0_y_offset, tftSetup.r1_x_offset, tftSetup.r1_y_offset,
                 tftSetup.r2_x_offset, tftSetup.r2_y_offset, tftSetup.r3_x_offset, tftSetup.r3_y_offset);
  Serial.printf(" GPIOs: MOSI = %d, MISO = %d, SCK = %d, CS = %d, DC = %d\n", 
                 tftSetup.pin_tft_mosi, tftSetup.pin_tft_miso, tftSetup.pin_tft_clk, tftSetup.pin_tft_cs, tftSetup.pin_tft_dc);
  Serial.println();
#endif  

  // show LILYGO logo for a few secs
  tft.pushImage(0, 0, 128, 128, (uint16_t *)image_logo);
  delay(MYDELAY);

  Serial.println("setup() finished");  
}

void loop() {
  Serial.println("loop() started");  

  Serial.println("small font size 2, inverted portrait, black background, no wrapping");  
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, FONT_SIZE_2);
  tft.setTextWrap(false);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("White text");
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.println("Red text");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Green text");
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.println("Blue text");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("Very long yellow text");  
  tft.setTextColor(TFT_VIOLET, TFT_BLACK);
  tft.println("Violet text");  
  delay(MYDELAY);

  Serial.println("medium font size 4, inverted landscape, white background, wrapping");  
  tft.setRotation(3);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, FONT_SIZE_4);
  tft.setTextWrap(true);

  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.println("Black text");
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.println("Red text");
  tft.setTextColor(TFT_GREEN, TFT_WHITE);
  tft.println("Long green text");
  tft.setTextColor(TFT_BLUE, TFT_WHITE);
  tft.println("Blue text");

  delay(MYDELAY);
}  
