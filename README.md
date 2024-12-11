# ESP32 Development Boards - Getting started

A big variety of **ESP32** development boards (from Espressif and lots of other companies) have surfaced since the first ESP32 chip was released in 2016 by Espressif. Most boards contain the original **ESP32**, some an **ESP32-S2** and newer ones the most recent flavour **ESP32-S3**. For differences between those chips look [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/ESP32_specs_and_manuals/Comparison_Espressif-ESP32-S2-C3-S3.pdf).

The ESP32 chip family is loaded with features, e.g. WiFi/Bluetooth, dual core (up to 240MHz clock frequency), integrated Flash memory, lots of GPIO pins (peripheral input/output), ADC/DAC, UART, I2S, SPI, low power states, JTAG debugging option, etc. Have a look at the many documents in folder [**ESP32_specs_and_manuals**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/ESP32_specs_and_manuals) for various ESP32 datasheets.

Almost all development boards feature a greater number of GPIO pins, a combination of WLAN antenna and/or IPX antenna connector, USB-Port, LDO voltage regulator and some LEDs (Power/RX/TX). Some boards have additional features, like RJ45 Ethernet, TFT display, camera (e.g. OV2640), RGB-LED (e.g. WS2812B), battery connector, more Flash memory (up to 16MB) and/or PSRAM (up to 8MB), etc.

Probably the best way to get familiar with a certain board is writing a few tiny software programs (examples) for it. A good start would be to let the onboard LED blink or change color (RGB-LED), generate a few lines serial output, connect to the local WLAN, read the status of a connected push button, put the ESP32 into sleep mode, create a single file on an attached SD-Card or some other simple task. You get the idea.  

BTW: Arduino uses the name **sketch** (file extension *.ino) for a program. 

## :file_folder: Software Examples

Under [**boards**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards) you find a (slowly growing) collection of programs for each board listed below. They will help you explore the features of those boards.

Examples are available for **VSCode/PlatformIO** and/or **ArduinoIDE**. Each example has it's own project directory (the ones for PlatformIO contain a platformio.ini file). Make sure you have the latest **Arduino ESP32 development package** installed.

Right now examples are provided for the following boards:

- [**ESP32-DevKit-V1** with ESP32-WROOM-32 module and microUSB](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-DevKit-V1)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-DevKit-V1/doc/ESP32-Devkit-V1.jpg" height="160"/></p>  

- [**YB-ESP32-S3-AMP** with PSRAM, microSD, 2 x MAX98357A Audio Amplifier, WiFi, USB-C](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_Rev2_top_with_screw_terminals.jpg" height="160"/></p>  

- [**YB-ESP32-S3-ETH** with Ethernet (W5500), WiFi, two USB ports and optional PSRAM](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-ETH)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH/doc/YB-ESP32-S3-ETH_board_top.jpg" height="160"/></p>  

- [**ESP32-CAM**, **ESP32-CAM-USB**, **ESP32-CAM-MB** with OV2640 camera, microSD and Flash LED](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-CAM)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM/doc/ESP32-CAM.jpg" height="160"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM/doc/ESP32-CAM-USB.jpg" height="150"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-CAM/doc/ESP32-CAM-MB.jpg" height="160"/></p>

- [**ESP32-1.14-Display**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-1.14-Display-DevBoard)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-1.14-Display-DevBoard/doc/LILYGO-ESP32-1.14-Display.jpg" height="160"/></p>

- [**ESP32-S3-0.85-Display**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-S3-0.85-Display-DevBoard)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-S3-0.85-Display-DevBoard/doc/LILYGO-ESP32-S3-0.85-Display.jpg" height="160"/></p>

- [**ESP32-S3-1.9-Display**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-S3-1.9-Display-DevBoard)
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/ESP32-S3-1.9-Display-DevBoard/doc/LILYGO-ESP32-S3-1.9-Display.jpg" height="160"/></p>

- [**YD-ESP32-S3** with big Flash, external PSRAM, WS2812B RGB-LED and two USB-C ports](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YD-ESP32-S3)  
<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YD-ESP32-S3/doc/YD-ESP32-S3_DevBoard.JPG" height="160"/></p>

## :hammer_and_wrench: Choosing a Development Platform

Arduino IDE is easy for beginners, simple but somewhat limited in functionality. In contrast PlatformIO is a much more powerful and versatile alternative which makes every task a breeze after getting familiar with it. 

VSCode/PlatformIO provides some major advantages over the Arduino IDE:
  - very comfortable and mighty code Editor with lots of shortcuts & integrated Intellisense features that make writing code a breeze
  - usage of project file *platformio.ini* for controlling builds, adjusting serial monitor behaviour, enabling debugging, setting port parameters for software uploads (port, speed, RTS/CTS), etc.
  - easy search for/integration of additional libraries
  - integrated debugging functionality (additional external hardware required)
  - integrated source control (github)
  - etc.

### :heavy_exclamation_mark: Application notes  

If your particular board is not (yet) on the board list and you want to start programming it anyway then select another, already known board from the list which preferably is similar in features to your new board and you should be good to go.  

However, in some cases you might not be able to fully utilize every board feature and build logs might report incorrect port numbers, memory sizes, etc. Provided example [**Test-ESP32-S3-1.9inch_RAM-PSRAM**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/ESP32-S3-1.9-Display-DevBoard/examples/Test-ESP32-S3-1.9inch_RAM-PSRAM) demonstrates how to reveal the true memory sizes of an ESP32 development board. Have a look at this example's files _Build.log_ & _Serial_Output.log_ to see above mentioned discrepancies.
### :zap: Useful settings in platformio.ini

The project configuration file has sections (each denoted by a [header]) and one or multiple key/value pairs within the sections. Lines beginning with ";" are ignored and may be used to provide comments. The file platformio.ini is explained in more detail [**here**](https://docs.platformio.org/en/stable/projectconf/index.html). 

**Important:** Different examples might require different build options, so always have a look at the platformio.ini file for relevant infos.

A small selection of some very useful project settings (for more experienced programmers) deserve to be mentioned here. 
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
3) Range of COM ports to be tried for upload and monitoring.
```
monitor_port = COM[345679]
upload_port = COM[345679]
```
4) The program is compiled with full symbolic debug information and no optimization.
```
build_type = debug
```
5) Enables usage of ESP32 JTAG debug adapter "[ESP32-Prog](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/debugging/JTAG%20adapter%20docs/ESP-Prog.jpg)" for debugging.
```
debug_tool = esp-prog
```
6) Sets the initial breakpoint to first line in function setup().
```
debug_init_break = tbreak setup
```
7) Generates various levels of debug output.
```
; 0 no debug output, 1 errors, 2 warnings, 3 infos, 4 debug info, 5 verbose 
build_flags = -DCORE_DEBUG_LEVEL=4
```
8) Disables any activity on COM signals RTS and/or DTR. Required by some ESP32 dev boards to enable serial output (e.g. ESP32-CAM-MB board). Be aware, this setting prevents the IDE from selecting boot modes and performing automatic software uploads to the board. In this case you must start the upload while pressing the boot mode button (usually named BOOT, PRG or IO0) for a few seconds.
```
monitor_rts = 0
monitor_dtr = 0
```
9) Switch between various built-in partition tables, which gets importand when you build for a board available with various flash sizes (e.g. Board-Xyz-N4 and Board-Xyz-N8R2). A big collection of predefined partition tables can be found [here](https://github.com/espressif/arduino-esp32/tree/master/tools/partitions). Below a few examples:
```
; standard partitions for boards with 4MB Flash (or more)
board_build.partitions = default.csv

; for boards with 4MB Flash (or more) with reduced spiffs partition and bigger application partition
;board_build.partitions = min_spiffs.csv

; for boards with 4MB Flash (or more), not OTA partition, bigger application partition
;board_build.partitions = no_ota.csv

; standard partitions for boards with 8MB Flash (or more)
;board_build.partitions = default_8MB.csv

; standard partitions for boards with 16MB Flash
;board_build.partitions = default_16MB.csv
```
10) You can even define your own partition tables. Copy an existing (and ideally almost matching) *.csv file into the root of your project directory, rename it to "partitions_custom.csv" and edit the partition settings as needed (e.g. size, offset).
```
board_build.partitions = partitions_custom.csv
```
11) Collection of personal tasks, to be found under PlatformIO -> PROJECT TASKS -> Custom. Eventual paths are relative to the project folder.
```
extra_scripts = add_tasks.py
```

