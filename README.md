# ESP32 Development Boards - Getting started

A big variety of **ESP32** development boards have flooded the market since the first ESP32 chip was released in 2016 by Espressif. Most boards contain the original **ESP32**, some an **ESP32-S2** and newer ones the most recent incarnation **ESP32-S3**. For differences between those chips look [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/doc/Comparison_Espressif-ESP32-S2-C3-S3.pdf).

The ESP32 chip family is loaded with features, e.g. WiFi/Bluetooth, dual core (up to 240MHz clock frequency), integrated Flash memory, lots of GPIO pins (peripheral input/output), ADC/DAC, UART, I2S, SPI, low power states, JTAG debugging option, etc. Have a look at the many documents in folder [**doc**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/doc) for ESP32 datasheets.

Almost all development boards feature a greater number of GPIO pins, a combination of WLAN antenna and/or IPX antenna connector, USB-Port, LDO voltage regulator and some LEDs (Power/RX/TX). Some boards have additional features, like a camera (e.g. OV2640), RGB-LED (e.g. WS2812B), battery connector, more Flash memory (up to 16MB) and/or PSRAM (up to 8MB), etc.

Probably the best way to get familiar with a certain board is to start with writing tiny software programs (examples) for trying out the different board features, e.g. letting the onboard LED blink or change color (RGB-LED), generate serial output, connect to your local WLAN, read the status of a connected push button, put the ESP32 into sleep mode, create a file on an attached SD-Card, etc. You get the idea.  

BTW: Arduino uses the name sketch (file extension *.ino) for a program. 

## :file_folder: Software Examples

Under [**boards**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/boards) you find software examples (programs) I used over the last couple of years for playing with & exploring various boards.  

All examples were created and build with **VSCode/PlatformIO**. Each example has it's own project directory (one that contains a platformio.ini file). Make sure you have the latest **Arduino ESP32 development package** installed (V5.1.1 at the time of this writing).

For editing (and later uploading) an example you need to start VSCode/PlatformIO, go to "File"-->"Open Folder" and select it's project directory.

Examples are provided for the following boards:

- ESP32-DevKitC-1 (Manufacturer: Espressif)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKitC-V1_(Espressif)/doc/ESP32-Devkit-V1.jpg" height="180"/></p>  

- ESP32-CAM, ESP32-CAM-CH340, ESP32-CAM-MB (Manufacturer: AI-THINKER)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM_(AI-THINKER)/doc/ESP32-CAM.jpg" height="180"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM_(AI-THINKER)/doc/ESP32-CAM-CH340.jpg" height="180"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM_(AI-THINKER)/doc/ESP32-CAM-MB.jpg" height="180"/></p>

- Lilygo ESP32-1.14-Display DevBoard (Manufacturer: TTGO)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-1.14-Display-DevBoard_(TTGO)/doc/LILYGO-ESP32-1.14-Display.jpg" height="180"/></p>

- YD-ESP32-S3 Development board (Manufacturer: VCC-GND.COM)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YD-ESP32-S3_(VCC-GND.COM)/doc/YD-ESP32-S3_DevBoard.JPG" height="180"/></p>

## :hammer_and_wrench: Development platform VSCode/PlatformIO

Arduino IDE is easy for beginners, simple but somewhat limited in functionality. In contrast PlatformIO is a much more powerful and versatile alternative which makes every task a breeze after getting familiar with it. Then you won't look for another IDE anymore.

VSCode/PlatformIO provides some major advantages over the Arduino IDE:
  - very comfortable and mighty code Editor with lots of shortcuts & integrated Intellisense features that make writing code a breeze
  - usage of project file *platformio.ini* for controlling builds, adjusting serial monitor behaviour, enabling debugging, setting port parameters for software uploads (port, speed, RTS/CTS), etc.
  - easy search for/integration of additional libraries
  - integrated debugging functionality (additional external hardware required)
  - integrated git functionality
  - etc.

### :zap: Useful settings in platformio.ini

The project configuration file has sections (each denoted by a [header]) and one or multiple key/value pairs within the sections. Lines beginning with ";" are ignored and may be used to provide comments. The file platformio.ini is explained in detail [here](https://docs.platformio.org/en/stable/projectconf/index.html). 

Some very useful settings deserve to be mentioned here. 
1) Forces builds to use older frameworks and platform packages instead of the newest one.
```c 
;platform = espressif32
platform = espressif32 @ 4.2.0
platform_packages = framework-arduinoespressif32 @ https://github.com/espressif/arduino-esp32/releases/download/2.0.3/esp32-2.0.3.zip
```
2) Serial output lines carry time stamps, exceptions get decoded with function backtrace and output gets logged to a file:
```c
monitor_filters = time, esp32_exception_decoder, log2file
```
3) The program is compiled with full symbolic debug information and no optimization.
```c
build_type = debug
```
4) Needed for usage of ESP32 JTAG Debug board "[ESP32-Prog](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/doc/ESP-Prog.jpg)".
```c
debug_tool = esp-prog
```
5) Sets the initial breakpoint to first line in function setup().
```c
debug_init_break = tbreak setup
```
6) Generates various levels of debug output.
```c
; 0 no debug output, 1 errors, 2 warnings, 3 infos, 4 debug info, 5 verbose 
build_flags = -DCORE_DEBUG_LEVEL=4
```
7) Disables any activity on COM signals RTS and/or DTR. Required by some ESP32 dev boards to enable serial output (e.g. ESP32-CAM-MB board). However, this will prevent a reboot triggered by the Development IDE.
```c
monitor_rts = 0
monitor_dtr = 0
```
8) Switch between various built-in tables. A big collection of predefined ones can be found [here](https://github.com/espressif/arduino-esp32/tree/master/tools/partitions).
```c
;board_build.partitions = min_spiffs.csv
board_build.partitions = no_ota.csv
```
9) Your own "partitions_custom.csv" in the root of the project directory.
```c
board_build.partitions = partitions_custom.csv
```
10) Collection of personal tasks, to be found under PlatformIO -> PROJECT TASKS -> Custom. Eventual paths are relative to the project folder.
```c
extra_scripts = add_tasks.py
```

### :information_source: Software Upload

tbw

### :information_source: Debugging your Software 

tbw