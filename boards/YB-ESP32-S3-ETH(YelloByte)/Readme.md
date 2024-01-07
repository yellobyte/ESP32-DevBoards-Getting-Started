## YB-ESP32-S3-ETH Development Board Overview:
The **YB-ESP32-S3-ETH** is a general purpose development board based on Espressif's ESP32-S3 MCU. It comes in different variants: with **ESP32-S3-WROOM-1U-N8R8 module** (8MB Flash/8MB PSRAM) or with **ESP32-S3-WROOM-1U-N4 module** (4MB Flash).  

All variants provide **WiFi**, **RJ45 Ethernet connector**, an Ethernet PHY bridge chip **Wiznet W5500** and **two microUSB connectors** for software upload, serial output, debugging and feeding power to the board. The boards ware currently available on sales platforms eBay [-N8R8](https://www.ebay.com/itm/125966982183), [-N4](https://www.ebay.com/itm/126265587891) and [Ricardo.ch](https://www.ricardo.ch/en/s/YB-ESP32-S3-ETH).

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/YB-ESP32-S3-ETH_board_top.jpg)

Arduino libraries for the Wiznet W5500 are widely available (e.g. [**here**](https://www.arduino.cc/reference/en/libraries/ethernet/)), enabling you to easily realise ESP32 projects around this board with **Ethernet or combined Ethernet/WLAN** support.

The densly populated YB-ESP32-S3-ETH board provides multiple GPIO pins (as shown below) and is still [**highly breadboard compatible**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/YB-ESP32-S3-ETH_on_breadboard.jpg) for it leaves one row of accessible breadboard contacts on either side of the board. All I/O ports (GPIOx) are labeled on both sides of the board. 

If WiFi/BT is needed instead of or additionally to an Ethernet connection then an external 2.4GHz antenna can be connected to the onboard WROOM-1U module. The connector is compatible with the following standards: U.FL (Hirose), MHF-I (I-PEX) and AMC (Amphen). 

### YB-ESP32-S3-ETH board features:
 - ESP32-S3-WROOM-1U-N8R8 module with 8MB Flash, 8MB PSRAM **or** ESP32-S3-WROOM-1U-N4 module with 4MB Flash 
 - WiFi/BT IPEX antenna connector
 - RJ45 10M/100M Ethernet connector driven by onboard PHY controller chip Wiznet W5500
 - The W5500 pins required to control the chip are hardwired to the ESP32-S3 GPIOs as follows:
   - *MOSI - GPIO11, MISO - GPIO13, SCK - GPIO12* (SPI bus data communication)
   - *SCS - GPIO14* (chip select, required for SPI bus control)
   - *RST - GPIO21* (chip reset, needed only for chip reset without resetting the whole board)  
   - *INT - GPIO18* (only if solder bridge on bottom is closed, seldomly needed)  
 - Two control LEDs. One LED labeled 'P' is connected to the 3.3V rail to indicate board power and the other LED labeled 'IO47' is connected to GPIO47 which can be used as status LED, for debugging purposes etc.
 - Two microUSB ports. One is connected to the ESP32-S3 directly for debugging or serial output (labeled 'USB') and the other via USB-TTL bridge chip CH340 (labeled 'UART') for serial output and software upload (e.g. via ArduinoIDE, VSCode/PlatformIO etc). The ESP32-S3 contains an inbuild JTAG adapter hence [**debugging**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/debugging) via port 'USB' becomes fairly easy.
 - Hardware logic for *automatic* software uploads (supported by most Development IDEs) via microUSB port labeled 'UART'.  
 - Two pushbuttons. One is labeled 'R' and resets the ESP32-S3 (shorts EN pin to ground) and the other one is labeled 'B' and shorts GPIO0 to ground when pressed. The latter is sometimes needed to force the board into boot mode.
 - Lots of available GPIOs next to the ones already mentioned above. Have a look at the boards pin layout and [**block diagram**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/YB-ESP32-S3-ETH_block_diagram.jpg) for more detailed hardware infos.
		
Detailed technical infos about Espressif's MCU ESP32-S3 and their ESP32-S3-WROOM-1(U) modules are provided [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc).

Just for information purposes: The ESP32-S3-WROOM-1(U) module family comprises several [**versions**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/ESP32-S3-WROOM-1(U)_Variants.jpg). The **-1** versions come with embedded PCB antenna, the **-1U** versions with IPEX antenna socket instead. The extension -Nx(Ry) defines the amount of available FLASH/PSRAM, e.g. -N4 (4MB Flash, no PSRAM), -N8 (8MB Flash, no PSRAM), -N8R2 (8 MB Flash and 2MB PSRAM), -N8R8 (8 MB Flash and 8MB PSRAM) etc.  

## Board Pin Layout:
 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/YB-ESP32-S3-ETH_pinlayout.jpg)

## Powering the board:
The board uses a LDO to drop the external supply voltage (5VDC min.) and internally operates on 3.3Volt. There are three (mutually exclusive) ways to provide power to the board.
  - through the microUSB ports
  - 5...9VDC applied to the VIN pin
  - 3.3VDC applied to the 3V3 pin(s)  

Normal operating current of the idle board (equipped with ESP32-S3-WROOM-1-N8R8, all GPIOs unconnected, Ethernet Link down, WiFi disabled) is about 120mA. With attached Ethernet cable and Link up the current rises to about 180mA. With WiFi active as well the board draws about 210...260mA (depending on WiFi link).

## Application hints:
At the time of this writing most development IDEs support the Espressif32 platform including ESP32-S3/C3 boards, e.g. the popular Espressif ESP32-S3-DevKitC-1 board.  

### Arduino IDE:
Select the board "**ESP32S3 Dev Module**" and choose the proper settings as shown below. Be aware, since the ESP32-S3 MCU is very versatile there are a lot of build options to play with. Espressif's homapage offers some help es well.

Special settings that apply to the **-N8R8** board:  
- Board: *ESP32S3 Dev Module*
- Flash Size: *8MB*
- Partition Scheme: *8MB with spiffs (...)*
- PSRAM: *OPI PSRAM*  

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/YB-ESP32-S3-ETH-N8R8_ArduinoIDE-Settings.jpg)  

