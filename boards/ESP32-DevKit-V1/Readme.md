## ESP32-DevKit-V1 Development Board Overview:

The **ESP32-DevKit-V1** development board is a low-footprint and entry-level development board that is part of Espressif's ESP32 DevKit series and has been available for some years now.  

Lots of almost identical China clones (2x15pin, 2x18pin, 2x19pin) for small money can be bought everywhere. If you want to get in touch with the ESP32 universe then this is the board to start with.

It features an ESP-WROOM-32 module with 4MB Flash and no PSRAM, two buttons (BOOT and EN/RESET), two LEDs (power + GPIO2), one LDO (e.g. AMS1117-3.3, NCP1117-3.3, etc.) to convert VIN to 3.3 Volts, one microUSB port and lots of available GPIOs (general purpose input/output pins).

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKit-V1/doc/ESP32-Devkit-V1.jpg" height="160"/></p>  

Overview of Espressif's latest ESP32 dev kit boards: "https://www.espressif.com/en/products/devkits/esp32-devkitc/overview".

## Board Pin Layout:
![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKit-V1/doc/ESP32-DevKit-V1-Pinout.jpg)

## Application hints:

The board (usually) uses a Silicon Labs CP2102 bridge chip between microUSB port and the ESP32 so you need to install the CP210x Drivers on your Laptop/PC. For Windows they are available [here](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads).  

### ArduinoIDE:

Select the board "**ESP32 Dev Module**" and choose the proper settings as shown below.

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKit-V1/doc/ESP32-DevKit-V1-ArduinoIDE-Settings.jpg)

### PlatformIO:

Make sure, you have the following settings in the [env:xxx] section of your projects platformio.ini file:

```
platform = espressif32
board = esp32dev
framework = arduino
....
```
### Software Upload to the board:

The board contains the usual ESP32 reset and upload circuitry which makes automatic uploading new software to the board with your IDE a breeze. 

However, with brandnew and previously unflashed boards the upload might fail and you will only see the following after powering up the board:

```
--- Terminal on COM7 | 115200 8-N-1
--- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
02:06:58.327 > e[     5][D][esp32-hal-cpu.c:244] setCpuFrequencyMhz(): PLL: 480 / 2 = 240 Mhz, APB: 80000000 Hz
02:06:58.329 > invalid header: 0xffffffff
02:06:58.333 > invalid header: 0xffffffff
....
```
In such case **put the ESP32 into upload mode manually** by keeping the **'BOOT'** button pressed, then pressing/releasing the **'EN'** button and finally releasing the **'BOOT'** button. The serial monitor output will subsequently confirm the boards readiness:  
```
....
02:10:49.339 > ets Jun  8 2016 00:22:57
02:10:49.355 >
02:10:49.356 > rst:0x1 (POWERON_RESET),boot:0x3 (DOWNLOAD_BOOT(UART0/UART1/SDIO_REI_REO_V2))
02:10:49.356 > waiting for download
```
