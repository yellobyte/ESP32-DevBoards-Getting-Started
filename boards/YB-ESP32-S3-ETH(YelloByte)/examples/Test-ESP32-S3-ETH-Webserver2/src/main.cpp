/*
  Test-Webserver2

  The sketch runs a Webserver listening on the RJ45 Ethernet port. 
  Built with ESP-IDF instead of Arduino Ethernet lib.

  IMPORTANT:  
  Utilizes W5500 interrupts, therefore solder bridge "INT" on bottom of dev boeard must be closed!

  Last updated 2023-06-18, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <WiFi.h>
#include <ETH.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "driver/spi_master.h"
#include "driver/spi_common_internal.h"

#define GPIO_STATUS_LED  47                        // onboard status LED connected to GPIO47

// Ethernet settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//#define ETH_STATIC_IP                            // Ethernet IP assigned manually and not via DHCP
#ifdef ETH_STATIC_IP
#define ETH_IP        "192.168.1.55"
#define ETH_NETMASK   "255.255.255.0"
#define ETH_GW        "192.168.1.1"
#endif

// GPIO settings for W5500 
#define GPIO_W5500_CS    14                        // onboard W5500 CS chip select pin connected to GPIO14
#define GPIO_W5500_INT   18                        // onboard W5500 INT pin connected to GPIO18
//#define GPIO_W5500_RST   21                        // onboard W5500 RST reset pin connected to GPIO21, not used here
#define GPIO_W5500_MOSI  11                        // SPI bus pins
#define GPIO_W5500_MISO  13                        //
#define GPIO_W5500_SCLK  12                        //

AsyncWebServer server(80);
bool ethOk = false;

// Event handler for Ethernet events
static void eth_event_handler(void *arg, esp_event_base_t eventBase, int32_t eventId, void *eventData)
{
  uint8_t macAddr[6] = {0};
  esp_eth_handle_t ethHandle = *(esp_eth_handle_t *)eventData;

  switch (eventId) {
    case ETHERNET_EVENT_CONNECTED:
      esp_eth_ioctl(ethHandle, ETH_CMD_G_MAC_ADDR, macAddr);
      Serial.printf("Ethernet Link Up, HW Addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
                      macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
      break;
    case ETHERNET_EVENT_DISCONNECTED:
      Serial.println("Ethernet link down");
      break;
    case ETHERNET_EVENT_START:
      Serial.println("Ethernet started");
      break;
    case ETHERNET_EVENT_STOP:
      Serial.println("Ethernet stopped");
      break;
    default:
      break;
  }
}

// Event handler for IP_EVENT_ETH_GOT_IP
static void got_ip_event_handler(void *arg, esp_event_base_t eventBase, int32_t eventId, void *eventData)
{
  ip_event_got_ip_t *event = (ip_event_got_ip_t *)eventData;
  const esp_netif_ip_info_t *ipInfo = &event->ip_info;

  Serial.printf("Ethernet IP: %d.%d.%d.%d, ", IP2STR(&ipInfo->ip));
  Serial.printf("Mask: %d.%d.%d.%d, ", IP2STR(&ipInfo->netmask));
  Serial.printf("GW: %d.%d.%d.%d\n", IP2STR(&ipInfo->gw));

  ethOk = true;
  digitalWrite(GPIO_STATUS_LED, HIGH);              // LED on
  // http init 
}

void setup() {                       
  pinMode(GPIO_STATUS_LED, OUTPUT);
  digitalWrite(GPIO_STATUS_LED, LOW);              // LED off
	
  Serial.begin(115200);
  // port 'USB' (directly attached to ESP32-S3 chip !) will be gone for a few seconds after resetting the board, 
  // if you dislike it you better direct serial output to port 'UART' (ARDUINO_USB_CDC_ON_BOOT=0 in platformio.ini)  
#if ARDUINO_USB_CDC_ON_BOOT == 1
  // we continue only when serial port becomes available: important when serial output is directed to port 'USB'
  while (!Serial);
  delay(100);
#endif	

  WiFi.begin();                                    // initializes needed network settings
  WiFi.mode(WIFI_OFF);                             // WiFi not used any further
  Serial.println();
  Serial.println("Initializing Ethernet...");

  // Initialize TCP/IP network interface
  ESP_ERROR_CHECK(esp_netif_init());

  // Create instance(s) of esp-netif for SPI Ethernet
  //esp_netif_config_t netifConfig = ESP_NETIF_DEFAULT_ETH();
  esp_netif_inherent_config_t espNetifConfig = ESP_NETIF_INHERENT_DEFAULT_ETH();
  esp_netif_config_t netifConfig = {
      .base = &espNetifConfig,
      .stack = ESP_NETIF_NETSTACK_DEFAULT_ETH
    };
  espNetifConfig.if_key = "ETH_SPI"; 
  espNetifConfig.route_prio = 30;
  esp_netif_t *ethNetif = esp_netif_new(&netifConfig);

#ifdef ETH_STATIC_IP
  // setting static Ethernet address
  const char *ip =      ETH_IP,
             *mask =    ETH_NETMASK,
             *gateway = ETH_GW;
  esp_netif_ip_info_t ipInfo;

  ESP_ERROR_CHECK(esp_netif_dhcpc_stop(ethNetif));
  memset(&ipInfo, 0, sizeof(esp_netif_ip_info_t));
  ipInfo.ip.addr = esp_ip4addr_aton(ip);
  ipInfo.netmask.addr = esp_ip4addr_aton(mask);
  ipInfo.gw.addr = esp_ip4addr_aton(gateway);
  if (!esp_netif_set_ip_info(ethNetif, &ipInfo))
      Serial.println("Setting static IP ok");
  else
      Serial.println("Setting static IP error");
#endif

  esp_eth_mac_t *ethMac = NULL;
  esp_eth_phy_t *ethPhy = NULL;
  gpio_install_isr_service(0);

  spi_bus_config_t busConfig = {
    // SPI bus signals
    .mosi_io_num = GPIO_W5500_MOSI, .miso_io_num = GPIO_W5500_MISO, .sclk_io_num = GPIO_W5500_SCLK,
    // SPI control signals 'write protect' & 'hold' are not used
    .quadwp_io_num = -1, .quadhd_io_num = -1,
  };
  // select available SPI peripheral
  int host = 0;
  for (; host <= (int)SPI3_HOST; host++ ) {
    if (spi_bus_get_attr((spi_host_device_t)host) == NULL) break;
  }
  ESP_ERROR_CHECK(spi_bus_initialize((spi_host_device_t)host, &busConfig, SPI_DMA_CH_AUTO));

  spi_device_handle_t spiHandle = NULL;
  spi_device_interface_config_t devConfig = {
      .command_bits = 16,   // Address phase in W5500 SPI frame
      .address_bits = 8,    // Control phase in W5500 SPI frame
      .mode = 0,
      .clock_speed_hz = 12 * 1000 * 1000,
      .spics_io_num = GPIO_W5500_CS,
      .queue_size = 20
  };
  ESP_ERROR_CHECK(spi_bus_add_device((spi_host_device_t)host, &devConfig, &spiHandle));

  // W5500 ethernet driver uses SPI driver
  eth_w5500_config_t w5500Config = ETH_W5500_DEFAULT_CONFIG(spiHandle);
  w5500Config.int_gpio_num = GPIO_W5500_INT;

  eth_mac_config_t macConfig = ETH_MAC_DEFAULT_CONFIG();
  eth_phy_config_t phyConfig = ETH_PHY_DEFAULT_CONFIG();
  phyConfig.reset_gpio_num = -1;

  if((ethMac = esp_eth_mac_new_w5500(&w5500Config, &macConfig)) == NULL) {
    log_e("esp_eth_mac_new_esp32() failed");
    while(true);
  }

  if((ethPhy = esp_eth_phy_new_w5500(&phyConfig)) == NULL) {
    log_e("esp_eth_phy_new() failed");
    while(true);
  }
    
  esp_eth_config_t ethConfig = ETH_DEFAULT_CONFIG(ethMac, ethPhy);
  esp_eth_handle_t ethHandle = NULL;
  ESP_ERROR_CHECK(esp_eth_driver_install(&ethConfig, &ethHandle));
  ESP_ERROR_CHECK(esp_eth_ioctl(ethHandle, ETH_CMD_S_MAC_ADDR, mac));

  // attach Ethernet driver to TCP/IP stack
  ESP_ERROR_CHECK(esp_netif_attach(ethNetif, esp_eth_new_netif_glue(ethHandle)));

  // register user defined event handlers
  ESP_ERROR_CHECK(esp_event_handler_instance_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL, NULL));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL, NULL));

  // start Ethernet driver state machine
  ESP_ERROR_CHECK(esp_eth_start(ethHandle));

#ifndef ETH_STATIC_IP
  Serial.println("Waiting for IP address...");
#endif
  while (!ethOk) delay(1);                            // delay() yields processing time

  // start webserver
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    static int count = 1;
    char buffer[70];
    
    digitalWrite(GPIO_STATUS_LED, !digitalRead(GPIO_STATUS_LED));   // toggle status LED
    snprintf(buffer, sizeof(buffer), "Hello World %d. Status LED is %s. Please refresh page.", 
               count, digitalRead(GPIO_STATUS_LED) ? "ON" : "OFF");
    request->send(200, "text/plain", buffer);
    Serial.print("Client request received ");
    Serial.println(count++);
  });
  server.onNotFound([](AsyncWebServerRequest *request) { 
    request->send(404, "text/plain", "Not found");
  });
  server.begin();
  Serial.println("Waiting for client...");
}

void loop(){
  //
}
