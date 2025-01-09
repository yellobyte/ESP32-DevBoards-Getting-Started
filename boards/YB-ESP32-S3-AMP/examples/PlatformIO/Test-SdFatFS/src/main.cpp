/*
  Test SdFatFS

  This example uses the fast SdFatFS library which reads a SD card in no time
  in order to create a playlist of all selected audio files found.
  
  Subsequently you can select the files to play, turn volume up/down, etc.

  Since we use "SD" library which requires the cards CS signal (GPIO10) the
  solder bridge SD_CS must be closed [default].

  Last updated 2025-01-09, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include "SD_SDFAT.h"
#include "Audio.h"  // Audio.h should included after SD_SDFAT to avoid compiler warnings
#include "SdFatPlayList.h"

// example tested with lib version from 13.12.2024!

#if CONFIG_IDF_TARGET_ESP32S3
    #define I2S_BCLK    5   // GPIOs 5/6/7 are not wired to a pin, they are exclusively used for the MAX98357A
    #define I2S_LRC     6
    #define I2S_DOUT    7
    SPIClass SD_SPI(FSPI);
#endif

#if CONFIG_IDF_TARGET_ESP32
    #define I2S_LRC     26
    #define I2S_DOUT    25
    #define I2S_BCLK    27
    SPIClass SD_SPI(VSPI);
#endif

Audio audio;
SdFatPlayList plist;
bool playNextFile(int offset = 1);
bool f_eof = true;
const uint8_t volume_steps = 21;
const uint8_t default_volume = 4;

const char *dir = "/";      // root dir for the playlist
int subdirLevels = 10;      // subdirLevels = 0 : add only files from dir to playlist. 
                            // subdirLevels > 0 : add files in dir files from all subdirs down to a depth of subdirLevels to the lis 

/**************************************************/

void setup() {
    Serial.begin(115200);
    if ( !SDF.begin(SdSpiConfig(SS, DEDICATED_SPI, SD_SCK_MHZ(25), &SD_SPI)) ) {
        log_e("Card Mount failed!");
        return;
    }
    
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolumeSteps(volume_steps);
    audio.setVolume(default_volume); // 0...21 Will need to add a volume setting in the app

    plist.setFileFilter( {"mp3", "ogg", "wav"} );
    uint32_t start = millis();
    uint32_t end = start;
    plist.createPlayList(dir, subdirLevels);
    end = millis()-start;
    log_w("read %d dirs with %d files in %lu ms", plist.dirs.size(), plist.files.size(), end);
    if (plist.files.empty()) {
        log_e("No files in playlist!");
        return;
    }
    f_eof = !playNextFile(0);   // play first file of playlist (index 0)
}

void loop() {
    int offset = 1;    
    char c = 0;
    static int cur_volume = default_volume;
    
    audio.loop();
    if (Serial.available()) {
        c = Serial.read();
        if ( !(c == '<' || c == '>') ) {
            String s(c);
            s += Serial.readString();       //SPACE -> next, ENTER -> repeat current song
                                            // or input positive or negative offset numbers to navigate in playlist                       
            if (s[0] != ' ')
                offset = s.toInt();
            audio.stopSong();
            f_eof = true;
        }
    }
    if (f_eof)
        f_eof = !playNextFile(offset);

    // volume control
    if  ( c == '<' || c == '>') { 
        cur_volume += (c == '>' ? 1 : -1);
        if (cur_volume < 0) cur_volume = 0;
        if (cur_volume > volume_steps) cur_volume = volume_steps; 
        audio.setVolume(cur_volume);
        log_i("Volume = %d", cur_volume);
    }
    vTaskDelay(1);
}

/****************************************************/

bool playNextFile(int offset) {
    static int cur_pos = 0;
    const char *file_path;
    if ( plist.files.size() ) {
        cur_pos = modulo(cur_pos += offset, plist.files.size());
        if ( (file_path = plist.getFileAtIdx(cur_pos)) != nullptr ) {
            if ( audio.connecttoFS(SDF, file_path) ) {
                log_i("\n****   now playing at [%d]: %s", cur_pos, file_path);
                return true;
            }
            else
                log_e("connectToSD failed: %s\n", file_path);
        }
    }
    return false;
}

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    // Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
    f_eof = true;
}
