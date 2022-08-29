/*
  Test_RGB-LED

  Test of YD-ESP32-S3_DevBoard (Manufactorer VCC-GND.COM) onboard RGB-LED.

  Last updated 2022-08-28, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

//#define RGB_MATRIX_64LEDS

#ifdef RGB_MATRIX_64LEDS
#define PIN 47
#define LED_COUNT 64
#else
// default: onboard RGB LED
#define PIN 48
#define LED_COUNT 1
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setPixelColor(uint8_t r, uint8_t g, uint8_t b)
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
  Serial.printf("Color setting: r=%d, g=%d, b=%d\n", r, g, b);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("--> Sketch started: RGB-LED-Test ESP32-S3-N8R2-DevKitC-1 Modul (VCC-GND.COM):\n"); 

  strip.begin();
  //strip.setPixelColor(0, strip.Color(255, 0, 0)); // Red
  strip.setBrightness(5);                         // 1 = Min, 255 = one below Max, 0 = Max
  //strip.show();                                   // Initialize all pixels to 'off'
}

void loop() {
  char c;

  if (Serial.read(&c, 1) && (c == 'r' || c == 'R')) {
		// check for terminal input: reboot requested
    Serial.println("\nSystem will restart in 3 seconds.\n\n"); 
    delay(3000);
    ESP.restart();  
  }
  setPixelColor(255, 0, 0); // Red
  delay(1000);
  setPixelColor(0, 255, 0); // Green
  delay(1000);
  setPixelColor(255, 255, 0); // Yellow
  delay(1000);  
  setPixelColor(0, 0, 255); // Blue
  delay(1000);
  Serial.println("Loop finished.");
}