# ESP32 Development Boards - Getting started

A big variety of **ESP32** development boards have flooded the market since the first ESP32 chip was released in 2016 by Espressif. Most boards contain the original **ESP32**, some an **ESP32-S2** and newer ones the most recent incarnation **ESP32-S3**. For differences between those chips look [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/doc/Comparison_Espressif-ESP32-S2-C3-S3.pdf).

The ESP32 chip family is loaded with features, e.g. WiFi/Bluetooth, dual core (up to 240MHz clock frequency), integrated Flash memory, lots of GPIO pins (peripheral input/output), ADC/DAC, UART, I2S, SPI, low power states, JTAG debugging option, etc. Have a look at the many documents in folder [**doc**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/doc) for more infos.

Almost all development boards feature a greater number of GPIO pins, a combination of WLAN antenna and/or IPX antenna connector, USB-Port, LDO voltage regulator and some LEDs (Power/RX/TX). Some boards have additional features, like camera (e.g. OV2640), RGB-LED (e.g. WS2812B), battery connector, more Flash memory (up to 16MB) and/or PSRAM (up to 8MB), etc.

Probably the best way to get familiar with a certain board is to write small software samples for trying out the different features, e.g. letting the onboard LED blink or change color (RGB-LED), generate serial output, connect to your local WLAN, read the status of a connected push button, put the ESP32 into sleep mode and so on.

## :file_folder: Examples

Subfolders in [**boards**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/boards) contain a collection of software examples I used over the last couple of years for playing with various boards.  

All examples were created and build with **VSCode/PlatformIO**. Make sure you have the latest **Arduino ESP32 development package** installed (V5.1.1 at the time of this writing).

For loading an example start VSCode/PlatformIO, go to "File"-->"Open Folder" and select an example directory (one that contains a platformio.ini file).

Examples are provided for the following boards:

- ESP32-DevKitC-1 (Manufacturer: Espressif)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKitC-V1_(Espressif)/doc/ESP32-Devkit-V1.jpg" height="180"/></p>  

- ESP32-CAM (Manufacturer: AI-THINKER)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM_(AI-THINKER)/doc/ESP32-CAM.jpg" height="180"/></p>

- Lilygo ESP32-1.14-Display DevBoard (Manufacturer: TTGO)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-1.14-Display-DevBoard_(TTGO)/doc/LILYGO-ESP32-1.14-Display.jpg" height="180"/></p>

- YD-ESP32-S3 Development board (Manufacturer: VCC-GND.COM)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YD-ESP32-S3_(VCC-GND.COM)/doc/YD-ESP32-S3_DevBoard.JPG" height="180"/></p>

## :hammer_and_wrench: Development platform VSCode/PlatformIO

VSCode/PlatformIO provides some major advantages over the ArduinoIDE:
  - very comfortable code Editor with lots of shortcuts that make writing code a breeze
  - usage of project file *platformio.ini* for controlling builds, adjusting serial monitor behaviour, enabling debugging, setting port parameters for software uploads (port, speed, RTS/CTS), etc.
  - easy search for/integration of additional libraries
  - integrated debugging functionality (additional external hardware required)
  - integrated git functionality
  - etc.

### :zap: Useful settings in platformio.ini


```c
build_flags = `-D__GNU_VISIBLE, -DUSE_ETHERNET`
```

### :information_source: Software Upload

tbw

### :information_source: Debugging your Software 

tbw