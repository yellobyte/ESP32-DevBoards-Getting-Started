/**
 * @file player-sdfat-i2s.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-player/player-sdfat-i2s/README.md
 *
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

// Pin settings & some adjustments to fit the YB-ESP32-S3-AMP dev board courtesy of https://github.com/anp59.

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioSourceSDFAT.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
  // #define PIN_CS      10
  // #define SPI_MOSI    11     // SD-Card MOSI
  // #define SPI_SCK     12
  // #define SPI_MISO    13     // SD-Card MISO
  #define I2S_BCLK    5   // GPIOs 5/6/7 are not wired to a pin, they are exclusively used for the MAX98357A
  #define I2S_LRC     6
  #define I2S_DOUT    7
#elif defined CONFIG_IDF_TARGET_ESP32
  #define SD_CS       5
  #define SPI_MOSI    23 // SD-Card MOSI
  #define SPI_SCK     18
  #define SPI_MISO    19 // SD-Card MISO
  #define I2S_LRC     26
  #define I2S_DOUT    25
  #define I2S_BCLK    27
#endif

const char *startFilePath = "/";
const char *ext = "mp3";
AudioSourceSDFAT source(startFilePath, ext);
I2SStream i2s;
MP3DecoderHelix decoder;
AudioPlayer player(source, i2s, decoder);

void printMetaData(MetaDataType type, const char *str, int len) {
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}

void setup() {
  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

  // setup output
  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.pin_ws = I2S_LRC;
  cfg.pin_data = I2S_DOUT;    
  cfg.pin_bck = I2S_BCLK;
  i2s.begin(cfg);

  player.setMetadataCallback(printMetaData);
  player.setAutoFade(false);
  player.setSilenceOnInactive(false);
  player.setVolume(0.2); 
  player.begin();
}

void loop() {
  player.copy();
    if (Serial.available()) {
    String r = Serial.readString();
    r.trim();
    if ( r[0] == 'x' || r[0] == 'y') { 
      player.setVolume(player.volume() + (r[0] == 'x' ? 0.1 : -0.1));
      log_i("PlayerVolume %.2f", player.volume());
    }
    else {
      player.next(r.toInt());   // input numbers nn: next (+nn) or previous (-nn) song, other input:  current song starts again
    }
  }
}