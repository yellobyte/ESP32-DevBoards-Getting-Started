/*
  Test-DAC-Sinus

  The ESP32 contains two 8-bit DAC output channels.
  DAC channel 1 is GPIO25 (Pin 25) and DAC channel 2 is GPIO26 (Pin 26).

  This sketch generates a ~1000Hz sinus signal with changing amplitude on 
  DAC channel 1 (Pin 25) using the integrated cosine waveform (CW) generator.

  Last updated 2022-06-08, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include "DacESP32.h"

DacESP32 dac1(GPIO_NUM_25);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  Serial.println();
  Serial.print("Sketch started. ~1000Hz Sinus signal on GPIO (Pin) number: ");
  Serial.println(GPIO_NUM_25);

  // output a sinus signal with frequency ~1000Hz and max. amplitude (default)
  dac1.outputCW(1000);
  // wait 5 secs before changing amplitude
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // change signal amplitude every second
  for (uint8_t i = 0; i < 4; i++) {
    delay(1000);
    if (i == 0)      
      dac1.setCwScale(DAC_CW_SCALE_1);
    else if (i == 1) 
      dac1.setCwScale(DAC_CW_SCALE_2);
    else if (i == 2)
      dac1.setCwScale(DAC_CW_SCALE_4);
    else if (i == 3)
      dac1.setCwScale(DAC_CW_SCALE_8);
  }
}