## :information_source: Resetting the board and uploading software via USB

The easiest way to reset an ESP32 dev board is to press the **reset button** (mostly labeled **RST** or **EN**). The easiest way to put the ESP32 into flash or upload mode is to press and release the reset button while holding down the button which selects the ESP32 boot mode (mostly labeled **BOOT**, **PRG** or **IO0**).

But have you ever wondered why your dev board sometimes is resetting when you start the Serial Monitor task and your development environment (PlatformIO or Arduino IDE) performs a successful software upload to the ESP32 and resets the board afterwards without prompting to press any button ? 

Almost each ESP32 dev board contains a 2 transistor circuit which enables any IDE to auto reset the ESP32 chip or put it in boot mode. 

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/reset_and_software_upload/ESP32_reset_and_upload_circuitry_only.jpg" height="180"/></p>

Function and design of this little circuitry is explained in detail in folder [**reset_and_software_upload**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/reset_and_software_upload).


## :information_source: Debugging a program via JTAG

Some **practical hints** on how to get started with debugging esp. with **ESP32-Prog** and **ESP32-S3's builtin JTAG adapter** I have put together in folder [**debugging**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/debugging).

Here a quick summary:  

The ESP32 toolchain features a GNU online debugger (GDB). GDB relies on another task running in the background called OpenOCD (Online-Chip-Debugger for ESP32). This task interacts via USB with an JTAG adapter.

This JTAG adapter can be 
- a separate external hardware (e.g. [ESP32-Prog module](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/JTAG%20adapter%20docs/ESP-Prog.jpg)) which is connected to the ESP32 JTAG pins, or
- integrated into the ESP32 chip itself (e.g ESP32-S3 with builtin JTAG adapter) 

A general introduction to JTAG debugging is given on [**this webpage**](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/jtag-debugging/index.html) from Espressif.  The picture below is taken from it.

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/Debugging_with_JTAG_adapter-block-diagram.jpg" height="450"/></p>

## :information_source: Debugging a program without JTAG

Probably the most frustrating part of coding is provoking a core to panic/to crash which in most cases looks similar to this:

```
23:29:09.416 > Guru Meditation Error: Core  1 panic'ed (IntegerDivideByZero). Exception was unhandled.
23:29:09.476 >
23:29:09.476 > Core  1 register dump:
23:29:09.479 > PC      : 0x4200193b  PS      : 0x00060430  A0      : 0x820019bb  A1      : 0x3fcebf40  
23:29:09.486 > A2      : 0x00000003  A3      : 0x00000001  A4      : 0x00000000  A5      : 0x0000e100  
...
```
However, don't panic ! A quick guide how to read aboves message and discover the cause of pain I have put together in folder [**debugging_without_JTAG**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/debugging_without_JTAG).
