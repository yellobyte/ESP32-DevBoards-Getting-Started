## YB-ESP32-S3-AMP Development Board Overview:
The **YB-ESP32-S3-AMP** is a general purpose development board based on Espressif's ESP32-S3 MCU. It comes in two different flavours (Rev.2 & Rev.3) and features a **ESP32-S3-WROOM-1-N8R2** module (8MB Flash, 2MB PSRAM, WiFi PCB antenna) and is available on sales platforms [eBay](https://www.ebay.de/sch/i.html?_nkw=yb-esp32-s3) and [Ricardo.ch](https://www.ricardo.ch/en/s/YB-ESP32-S3).

Apart from the **ESP32-S3** it provides a **microSD** card slot, two **MAX98357A I2S PCM Class D Amplifiers** (for left + right audio channels), **USB-C connector** for software upload, serial output and/or feeding power to the board, **two status LEDs** and **GPIO pins** for free use. 

Revision 2 features an USB bridge chip CH340 between USB-C and ESP32-S3. On boards with revision 3 the USB-C is connected directly to the ESP32-S3. Both solutions have their own advantages so you can select the model you prefer.

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_Rev2_top_with_screw_terminals.jpg" height="160"/><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_Rev3_top_with_screw_terminals.jpg" height="160"/></p>  

For an easy start **connect 2 loudspeakers (4...8 Ohm)** to the board, apply power, build and upload an example and off you go. You quickly can listen to **internet radio stations**, **play audio files from microSD card** and much more.  

Of course you can connect additional hardware to the board, e.g. TFT displays, IR receivers or any other module that communicates via I2C/SPI, etc. Please have a look at the provided [examples](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/examples) for PlatformIO resp. ArduinoIDE. 
Most of them use Schreibfaul1's awesome Arduino library [**ESP32-audioI2S**](https://github.com/schreibfaul1/ESP32-audioI2S) which is highly suitable for this board.  

The densly populated YB-ESP32-S3-AMP board provides multiple GPIO pins (as shown below) and is still highly [**breadboard compatible**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_on_breadboard.jpg) for it leaves one row of accessible breadboard contacts on either side of the board. All I/O ports (GPIOx) are clearly labeled on both sides of the board. 

## YB-ESP32-S3-AMP board features in detail:
 - **ESP32-S3-WROOM-1-N8R2** module with 8MB Flash, 2MB PSRAM, WiFi PCB antenna
 - **2 x MAX98357A** I2S PCM Class D Amplifier (stereo output, 2 x 3.2W @ 4Ohm) connected to the ESP32-S3 as follows:
   - *GPIO5 - BCLK* (bit clock)
   - *GPIO6 - LRCLK* (frame clock)
   - *GPIO7 - DIN* (digital audio signal)  

    Note: GPIOs 5/6/7 are not wired to any board pin, they are exclusively used for the two MAX98357A.
 - **screw terminals** for connecting 2 loudspeakers (4...8 Ohm, left + right audio channel)
 - **microSD** card slot connected to the ESP32-S3 via fast SPI bus *FSPI*:
   - *GPIO10 - SCS* (SPI bus control, chip select, this control line is not needed for SD_MMC-lib and available for other usage when solder bridge *SD_CS* is open [default closed])
   - *GPIO11 - MOSI* (SPI bus data communication, SD_MMC calls it *CMD*)
   - *GPIO12 - SCK* (SPI bus clock signal)
   - *GPIO13 - MISO* (SPI bus data communication, SD_MMC calls it *D0*)  

   Note: GPIOs 11/12/13 are not wired to any board pin, they are exclusively used for the microSD slot.
 - **control LEDs**. One LED labeled 'P' is connected to the 3.3V rail to indicate board power and the other LED labeled 'IO47' is connected to GPIO47 which can be used as status LED.  
 If solder bridge *DAC_MUTE* is closed [default open] this pin can be used for muting the two audio amplifiers: active when GPIO47 is HIGH or muted when GPIO47 is LOW.
 - **USB-C** port connected to ESP32-S3 via USB-TTL bridge chip CH340 (board revision 2.x only) resp. directly to the ESP32-S3 (board revision 3.x only) for serial output and software upload (e.g. via ArduinoIDE, VSCode/PlatformIO etc). Can also be used to power the board.
 - **hardware logic** for *automatic* software uploads (supported by most Development IDEs) via USB-C port on boards with revision 2.x. How this works is explained [here](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/reset_and_software_upload).  
 - **pushbuttons**. One is labeled 'R' and resets the ESP32-S3 (shorts EN pin to ground) and the other one is labeled 'B' and shorts GPIO0 to ground when pressed. The latter is sometimes needed to force the board into boot mode.
 - **lots of available GPIOs**, useful for connecting LEDs, buttons, additional modules via second SPI bus or I2C, e.g displays, rotary encoders, bluetooth, etc.

