/*
  LED control

  Controlling the LED connected to GPIO2 on ESP32-DevKitC-V1 (Espressif).
  GPIO2 Output: High...LED on, Low...LED off.

  Detailed infos about ESP32 LED control is provided here:
  "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html".

  Last updated 2020-04-22, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>

#define LED_BUILTIN  2
#define ledPin       LED_BUILTIN

#define MODE      1                   // 0...simple on/off, 1...breathing, 2...PWM, 3...PWM (using analogWrite() wrapper function)

#define BRIGHT    350                 // max LED intensity
#define INHALE    1250                // cycle time in ms
#define PULSE     INHALE*1000/BRIGHT
#define REST      1300                // delay between cycles

#if MODE == 2
int ledChannel = 0;
#endif

void setup() {
  // put your setup code here, to run once:
#if MODE == 2
  // PWM
  int freq = 2000;        // Hz
  int resolution = 10;    // bit

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
#else
  pinMode(ledPin, OUTPUT);  
#endif

  Serial.begin(115200);
  Serial.print("ESP32-DevKitC-V1 board LED test started. MODE = ");
  Serial.println(MODE);
}

void loop() {
  // put your main code here, to run repeatedly:
#if MODE == 0
  // simple LED on/off
  Serial.println("LED off");
  digitalWrite(ledPin, LOW);             // turn LED off
  delay(1000);
  Serial.println("LED on");
  digitalWrite(ledPin, HIGH);            // turn LED on
  delay(1000);
#endif  
#if MODE == 1
  // increasing intensity
  Serial.println("inhaling");
  for (int i = 1; i < BRIGHT; i++){
    digitalWrite(ledPin, HIGH);          // turn LED on
    delayMicroseconds(i * 10);           // wait
    digitalWrite(ledPin, LOW);           // turn LED off
    delayMicroseconds(PULSE - i * 10);   // wait
  }
  // decreasing intensity
  Serial.println("exhaling");
  for (int j = BRIGHT - 1; j > 0; j--){
    digitalWrite(ledPin, HIGH);          // turn LED on
    delayMicroseconds(j * 10);           // wait
    digitalWrite(ledPin, LOW);           // turn LED off
    delayMicroseconds(PULSE - j * 10);   // wait
    j--;
  }
  delay(REST);                           // take a rest
#endif
#if MODE == 2
  // PWM (changing LED brightness using core functions)
  Serial.println("cycle started");
  ledcWrite(ledChannel, 100);            // duty cycle = 100/1023
  delay(2000);
  ledcWrite(ledChannel, 400);            // duty cycle = 400/1023
  delay(2000); 
  ledcWrite(ledChannel, 1023);           // duty cycle = 1023/1023
  delay(2000);
  ledcWrite(ledChannel, 0);              // duty Cycle = 0 (LED off)
  delay(3000);
#endif
#if MODE == 3
  // PWM (using framework wrapper function analogWrite() with f=1kHz, resolution=8bit)
  Serial.println("cycle started");
  analogWrite(ledPin, 20);               // duty cycle = 20/255
  delay(2000);
  analogWrite(ledPin, 100);              // duty cycle = 100/255
  delay(2000);
  analogWrite(ledPin, 255);              // duty cycle = 255/255
  delay(2000);
  analogWrite(ledPin, 0);                // duty cycle = 0/255 (LED off)
  delay(3000);
#endif
}

