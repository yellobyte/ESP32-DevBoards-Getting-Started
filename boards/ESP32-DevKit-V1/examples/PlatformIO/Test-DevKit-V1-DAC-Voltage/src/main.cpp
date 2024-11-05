/*
  Test-DAC-Voltage

  The ESP32 contains two 8-bit DAC output channels.
  DAC channel 1 is GPIO25 (Pin 25) and DAC channel 2 is GPIO26 (Pin 26).

  This sketch activates only DAC channel 1 and generates a sawtooth waveform
  with a very low frequency (~4Hz). 

  Last updated 2022-06-09, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include "DacESP32.h"

DacESP32 dac1(GPIO_NUM_25);

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.print("Sketch started. Voltage level changes on GPIO (Pin) number: ");
  Serial.println(GPIO_NUM_25);
}

void loop() {
  uint16_t level;
  
  // outputs the full 8-bit scale from 0 to 255
  for (level = 0; level < 256; level++) {
    dac1.outputVoltage((uint8_t)level);
    delay(1);
  }
}
