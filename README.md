# ESP32 Development Boards - Getting started

A big variety of **ESP32** development boards (from Espressif and lots of other companies) have flooded the markets since the first ESP32 chip was released in 2016 by Espressif. Most boards contain the original **ESP32**, some an **ESP32-S2** and newer ones the most recent flavour **ESP32-S3**. For differences between those chips look [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/ESP32_specs_and_manuals/Comparison_Espressif-ESP32-S2-C3-S3.pdf).

The ESP32 chip family is loaded with features, e.g. WiFi/Bluetooth, dual core (up to 240MHz clock frequency), integrated Flash memory, lots of GPIO pins (peripheral input/output), ADC/DAC, UART, I2S, SPI, low power states, JTAG debugging option, etc. Have a look at the many documents in folder [**ESP32_specs_and_manuals**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/ESP32_specs_and_manuals) for various ESP32 datasheets.

Almost all development boards feature a greater number of GPIO pins, a combination of WLAN antenna and/or IPX antenna connector, USB-Port, LDO voltage regulator and some LEDs (Power/RX/TX). Some boards have additional features, like TFT display, camera (e.g. OV2640), RGB-LED (e.g. WS2812B), battery connector, more Flash memory (up to 16MB) and/or PSRAM (up to 8MB), etc.

Probably the best way to get familiar with a certain board is writing a few tiny software programs (examples) for it. A good start would be to let the onboard LED blink or change color (RGB-LED), generate a few lines serial output, connect to the local WLAN, read the status of a connected push button, put the ESP32 into sleep mode, create a single file on an attached SD-Card or some other simple task. You get the idea.  

BTW: Arduino uses the name **sketch** (file extension *.ino) for a program. 

## :file_folder: Software Examples

Under [**boards**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards) you find a (growing) collection of programs for each board listed below. They will help you explore the features of those boards.

All examples were created and build with **VSCode/PlatformIO**. Each example has it's own project directory (one that contains a platformio.ini file). Make sure you have the latest **Arduino ESP32 development package** installed (V5.2.0 at the time of this writing).

For loading, editing and uploading an example just start VSCode/PlatformIO, go to "File"-->"Open Folder" and select the example's project directory.

Right now examples are provided for the following boards:

- [ESP32-DevKit-V1](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-DevKit-V1(Espressif)) (orig. Manufacturer: Espressif) and it's various clones (e.g. from DOIT, etc)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKit-V1(Espressif)/doc/ESP32-Devkit-V1.jpg" height="160"/></p>  

- [ESP32-CAM, ESP32-CAM-USB and ESP32-CAM-MB](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-CAM(AI-THINKER)) (orig. Manufacturer: AI-THINKER)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM(AI-THINKER)/doc/ESP32-CAM.jpg" height="160"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM(AI-THINKER)/doc/ESP32-CAM-USB.jpg" height="150"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM(AI-THINKER)/doc/ESP32-CAM-MB.jpg" height="160"/></p>

- [ESP32-1.14-Display DevBoard](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-1.14-Display-DevBoard(TTGO)) (orig. Manufacturer: TTGO)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-1.14-Display-DevBoard(TTGO)/doc/LILYGO-ESP32-1.14-Display.jpg" height="160"/></p>

- [ESP32-S3-0.85-Display DevBoard](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-S3-0.85-Display-DevBoard(LILYGO)) (orig. Manufacturer: LILYGO)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-S3-0.85-Display-DevBoard(LILYGO)/doc/LILYGO-ESP32-S3-0.85-Display.jpg" height="160"/></p>

- [ESP32-S3-1.9-Display DevBoard](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-S3-1.9-Display-DevBoard(LILYGO)) (orig. Manufacturer: LILYGO)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-S3-1.9-Display-DevBoard(LILYGO)/doc/LILYGO-ESP32-S3-1.9-Display.jpg" height="160"/></p>

- [YD-ESP32-S3 DevBoard with big Flash and external PSRAM](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YD-ESP32-S3(VCC-GND.COM)) (orig. Manufacturer: VCC-GND.COM)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YD-ESP32-S3(VCC-GND.COM)/doc/YD-ESP32-S3_DevBoard.JPG" height="160"/></p>

## :hammer_and_wrench: Development platform VSCode/PlatformIO

Arduino IDE is easy for beginners, simple but somewhat limited in functionality. In contrast PlatformIO is a much more powerful and versatile alternative which makes every task a breeze after getting familiar with it. 

VSCode/PlatformIO provides some major advantages over the Arduino IDE:
  - very comfortable and mighty code Editor with lots of shortcuts & integrated Intellisense features that make writing code a breeze
  - usage of project file *platformio.ini* for controlling builds, adjusting serial monitor behaviour, enabling debugging, setting port parameters for software uploads (port, speed, RTS/CTS), etc.
  - easy search for/integration of additional libraries
  - integrated debugging functionality (additional external hardware required)
  - integrated source control (github)
  - etc.

### :heavy_exclamation_mark: Application notes  

