# ESP32 Development Boards - Getting started

A big variety of **ESP32** development boards has flooded the market since the first ESP32 chip was released in 2016 by Espressif. Most boards contain the original **ESP32**, some an **ESP32-S2** and newer ones the most recent flavor **ESP32-S3**. For differences between those chips look [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/doc/Comparison_Espressif-ESP32-S2-C3-S3.pdf).

The ESP32 chip family is loaded with features, e.g. WiFi/Bluetooth, dual core (up to 240MHz clock frequency), integrated Flash memory, lots of GPIO pins (peripheral input/output), ADC/DAC, UART, I2S, SPI, low power states, JTAG debugging option, etc. Have a look at the many documents in folder [**doc**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/doc) for more infos.

Almost all development boards feature a greater number of GPIO pins, a combination of WLAN antenna and/or IPX antenna connector, USB-Port, LDO voltage regulator and some LEDs (Power/RX/TX). Some boards have additional features, like camera, RGB-LED, battery connector, more Flash memory and/or PSRAM, etc.

Probably the best way to get familiar with a certain board is to write small software samples for trying out the different features, e.g. letting the onboard LED blink or change color (RGB-LED), generate serial output, connect to a WLAN, read the status of a connected push button, etc.

## :file_folder: Examples

In folder [**boards**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/boards) you will find  a collection of software examples I used over the last couple of years for playing with the various boards.  

All examples were created and build with **VSCode/PlatformIO**. Make sure you have the latest **Arduino ESP32 development package** installed (V5.1.1 at the time of this writing).

For loading an example start VSCode/PlatformIO, go to "File"-->"Open Folder" and select an example directory (the ones that contain a platformio.ini file).

Examples are provided for the following boards:

- ESP32-DevKitC-1 (Manufacturer: Espressif)  
<img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKitC-1_(Espressif)/doc/ESP32-Devkit-V1.jpg" width="50"/>  

![<img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKitC-1_(Espressif)/doc/ESP32-Devkit-V1.jpg" width="70"/>](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKitC-1_(Espressif)/doc/ESP32-Devkit-V1.jpg)  

-![github](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKitC-1_(Espressif)/doc/ESP32-Devkit-V1.jpg)  
   

- ESP32-CAM (Manufacturer: AI-THINKER)
- Lilygo ESP32-1.14-Display DevBoard (Manufacturer: TTGO)
- YD-ESP32-S3 Development board (Manufacturer: VCC-GND.COM)  

## :hammer_and_wrench: Development platform VSCode/PlatformIO

VSCode/PlatformIO provides some major advantages over the ArduinoIDE:
  - very comfortable code Editor with lots of shortcuts that make writing code a breeze
  - usage of project file *platformio.ini* for controlling builds, adjusting serial monitor behaviour, enabling debugging, setting port parameters for software uploads (port, speed, RTS/CTS), etc.
  - easy integration of additional libraries
  - integrated debugging functionality (additional external hardware required)
  - integration of git functionality
  - etc.

### :zap: Useful settings in platformio.ini


```c
build_flags = `-D__GNU_VISIBLE, -DUSE_ETHERNET`
```

### :information_source: Software Upload

tbw

### :information_source: Debugging your Software 

tbw