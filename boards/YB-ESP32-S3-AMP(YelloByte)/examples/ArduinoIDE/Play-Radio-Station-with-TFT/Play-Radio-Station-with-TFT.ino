/*
  Play-Radio-Station-with-TFT

  Plays a radio station's audio stream over the 2 MAX98357A I2S amplifiers (left+right audio channels). An attached 
  TFT display with ILI9431 controller shows some serial output.

  Please add libraries "ESP32-audioI2S", "Adafruit GFX" and "Adafruit ILI9341" to your project or ArduinoIDE.

  Jumper bridge DAC_MUTE is open by default and both MAX98357A are active. Closing the jumper bridge will connect pin
  GPIO47 (onboard LED IO47) with the amps SD_MODE pin. Setting GPIO47 to LOW (LED off) will shut down (mute) the amps
  and setting GPIO47 to HIGH (LED on) will activate the amps.

  Last updated 2024-10-15, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Audio.h"

#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://stream.laut.fm/oldies"

#define I2S_BCLK   5   // GPIO 5/6/7 are not wired to a pin, they are exclusively used for the MAX98357A
#define I2S_LRC    6
#define I2S_DOUT   7

#define TFT_CS    38   //  HSPI CS0
#define TFT_MOSI  39   //  HSPI D
#define TFT_SCLK  40   //  HSPI CLK
#define TFT_MISO  41   //  HSPI Q
#define TFT_DC    42
#define TFT_RST    1

const char ssid[] = "MySSID";
const char pass[] = "MyPassword"; 
 
// simple wrapper class to provide a "scrolling text" feature on the tft display,
// with scrolling only supported in portrait mode !
class myTFT : public Adafruit_ILI9341 {
public:
  myTFT(SPIClass *spiClass, int8_t dc, int8_t cs = -1, int8_t rst = -1) 
    : Adafruit_ILI9341(spiClass, dc, cs, rst) {}; // Constructor
  void printf(const char* format, ...)
  {
    char buf[300];
    static uint16_t dummy1, textHeight = 0;
    static int16_t dummy2, rotation, scrollValue = 0;
    static bool bScroll = false;

    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(buf, sizeof(buf), format, varArgs);
    va_end(varArgs);

    rotation = getRotation();
    if (rotation == 0 || rotation == 2) {
      if (wrap || (!wrap && !textHeight))
        getTextBounds(buf, 0, 0, &dummy2, &dummy2, &dummy1, &textHeight);
      if (getCursorY() + textHeight > height()) {
        setCursor(0, 0);
        bScroll = true;  // scrolling starts
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

SPIClass spi_tft = SPIClass(HSPI);      // second SPI bus (HSPI) used for TFT display
myTFT tft        = myTFT(&spi_tft, TFT_DC, TFT_CS, TFT_RST);

Audio audio;

void audio_info(const char *info)       // file info
{
  Serial.printf("info: %s\n", info); 
  tft.printf("i: %s\n", info);
}

void audio_id3data(const char *info)    // id3 metadata
{
  Serial.printf("id3data: %s\n", info);
  tft.printf("id3: %s\n", info);
}

void setup()
{
  // HSPI will work on any available GPIO (routed through GPIO matrix)
  spi_tft.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextWrap(true);               // text wrapping enabled
  tft.setTextSize(0);                  // smallest font
  tft.setRotation(2);                  // portrait mode upside down

  Serial.begin(115200);
  Serial.println();
  Serial.println("running example \"Play-Radio-Station-with-TFT\":");

  // connecting to local WiFi network
  Serial.printf("connecting to WiFi network \"%s\"\n", ssid);
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.printf("\nconnected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10);                 // 0...21(max)
  audio.setConnectionTimeout(1200,0);  // needed for some stations esp. from around the globe
  audio.connecttohost(RADIO_STREAM);
}

void loop()
{
  audio.loop();                        // play audio stream
}


