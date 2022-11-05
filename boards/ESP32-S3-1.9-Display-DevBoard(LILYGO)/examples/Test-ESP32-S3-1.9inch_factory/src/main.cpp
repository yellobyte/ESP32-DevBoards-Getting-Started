/*
  Example "factory"

  Each distributed ESP32-S3 DevBoard with 1.9inch Display (Manufactorer LILYGO) is pre-programmed with software. 
  The original code is available on "https://github.com/Xinyuan-LilyGO/T-Display-S3/example/". The program simply 
  connects to WLAN, fetches the valid time from an internet time server and displays it in station clock style.
  
  This example uses above code with only minor modifications and adjustments for PlatformIO & latest ESP32 frameworks.

  Last updated 2022-11-05, ThJ <yellobyte@bluewin.ch>
*/

#include <Arduino.h>
#include <WiFi.h>
#include <sntp.h>
#include <time.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>
#include "pin_config.h"
//#include "lv_conf.h"
#include "lvgl.h"
#include "cstxx.h"
#include "factory_gui.h"
#include "OneButton.h"

#define WIFI_SSID          "MySSID"
#define WIFI_PASSWORLD     "MyPassword"

esp_lcd_panel_io_handle_t io_handle = NULL;
static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv;      // contains callback functions
static lv_color_t *lv_disp_buf;
static bool is_initialized_lvgl = false;
OneButton button1(PIN_BUTTON_1, true);
OneButton button2(PIN_BUTTON_2, true);
CSTXXX touch(0, 0, 170, 320);

bool inited_touch = false;

extern const unsigned char img_logo[20000];
void wifi_test(void);
void timeavailable(struct timeval *t);
void printLocalTime();
void SmartConfig();

static bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx) {
  if (is_initialized_lvgl) {
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
  }
  return false;
}

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map) {
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
  // copy a buffer's content to a specific area of the display
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

void setup() {
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  Serial.begin(115200);

  sntp_servermode_dhcp(1); // (optional)
  configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2);

  pinMode(PIN_LCD_RD, OUTPUT);
  digitalWrite(PIN_LCD_RD, HIGH);
  esp_lcd_i80_bus_handle_t i80_bus = NULL;
  esp_lcd_i80_bus_config_t bus_config = {
    .dc_gpio_num = PIN_LCD_DC,
    .wr_gpio_num = PIN_LCD_WR,
    .clk_src = LCD_CLK_SRC_PLL160M,
    .data_gpio_nums = {
      PIN_LCD_D0,
      PIN_LCD_D1,
      PIN_LCD_D2,
      PIN_LCD_D3,
      PIN_LCD_D4,
      PIN_LCD_D5,
      PIN_LCD_D6,
      PIN_LCD_D7,
    },
    .bus_width = 8,
    .max_transfer_bytes = LVGL_LCD_BUF_SIZE * sizeof(uint16_t),
  };
  esp_lcd_new_i80_bus(&bus_config, &i80_bus);

  esp_lcd_panel_io_i80_config_t io_config = {
    .cs_gpio_num = PIN_LCD_CS,
    .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
    .trans_queue_depth = 20,
    .on_color_trans_done = example_notify_lvgl_flush_ready,
    .user_ctx = &disp_drv,
    .lcd_cmd_bits = 8,
    .lcd_param_bits = 8,
    .dc_levels = {
      .dc_idle_level = 0,
      .dc_cmd_level = 0,
      .dc_dummy_level = 0,
      .dc_data_level = 1,
    },
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));
  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = PIN_LCD_RES,
      .color_space = ESP_LCD_COLOR_SPACE_RGB,
      .bits_per_pixel = 16,
  };
  esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle);
  esp_lcd_panel_reset(panel_handle);
  esp_lcd_panel_init(panel_handle);
  esp_lcd_panel_invert_color(panel_handle, true);

  esp_lcd_panel_swap_xy(panel_handle, true);
  esp_lcd_panel_mirror(panel_handle, false, true);
  // the gap is LCD panel specific, even panels with the same driver IC, can
  // have different gap value
  esp_lcd_panel_set_gap(panel_handle, 0, 35);

  /* Lighten the screen with gradient */
  ledcSetup(0, 10000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  for (uint8_t i = 0; i < 0xFF; i++) {
    ledcWrite(0, i);
    delay(2);
  }

  lv_init();
  lv_disp_buf = (lv_color_t *)heap_caps_malloc(LVGL_LCD_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

  lv_disp_draw_buf_init(&disp_buf, lv_disp_buf, NULL, LVGL_LCD_BUF_SIZE);
  /*Initialize the display*/
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = example_lvgl_flush_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = panel_handle;
  lv_disp_drv_register(&disp_drv);

  is_initialized_lvgl = true;

  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  inited_touch = touch.init(Wire, PIN_TOUCH_RES, PIN_TOUCH_INT);

  wifi_test();

  button1.attachClick([]() {
    pinMode(PIN_POWER_ON, OUTPUT);
    pinMode(PIN_LCD_BL, OUTPUT);
    digitalWrite(PIN_POWER_ON, LOW);
    digitalWrite(PIN_LCD_BL, LOW);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON_2, 0); // 1 = High, 0 = Low
    esp_deep_sleep_start();
  });
  
  button2.attachClick([]() { ui_switch_page(); });
}