In summary, the board replaces a lot of separate modules & cables, saves space on the breadboard and might avoid the odd frustration to get it all running.  

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_modules_replacement.jpg)

## Board Pin Layout:
 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_pinlayout.jpg)

The boards **outline** and **schematic** files are all located in folder [doc](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/doc), together with data sheets for Espressif's MCU ESP32-S3 and the ESP32-S3-WROOM-1 module family.

## Powering the board:
There are two ways to provide power to the board:
  - through the USB-C port or
  - ~5VDC applied to the 5V pin  

The board uses a LDO to drop the external supply voltage to 3.3Volt needed by the ESP32-S3. The two audio amplifiers MAX98357A though get their voltage directly from pin '5V'. They can operate up to 5.5V and their absolute maximum rating is 6V. Therefore **never supply more than ~5.5VDC to the '5V' power input pins** !!  

Normal operating current of the idle board (all GPIOs unconnected, no audio output, WiFi disabled) is about 45mA. With WiFi active the board draws about 100mA (mainly depending on WiFi link). With WiFi active, a microSD in the slot and lower audio output power on both amp channels the current rises to ~160mA.

### Recommended wiring for max. audio output power:

Each audio channel can produce max. 3.2W output power at 4 Ohms. If your project requires more then just a few mW of audio output power it is recommended to use a capable power supply (5VDC/1.5A) and preferably an additional capacitor of min. 220uF or greater value across the 5V/GND pins of the board as shown below:  

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_recommended_wiring.jpg)

## Application hints: 
The board with revision 2.x uses the popular WCH CH340X bridge chip between USB-C port and the ESP32-S3. If you plan to use the USB port then you need to install the CH340X Drivers on your Laptop/PC. For Windows go [here](https://www.wch-ic.com/search?t=all&q=ch341) and select the newest version of the driver installer 'CH341SER.EXE'. 

### Arduino IDE:
As of Arduino ESP32 Core V3.1.1 you open the board list, enter "yb" and then select "**Yellobyte YB-ESP32-S3-AMP**". Now choose the proper settings for COM port, debug level, flash size, PSRAM, etc. as shown below. Be aware, since the ESP32-S3 MCU is very versatile there are a lot of build options to play with. Espressif's homepage offers some help.

Correct **ArduinoIDE** settings for the **YB-ESP32-S3-AMP (Rev.2, with CH340)** board:  
- Board: *YB-ESP32-S3-AMP (Rev.2)*
- USB CDC On Boot: _Disabled_
- Flash Size: *8MB (64Mb)*
- Partition Scheme: *8MB with spiffs (...)*
- PSRAM: *QSPI PSRAM*  

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_Rev2_ArduinoIDE-Settings.jpg) 

Following the correct **ArduinoIDE** settings for the **YB-ESP32-S3-AMP (Rev.3)** board:  
- Board: *YB-ESP32-S3-AMP (Rev.3)*
- USB CDC On Boot: **_Enabled_**
- Flash Size: *8MB (64Mb)*
- Partition Scheme: *8MB with spiffs (...)*
- PSRAM: *QSPI PSRAM* 

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_Rev3_ArduinoIDE-Settings.jpg) 
  
### PlatformIO:
Building with **PlatformIO** is easy as well. Starting with Arduino ESP32 Core v3.1.1 the VSCode/PlatformIO IDE provides all the necessary board definition files. These *.json files provide the correct board definitions & settings.  

