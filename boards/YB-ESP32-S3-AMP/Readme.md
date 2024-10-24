## YB-ESP32-S3-AMP Development Board Overview:
The **YB-ESP32-S3-AMP** is a general purpose development board based on Espressif's ESP32-S3 MCU. Presently it comes in one variant:  
- with **ESP32-S3-WROOM-1-N8R2** module (**8MB** Flash, **2MB** PSRAM, **WiFi** PCB antenna)

Additionally it provides a **microSD** card slot, two **MAX98357A I2S PCM Class D Amplifiers** (for left + right audio channels), **USB-C connector** for software upload, serial output and/or feeding power to the board, **two status LEDs** and **GPIO pins** for free use. 

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/under-construction.jpg)

Just **connect 2 loudspeakers (4...8 Ohm)** to the board, apply power, build and upload an example and off you go. You can listen to internet radio stations, play audio files from microSD card and much more.  

Of course you can connect additional hardware to the board, e.g. TFT displays, IR receivers or any other module that communicates via I2C/SPI, etc. Please have a look at the provided [examples](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/examples) for PlatformIO resp. ArduinoIDE. 
Most of them use Schreibfaul1's awesome Arduino library [**ESP32-audioI2S**](https://github.com/schreibfaul1/ESP32-audioI2S) which is perfectly suitable for this bord.  

The densly populated YB-ESP32-S3-AMP board provides multiple GPIO pins (as shown below) and is still highly [**breadboard compatible**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_on_breadboard.jpg) for it leaves one row of accessible breadboard contacts on either side of the board. All I/O ports (GPIOx) are clearly labeled on both sides of the board. 

## YB-ESP32-S3-AMP board features in detail:
 - **ESP32-S3-WROOM-1-N8R2** module with 8MB Flash, 2MB PSRAM, WiFi PCB antenna
 - **MAX98357A** I2S PCM Class D Amplifiers (for stereo output) connected to the ESP32-S3 as follows:
   - *GPIO5 - BCLK* (bit clock)
   - *GPIO6 - LRCLK* (frame clock)
   - *GPIO7 - DIN* (digital audio signal)  

    Note: GPIO 5/6/7 are not wired to any board pin, they are exclusively used for the two MAX98357A.
 - **screw terminals** for connecting 2 loudspeakers (4...8 Ohm, left + right audio channel)
 - **microSD** card slot connected to the ESP32-S3 via fast SPI bus *FSPI*:
   - *GPIO10 - SCS* (SPI bus control, chip select, this control line is not needed for SD_MMC-lib and available for other usage when solder bridge *SD_CS* is open [default closed])
   - *GPIO11 - MOSI* (SPI bus data communication, SD_MMC calls it *CMD*)
   - *GPIO12 - SCK* (SPI bus clock signal)
   - *GPIO13 - MISO* (SPI bus data communication, SD_MMC calls it *D0*)  

   Note: GPIO 11/12/13 are not wired to any board pin, they are exclusively used for the microSD slot.
 - **control LEDs**. One LED labeled 'P' is connected to the 3.3V rail to indicate board power and the other LED labeled 'IO47' is connected to GPIO47 which can be used as status LED.  
 If solder bridge *DAC_MUTE* is closed [default open] this pin can be used for muting the two audio amplifiers: active when GPIO47 is HIGH or muted when GPIO47 is LOW.
 - **USB-C** port connected to ESP32-S3 via USB-TTL bridge chip CH340 for serial output and software upload (e.g. via ArduinoIDE, VSCode/PlatformIO etc). Can also be used to power the board.
 - **hardware logic** for *automatic* software uploads (supported by most Development IDEs) via USB-C port. How this works is explained [here](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/reset_and_software_upload).  
 - **pushbuttons**. One is labeled 'R' and resets the ESP32-S3 (shorts EN pin to ground) and the other one is labeled 'B' and shorts GPIO0 to ground when pressed. The latter is sometimes needed to force the board into boot mode.
 - **lots of available GPIOs**, useful for connecting LEDs, buttons, additional modules via SPI/I2C, e.g displays, rotary encoders, bluetooth, etc.

## Board Pin Layout:
 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_pinlayout.jpg)

The boards outline and schematic files are all located in folder [doc](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/doc), together with data sheets for Espressif's MCU ESP32-S3 and ESP32-S3-WROOM-1 module.

## Powering the board:
There are two ways to provide power to the board:
  - through the USB-C port or
  - ~5VDC applied to the 5V pin  

The board uses a LDO to drop the external supply voltage to 3.3Volt needed by the ESP32-S3. The two audio amplifiers MAX98357A get their voltage directly from pin '5V'. They can operate up to 5.5V and their absolute maximum rating is 6V. Therefore **never supply more than ~5.5VDC to the '5V' power input pin** !!  


Normal operating current of the idle board (all GPIOs unconnected, no audio output, WiFi disabled) is about 45mA. With WiFi active the board draws about 100mA (mainly depending on WiFi link). With WiFi active, a microSD in the slot and audio output on both amp channels the current rises to ~150mA.

## Application hints: 

### Arduino IDE:
Select the board "**ESP32S3 Dev Module**" and choose the proper settings as shown below. Be aware, since the ESP32-S3 MCU is very versatile there are a lot of build options to play with. Espressif's homapage offers some help as well.