Freshly released boards with special features (e.g. more Flash, additional PSRAM, exotic display) are sometimes only fully supported by the ESP32 development framework after being added to its official ESP32 board list! Normally this quickly happens but in rare cases this might take weeks/month or does not happen at all. If your particular board is not (yet) on the board list and you want to start programming it anyway then in platformio.ini select another, already known board from the list which preferably is similar in features to your new board and you should be good to go.  

In this case, be aware, you might not be able to fully utilize every board feature and build logs might report incorrect port numbers, memory sizes, etc. Provided example [Test-ESP32-S3-1.9inch_RAM-PSRAM](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-S3-1.9-Display-DevBoard(LILYGO)/examples/Test-ESP32-S3-1.9inch_RAM-PSRAM) demonstrates how to reveal the true memory sizes of an ESP32 development board. Have a look at this example's files _Build.log_ & _Serial_Output.log_ to see above mentioned discrepancies.

### :zap: Useful settings in platformio.ini

The project configuration file has sections (each denoted by a [header]) and one or multiple key/value pairs within the sections. Lines beginning with ";" are ignored and may be used to provide comments. The file platformio.ini is explained in more detail [here](https://docs.platformio.org/en/stable/projectconf/index.html). 

**Important:** Different examples might require different build options, so always have a look at the platformio.ini file for relevant infos.

Some very useful general settings (for more experienced programmers) deserve to be mentioned here. 
1) Forces builds to use older platform packages and/or frameworks instead of the newest one.
```
;platform = espressif32
platform = espressif32 @ 4.2.0
platform_packages = framework-arduinoespressif32 @ https://github.com/espressif/arduino-esp32/releases/download/2.0.4/esp32-2.0.4.zip
;platform_packages = framework-arduinoespressif32@https://github.com/espressif/arduino-esp32.git#2.0.3
```
2) Serial output lines carry time stamps, exceptions get decoded with function backtrace and serial output gets logged to a file:
```
monitor_filters = time, esp32_exception_decoder, log2file
```
3) The program is compiled with full symbolic debug information and no optimization.
```
build_type = debug
```
4) Enables usage of ESP32 JTAG debug adapter "[ESP32-Prog](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/doc/ESP-Prog.jpg)" for debugging.
```
debug_tool = esp-prog
```
5) Sets the initial breakpoint to first line in function setup().
```
debug_init_break = tbreak setup
```
6) Generates various levels of debug output.
```
; 0 no debug output, 1 errors, 2 warnings, 3 infos, 4 debug info, 5 verbose 
build_flags = -DCORE_DEBUG_LEVEL=4
```
7) Disables any activity on COM signals RTS and/or DTR. Required by some ESP32 dev boards to enable serial output (e.g. ESP32-CAM-MB board). However, this setting prevents the IDE from selecting boot modes and performing automatic software uploads to the board. In this case you must start the upload while pressing the boot mode button (usually named BOOT, PRG or IO0) for a few seconds.
```
monitor_rts = 0
monitor_dtr = 0
```
8) Switch between various built-in tables. A big collection of predefined tables can be found [here](https://github.com/espressif/arduino-esp32/tree/master/tools/partitions).
```
;board_build.partitions = min_spiffs.csv
board_build.partitions = no_ota.csv
```
9) Your own "partitions_custom.csv" in the root of the project directory.
```
board_build.partitions = partitions_custom.csv
```
10) Collection of personal tasks, to be found under PlatformIO -> PROJECT TASKS -> Custom. Eventual paths are relative to the project folder.
```
extra_scripts = add_tasks.py
```

## :information_source: Resetting the board and uploading software via USB

The easiest way to reset an ESP32 dev board is to press the **reset button** (mostly labeled **RST** or **EN**). The easiest way to put the ESP32 into flash or upload mode is to press and release the reset button while holding down the button which selects the ESP32 boot mode (mostly labeled **BOOT**, **PRG** or **IO0**).

But have you ever wondered why your dev board sometimes is resetting when you start the Serial Monitor task and your development environment (PlatformIO or Arduino IDE) performs a successful software upload to the ESP32 and resets the board afterwards without prompting to press any button ? 

Almost each ESP32 dev board contains a 2 transistor circuit which enables any IDE to auto reset the ESP32 chip or put it in boot mode. 

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/reset_and_software_upload/ESP32_reset_and_upload_circuitry_only.jpg" height="180"/></p>

Function and design of this little circuitry is explained in detail [here](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/reset_and_software_upload).


## :information_source: Debugging a program 

The ESP32 toolchain features a GNU online debugger (GDB). For debugging programs running on the ESP32 the GDB only needs access to the JTAG pins on the ESP32. This is normally done with a small JTAG adapter/tool with a USB and a JTAG port sitting between the computer running the GDB and the ESP32 dev board.

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/Debugging_with_JTAG_adapter.jpg" height="180"/></p>

Good news:  
Since the release of the ESP32-S3 with builtin JTAG adapter debugging without external adapter has become possible. All one need is an ESP32-S3 dev board with two USB ports, like the YD-ESP32-S3 dev board as shown above.

More infos about debugging the ESP32 is shown [here](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/debugging).