For the **-N4** board the following applies: 
- Board: *ESP32S3 Dev Module*
- Flash Size: *4MB*
- Partition Scheme: *Default 4MB*
- PSRAM: *Disabled*.

### PlatformIO:
Building with **PlatformIO** is no problem as well. Use the provided files in the examples directories.
Each of the provided software example for PlatformIO comes with a subdirectory *'boards'*. There lie the specific board definition files (yb-esp32-s3-eth-n...json) that provide the build environment with the proper board definitions & settings.  

Going a step further and putting these files to your development platforms *core_dir/platforms/espressif32/boards* global directory (on my PC it's  *C:\Users\name\\.platformio\platforms\espressif32\boards*) means you can now create a new project in PlatformIO and simply select this board from the pull-down list.  

 ![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/doc/YB-ESP32-S3-ETH_PlatformIO_board_selection.jpg)

### Using the two USB ports:

With both MicroUSB ports connected simultaneously you will see 2 COM ports and an additional usb device "USB JTAG/serial debug unit". How to use this device for debugging is explained in detail [**here**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/debugging).

**Serial output** generated with Serial.print() can be directed either to port **'USB'** or port **'UART'**. 

In ArduinoIDE choose as follows:  
- **USB CDC On Boot: Disabled**, serial output goes to **UART**
- **USB CDC On Boot: Enabled**, serial output goes to **USB**

In PlatformIO set a build flag in file platformio.ini:  
- **ARDUINO_USB_CDC_ON_BOOT=0**, serial output goes to **UART**
- **ARDUINO_USB_CDC_ON_BOOT=1**, serial output goes to **USB**

**Remarks:**  

The MicroUSB socket labeled 'USB' is wired directly to the ESP32-S3 MCU (GPIO19/GPIO20). Therefore when the development board gets resetted a PC/Laptop connected to this port might temporarily lose connection (the associated COM port will disappear for a second or two). This can confuse some terminal programs. Preferably port UART is used for serial output and port USB for debugging.  

The PlatformIO builder scripts (*.json) for modules containing ESP32-S3/C3 already define the build flag _ARDUINO_USB_MODE=1_. It disables the USB-OTG mode. If not disabled or you want to override it you can (re-)define it in your platformio.ini control file. Normally you don't have to worry about it.
   
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
In such case **put the ESP32-S3 into upload mode manually** by keeping the **'B'** button pressed, then pressing/releasing the **'R'** button and finally releasing the **'B'** button. The serial monitor output will subsequently confirm the boards readiness:  

```
....
23:19:07.147 > invalid header: 0xffffffff
23:19:07.453 > ESP-ROM:esp32s3-20210327
23:19:07.453 > Build:Mar 27 2021
23:19:07.453 > rst:0x1 (POWERON),boot:0x0 (DOWNLOAD(USB/UART0))
23:19:07.459 > waiting for download
```
Below the log of flashing the dev board with provided software example [Test-ESP32-S3-ETH-DHCP](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-ETH(YelloByte)/examples/Test-ESP32-S3-ETH-DHCP).  
```
Executing task: C:\Users\tj\.platformio\penv\Scripts\platformio.exe run --target upload --target monitor --environment n8r8 --upload-port COM7 --monitor-port COM7 

Processing n8r8 (board: yb-esp32-s3-eth-n8r8; platform: espressif32; framework: arduino)
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/yb-esp32-s3-eth-n8r8.html
PLATFORM: Espressif 32 (6.1.0) > YB-ESP32-S3-ETH-N8R8 (8 MB QD Flash, 8 MB Octal SPI PSRAM)
HARDWARE: ESP32S3 240MHz, 320KB RAM, 8MB Flash
DEBUG: Current (esp-builtin) On-board (esp-builtin) External (cmsis-dap, esp-bridge, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20007.0 (2.0.7) 
 - tool-esptoolpy @ 1.40500.0 (4.5.0) 
 - tool-mkfatfs @ 2.0.1 
 - tool-mklittlefs @ 1.203.210628 (2.3) 
 - tool-mkspiffs @ 2.230.0 (2.30) 
 - toolchain-riscv32-esp @ 8.4.0+2021r2-patch5 
 - toolchain-xtensa-esp32s3 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 62 compatible libraries
Scanning dependencies...
Dependency Graph
|-- Ethernet @ 2.0.0
Building in release mode
Compiling .pio\build\n8r8\src\main.cpp.o
Linking .pio\build\n8r8\firmware.elf
Retrieving maximum program size .pio\build\n8r8\firmware.elf
Checking size .pio\build\n8r8\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   6.2% (used 20344 bytes from 327680 bytes)
Flash: [=         ]   8.8% (used 295089 bytes from 3342336 bytes)
Building .pio\build\n8r8\firmware.bin
esptool.py v4.5
Creating esp32s3 image...
Merged 2 ELF sections
Successfully created esp32s3 image.
Configuring upload protocol...
AVAILABLE: cmsis-dap, esp-bridge, esp-builtin, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
CURRENT: upload_protocol = esptool
Looking for upload port...
Using manually specified: COM7
Uploading .pio\build\n8r8\firmware.bin
esptool.py v4.5
Serial port COM7
Connecting....
Chip is ESP32-S3 (revision v0.2)
Features: WiFi, BLE
Crystal is 40MHz
MAC: 48:27:e2:e8:9a:68
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 921600
Changed.
Configuring flash size...
Flash will be erased from 0x00000000 to 0x00003fff...
Flash will be erased from 0x00008000 to 0x00008fff...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Flash will be erased from 0x00010000 to 0x00058fff...
Compressed 15040 bytes to 10333...
Writing at 0x00000000... (100 %)
Wrote 15040 bytes (10333 compressed) at 0x00000000 in 0.2 seconds (effective 495.0 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 146...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (146 compressed) at 0x00008000 in 0.0 seconds (effective 832.6 kbit/s)...
Hash of data verified.
Compressed 8192 bytes to 47...
Writing at 0x0000e000... (100 %)
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.1 seconds (effective 1239.1 kbit/s)...
Hash of data verified.
Compressed 295456 bytes to 166046...
Writing at 0x00010000... (9 %)
Writing at 0x0001bdd2... (18 %)
Writing at 0x00024856... (27 %)
Writing at 0x0002a32d... (36 %)
Writing at 0x0002f96c... (45 %)
Writing at 0x00034d30... (54 %)
Writing at 0x0003a7b5... (63 %)
Writing at 0x00043e7f... (72 %)
Writing at 0x0004c14a... (81 %)
Writing at 0x000517e1... (90 %)
Writing at 0x0005751a... (100 %)
Wrote 295456 bytes (166046 compressed) at 0x00010000 in 2.3 seconds (effective 1046.6 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...

Please build project in debug configuration to get more details about an exception.
See https://docs.platformio.org/page/projectconf/build_configurations.html

--- Terminal on COM7 | 115200 8-N-1
--- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time        
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
23:27:28.299 > 
23:27:28.299 > Please make sure Ethernet cable is connected between board and switch and DHCP service is available in your LAN.
23:27:28.307 > blinkTask has started.
23:27:28.860 > Ethernet link is down.
23:27:29.861 > Ethernet link is down.
....
```
## Remarks:  
- All YB-ESP32-S3-ETH boards delivered are already flashed with software example 'Test-ESP32-S3-ETH-DHCP'. Means the status LED 'IO47' blinks fast whith power applied and blinks slow with obtained IP address (DHCP via Ethernet).