Settings that apply to the **YB-ESP32-S3-AMP-N8R2** board:  
- Board: *ESP32S3 Dev Module*
- Flash Size: *8MB*
- Partition Scheme: e.g. *8MB with spiffs (...)*
- PSRAM: *OPI PSRAM*  

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP-N8R2_ArduinoIDE-Settings.jpg)  

### PlatformIO:
Building with **PlatformIO** is even easier. Each of the provided software examples for PlatformIO comes with a subdirectory *'boards'*. This holds the specific board definition files (e.g. YB-ESP32-S3-AMP-n8r2.json) which provide the proper board definitions & settings.  

Going a step further and putting these files to your development platforms *core_dir/platforms/espressif32/boards* global directory (on my PC it's  *C:\Users\name\\.platformio\platforms\espressif32\boards*) means you can create a new project in PlatformIO and simply select this board from the pull-down list.  

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_PlatformIO_board_selection.jpg)

### General notes:

1) In case you get left channel output on the right speaker and vice-versa (L + R channels are swapped) you need to build with a [newer ESP32-AudioI2S library version](https://github.com/schreibfaul1/ESP32-audioI2S/releases) with this error fixed.

2) The ESP32-S3 includes four SPI controllers: SPI0, SPI1, SPI2(Fast SPI) and SPI3. SPI0/1 are reserved for Flash and PSRAM (if available) and should be left alone. The remaining two are available for the public. In the Arduino universe SPI2 & SPI3 are named FSPI & HSPI. FSPI by default is assigned to GPIOs 10-15 and routed via IO MUX. However, both can get pinned to any available GPIO pin if needed but FSPI will be slower if routed through GPIO Matrix

3) The ESP32-S3-WROOM-1 module family comprises several [**versions**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/ESP32-S3-WROOM-1(U)_Variants.jpg). The **-1** versions come with embedded PCB antenna, the **-1U** versions with IPEX antenna socket instead. The extension -Nx(Ry) defines the amount of integrated FLASH/PSRAM, e.g. -N4 (4MB Flash, no PSRAM), -N4R2 (4 MB Flash, 2MB PSRAM), -N8R2 (8 MB Flash, 2MB PSRAM) etc.  

### Software Upload to the board:
In rare cases you might see the following after powering up the board:  
```
ESP-ROM:esp32s3-20210327
23:10:07.248 > Build:Mar 27 2021
23:10:07.251 > rst:0x7 (TG0WDT_SYS_RST),boot:0x8 (SPI_FAST_FLASH_BOOT)
23:10:07.256 > Saved PC:0x40049087
23:10:07.256 > invalid header: 0xffffffff
23:10:07.259 > invalid header: 0xffffffff
....
....
```
In such case force the ESP32-S3 into upload mode *manually*. Just keep the **'B'** button pressed, then press/release the **'R'** button and finally release the **'B'** button. The serial monitor output will subsequently confirm the boards readiness and the next upload attempt will be successful.  

```
....
23:19:07.147 > invalid header: 0xffffffff
23:19:07.453 > ESP-ROM:esp32s3-20210327
23:19:07.453 > Build:Mar 27 2021
23:19:07.453 > rst:0x1 (POWERON),boot:0x0 (DOWNLOAD(USB/UART0))
23:19:07.459 > waiting for download
```

### Flash/RAM usage:

The -N8R2 module on the board should provide enough memory even for demanding projects. Building e.g. software example [Play-All-MP3-Files-on-SD](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/examples/ArduinoIDE/Play-All-MP3-Files-on-SD) shows the following Flash/RAM usage:  
```
Executing task: C:\Users\tj\.platformio\penv\Scripts\platformio.exe run 

Processing n8r2 (board: yb-esp32-s3-amp-n8r2; platform: espressif32; framework: arduino)
------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/yb-esp32-s3-amp-n8r2.html
PLATFORM: Espressif 32 (6.9.0) > YB-ESP32-S3-AMP-N8R2 (8 MB QD FLASH, 2 MB PSRAM)
HARDWARE: ESP32S3 240MHz, 320KB RAM, 8MB Flash
DEBUG: Current (esp-builtin) On-board (esp-builtin) External (cmsis-dap, esp-bridge, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20017.0 (2.0.17) 
 - tool-esptoolpy @ 1.40501.0 (4.5.1) 
 - toolchain-riscv32-esp @ 8.4.0+2021r2-patch5 
 - toolchain-xtensa-esp32s3 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 62 compatible libraries
Scanning dependencies...
Dependency Graph
|-- SD @ 2.0.0
|-- ESP32-audioI2S @ 2.0.0
|-- FS @ 2.0.0
|-- SPI @ 2.0.0
Building in release mode
Retrieving maximum program size .pio\build\n8r2\firmware.elf
Checking size .pio\build\n8r2\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   8.4% (used 27608 bytes from 327680 bytes)
Flash: [===       ]  28.5% (used 950957 bytes from 3342336 bytes)
```
## Remark:  
All YB-ESP32-S3-AMP boards delivered are already flashed with software example 'Play-All-MP3-Files-on-SD'. For a quick board test just insert a microSD card (FAT32) into the socket holding some *.mp3 files, connect one or two speaker(s) to the board and power it up.