Just create a new project and give it a name, then go to board selection, enter "yb-" and choose your YB-ESP32-S3-*** board from the list thats popping up.

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/YB-ESP32-S3-AMP_PlatformIO_board_selection.jpg)

Examples that need to be build with an older framework still come with a folder "boards" which keeps the necessary *.json board definition files. 

### Software Upload to the board:

Uploading new software to the board with your IDE normally is a breeze. Select the correct COM port and upload the program.

However, every now and then an upload might fail and you see this:
```
ESP-ROM:esp32s3-20210327
23:10:07.248 > Build:Mar 27 2021
23:10:07.251 > rst:0x7 (TG0WDT_SYS_RST),boot:0x8 (SPI_FAST_FLASH_BOOT)
23:10:07.256 > Saved PC:0x40049087
23:10:07.256 > invalid header: 0xffffffff
23:10:07.259 > invalid header: 0xffffffff
....
```
In such case you can force the ESP32-S3 into upload mode *manually*. Keep the **'B'** button pressed, then press/release the **'R'** button and finally release the **'B'** button. The serial monitor output will subsequently confirm the boards readiness and the next upload attempt will be successful.  

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

Processing v2 (board: yb-esp32-s3-amp-v2; platform: espressif32; framework: arduino)
------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/yb-esp32-s3-amp-v2.html
PLATFORM: Espressif 32 (6.9.0) > YB-ESP32-S3-AMP (8 MB QD FLASH, 2 MB PSRAM)
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
Retrieving maximum program size .pio\build\v2\firmware.elf
Checking size .pio\build\v2\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   8.4% (used 27608 bytes from 327680 bytes)
Flash: [===       ]  28.5% (used 950957 bytes from 3342336 bytes)
```
### General notes:

1) In case you get left channel output on the right speaker and vice-versa (L + R channels are swapped) you need to build with a [newer ESP32-AudioI2S library version](https://github.com/schreibfaul1/ESP32-audioI2S/releases) with this error fixed.

2) The ESP32-S3 includes four SPI controllers: SPI0, SPI1, SPI2(Fast SPI) and SPI3. SPI0/1 are reserved for Flash and PSRAM (if available) and should be left alone. The remaining two are available for the public. In the Arduino universe SPI2 & SPI3 are named FSPI & HSPI. FSPI by default is assigned to GPIOs 10-15 and routed via IO MUX. However, both can get pinned to any available GPIO pin if needed but FSPI will be slower if routed through GPIO Matrix

3) The ESP32-S3-WROOM-1 module family comprises several [**versions**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/ESP32_specs_and_manuals/ESP32-S3-WROOM-1(U)_Variants.jpg). The **-1** versions come with embedded PCB antenna, the **-1U** versions with IPEX antenna socket instead. The extension -Nx(Ry) defines the amount of integrated FLASH/PSRAM, e.g. -N4 (4MB Flash, no PSRAM), -N4R2 (4 MB Flash, 2MB PSRAM), -N8R2 (8 MB Flash, 2MB PSRAM) etc.  

5) Boards with revision 3.x support USB Serial JTAG debugging. An example is provided [here](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/examples/PlatformIO/Debug-BoardRev3-via-builtin-JTAG). 

### Integrating this board into your own PCB design projects:

Its easy. Folder [doc](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-AMP/doc) provides the Eagle library file **_yb-esp32-s3-amp.lbr_** containing both boards. Most other PCB design software (e.g. KiCad) are able to import and use Eagle lib files. 

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/Eagle_project_with_yb-esp32-s3-amp.jpg" height="250"/>&nbsp;<img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-AMP/doc/Eagle_project_with_yb-esp32-s3-amp2.jpg" height="250"/></p> 

## Final Remark for first usage:  
**>>> All YB-ESP32-S3-AMP boards delivered have already been flashed with software example 'Play-All-MP3-Files-on-SD'. <<<**  

For a quick board test just insert a microSD card (FAT32) into the socket holding some *.mp3 files, connect one or two speaker(s) to the board and power it up (via USB or 5V pins).  

The status LED will light up when the microSD card has been detected successfully and the board will start playing all *.mp3 files found.
