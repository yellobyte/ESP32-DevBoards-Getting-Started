/*
  Test_WiFi

  Test WiFi functionality of ESP32-S3 DevBoard 1.9inch Display (Manufactorer LILYGO).

  Last updated 2022-10-12, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <WiFi.h>

// uncomment in case you want to know
//#define SHOW_ESP32_MEMORY_STATISTICS

const char ssid[] = "MySSID";
const char pass[] = "MyPassword"; 

const char *wifiAuthModes[] = {     
  "WIFI_AUTH_OPEN",             /* authenticate mode : open */
  "WIFI_AUTH_WEP",              /* authenticate mode : WEP */
  "WIFI_AUTH_WPA_PSK",          /* authenticate mode : WPA_PSK */
  "WIFI_AUTH_WPA2_PSK",         /* authenticate mode : WPA2_PSK */
  "WIFI_AUTH_WPA_WPA2_PSK",     /* authenticate mode : WPA_WPA2_PSK */
  "WIFI_AUTH_WPA2_ENTERPRISE",  /* authenticate mode : WPA2_ENTERPRISE */
  "WIFI_AUTH_WPA3_PSK",         /* authenticate mode : WPA3_PSK */
  "WIFI_AUTH_WPA2_WPA3_PSK",    /* authenticate mode : WPA2_WPA3_PSK */
  "WIFI_AUTH_WAPI_PSK",         /* authenticate mode : WAPI_PSK */
  ""
};

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000);  
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.print("scan done, ");
  if (n == 0) {
    Serial.println("no networks found.");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.printf("  %d: %s (%ddBm), Channel: %d, %s\n", 
                    i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i), wifiAuthModes[WiFi.encryptionType(i)]);
      delay(10);
    }
  }
  Serial.println("");
  
  // connect to local network via WiFi
  Serial.printf("Connecting to WiFi network \"%s\"\n", ssid);
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("Connected successfully to \"%s\". IP address: ", ssid);
  Serial.println(WiFi.localIP());
  Serial.println();

#ifdef SHOW_ESP32_MEMORY_STATISTICS
  Serial.println("Some ESP32 memory stats after running this sketch:");
  Serial.print(" 1) minimum ever free memory of all regions [in bytes]: ");
  Serial.println(ESP.getMinFreeHeap());
  Serial.print(" 2) minimum ever free heap size [in bytes]:             ");
  Serial.println(xPortGetMinimumEverFreeHeapSize());
  Serial.print(" 3) minimum ever stack size of this task [in bytes]:    ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL)); 
  Serial.println();
#endif

  Serial.println("Sketch finished.");
}

void loop() {
  // 
}