void loop() {
  lv_timer_handler();
  button1.tick();
  button2.tick();
  delay(3);
  static uint32_t last_tick;
  if (millis() - last_tick > 100) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      lv_msg_send(MSG_NEW_HOUR, &timeinfo.tm_hour);
      lv_msg_send(MSG_NEW_MIN, &timeinfo.tm_min);
    }
    uint32_t volt = (analogRead(PIN_BAT_VOLT) * 2 * 3.3 * 1000) / 4096;
    lv_msg_send(MSG_NEW_VOLT, &volt);

    if (inited_touch) {
      touch_info_t t;
      String str_buf;
      static uint8_t last_finger;
      touch.get_touch_point(&t);
      str_buf += " Finger num :";
      str_buf += t.finger_num;
      str_buf += " \n";
      for (uint8_t i = 0; i < t.finger_num; i++) {
        str_buf += "x:";
        str_buf += t.point[i].x;
        str_buf += " y:";
        str_buf += t.point[i].y;
        str_buf += " p:";
        str_buf += t.point[i].pressure;
        str_buf += " \n";
      }
      lv_msg_send(MSG_NEW_TOUCH_POINT, str_buf.c_str());
    }
    last_tick = millis();
  }
}

LV_IMG_DECLARE(lilygo2_gif);

void wifi_test(void) {
  String text;
  lv_obj_t *logo_img = lv_gif_create(lv_scr_act());
  lv_obj_center(logo_img);
  lv_gif_set_src(logo_img, &lilygo2_gif);
  LV_DELAY(1200);
  lv_obj_del(logo_img);

  lv_obj_t *log_label = lv_label_create(lv_scr_act());
  lv_obj_align(log_label, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_set_width(log_label, LV_PCT(100));
  lv_label_set_text(log_label, "Scan WiFi");
  lv_label_set_long_mode(log_label, LV_LABEL_LONG_SCROLL);
  lv_label_set_recolor(log_label, true);
  LV_DELAY(1);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    text = "no networks found";
  } else {
    text = n;
    text += " networks found\n";
    for (int i = 0; i < n; ++i) {
      text += (i + 1);
      text += ": ";
      text += WiFi.SSID(i);
      text += " (";
      text += WiFi.RSSI(i);
      text += ")";
      text += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " \n" : "*\n";
      delay(10);
    }
  }
  lv_label_set_text(log_label, text.c_str());
  Serial.println(text);
  LV_DELAY(2000);
  text = "Connecting to ";
  Serial.print("Connecting to ");
  text += WIFI_SSID;
  text += "\n";
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORLD);
  uint32_t last_tick = millis();
  uint32_t i = 0;
  bool is_smartconfig_connect = false;
  lv_label_set_long_mode(log_label, LV_LABEL_LONG_WRAP);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    text += ".";
    lv_label_set_text(log_label, text.c_str());
    LV_DELAY(100);
    if (millis() - last_tick > WIFI_CONNECT_WAIT_MAX) { /* Automatically start smartconfig when connection times out */
      text += "\nConnection timed out, start smartconfig";
      lv_label_set_text(log_label, text.c_str());
      LV_DELAY(100);
      is_smartconfig_connect = true;
      WiFi.mode(WIFI_AP_STA);
      Serial.println("\r\n wait for smartconfig....");
      text += "\r\n wait for smartconfig....";
      text += "\nPlease use #ff0000 EspTouch# Apps to connect to the distribution network";
      lv_label_set_text(log_label, text.c_str());
      WiFi.beginSmartConfig();
      while (1) {
        LV_DELAY(100);
        if (WiFi.smartConfigDone()) {
          Serial.println("\r\nSmartConfig Success\r\n");
          Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
          Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
          text += "\nSmartConfig Success";
          text += "\nSSID:";
          text += WiFi.SSID().c_str();
          text += "\nPSW:";
          text += WiFi.psk().c_str();
          lv_label_set_text(log_label, text.c_str());
          LV_DELAY(1000);
          last_tick = millis();
          break;
        }
      }
    }
  }
  if (!is_smartconfig_connect) {
    text += "\nCONNECTED \nTakes ";
    Serial.print("\n CONNECTED \nTakes ");
    text += millis() - last_tick;
    Serial.print(millis() - last_tick);
    text += " ms\n";
    Serial.println(" millseconds");
    lv_label_set_text(log_label, text.c_str());
  }
  LV_DELAY(2000);
  ui_begin();
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
  WiFi.disconnect();
}
