// ESP-32 Dev Board (China)
// Die rote LED nahe dem CP2102 wird über GPIO2 angesteuert 
// HIGH=An, LOW=Aus, Achtung: Auf dem ESP-8266 ist es genau umgekehrt!

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <esp_ota_ops.h>

#define LED_BUILTIN 2
#define ledPin  LED_BUILTIN

#define BREATHING
//#define PWM

#define BRIGHT    350     //max led intensity (1-500)
#define INHALE    1250    //Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      1000    //Rest Between Inhalations.

#ifdef PWM
  int ledChannel = 0;
#endif

int j;
static uint32_t l = 0;    // Doesn't show under "Variables" while debugging but can be selected under "Watch"

void setup() {
  // put your setup code here, to run once:
#ifdef PWM
  int freq = 2000;
  int resolution = 10;

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
#endif
#ifndef PWM
  pinMode(ledPin, OUTPUT);  
#endif
  Serial.begin(115200);
  yield();
  Serial.println("ESP32 Dev Board LED Test");
  const esp_partition_t *configured = esp_ota_get_boot_partition();
  const esp_partition_t *running = esp_ota_get_running_partition();
	/*if (configured != running) {
			ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
							 configured->address, running->address);
			ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
	}*/
	Serial.printf("Boot partition type %d subtype %d (offset 0x%08x)\n", configured->type, configured->subtype, configured->address);
	Serial.printf("Running partition type %d subtype %d (offset 0x%08x)", running->type, running->subtype, running->address);
}

void loop() {
  volatile int i;                        // otherwise it's optimized out, -O0 would help too
  static uint32_t k = 0;
  // put your main code here, to run repeatedly:
#ifdef BREATHING
  //ramp increasing intensity, Inhalation: 
  for (i=1;i<BRIGHT;i++){
    digitalWrite(ledPin, HIGH);          // turn the LED on.
    delayMicroseconds(i*10);             // wait
    digitalWrite(ledPin, LOW);           // turn the LED off.
    delayMicroseconds(PULSE-i*10);       // wait
    delay(0);                            //to prevent watchdog firing.
    l++;
  }
  //ramp decreasing intensity, Exhalation (half time):
  for (j=BRIGHT-1;j>0;j--){
    digitalWrite(ledPin, HIGH);          // turn the LED on.
    delayMicroseconds(j*10);             // wait
    digitalWrite(ledPin, LOW);           // turn the LED off.
    delayMicroseconds(PULSE-j*10);       // wait
    j--;
    delay(0);                            //to prevent watchdog firing.
    l++;
  }
  k++;
  delay(REST);                           //take a rest...
#elif defined PWM
  ledcWrite(ledChannel, 100); // Duty Cycle = 100
  delay(1000);                // wait for a while...Wifi wird im Hintergrund erledigt
  ledcWrite(ledChannel, 400); // Duty Cycle = 400
  delay(1000);                // wait for a while...Wifi wird im Hintergrund erledigt
  ledcWrite(ledChannel, 1000); // Duty Cycle = 1000
  delay(1000);                // wait for a while...Wifi wird im Hintergrund erledigt
  ledcWrite(ledChannel, 0);   // Duty Cycle = 0
  delay(1000);                  // wait for a while...Wifi wird im Hintergrund erledigt
  /* analogWrite() gibt es (noch?) nicht in der ESP-32 Umgebung 
  analogWrite(ledPin, 200);      // Im Gegensatz zum Arduino ist der Range hier 0...1023 !!
  delay(1000);                  // wait for a while...Wifi wird im Hintergrund erledigt
  analogWrite(ledPin, 700);
  delay(500);                  // wait for a while...Wifi wird im Hintergrund erledigt
  analogWrite(ledPin, 1000);
  delay(500);                   // wait for a while...Wifi wird im Hintergrund erledigt
  analogWrite(ledPin, 0);       // the only way to switch PWM Off (but LED is on now)
  digitalWrite(ledPin,LOW);    // turn LED off
  delay(2000);
  */
#else
  digitalWrite(ledPin, LOW);   // turn the LED off
  delay(1600);              // wait for a while...Wifi wird im Hintergrund erledigt
  //Serial.println("LED Off");
  //yield();                // wegen delay(..) nicht nötig !!
  for ( int i = 0; i < 3; i++ )
  {
    digitalWrite(ledPin, HIGH);    // turn the LED on
    delay(180);
    digitalWrite(ledPin, LOW);   // turn the LED off
    delay(180);
  }
  delay(200);
  for ( int i = 0; i < 3; i++ )
  {
    digitalWrite(ledPin, HIGH);    // turn the LED on
    delay(600);
    digitalWrite(ledPin, LOW);   // turn the LED off
    delay(200);
  }
  delay(170);
  for ( int i = 0; i < 3; i++ )
  {
    digitalWrite(ledPin, HIGH);    // turn the LED on
    delay(180);
    digitalWrite(ledPin, LOW);   // turn the LED off
    delay(180);
  }
  //Serial.println("LED On");
  //delay(100);              // wait for a while...Wifi wird im Hintergrund erledigt
#endif  
}

