## YB-ESP32-S3-DRV Development Board Overview:
The **YB-ESP32-S3-DRV** is a development board based on Espressif's ESP32-S3 MCU and especially designed for driving a wide range of loads including **solenoids**, **stepper motors (e.g. 28BYJ-48)**, **high power LEDs**, etc. It features an **ESP32-S3-WROOM-1-N8R2** module (8MB Flash, 2MB PSRAM, WiFi PCB antenna) and is available on sales platforms [eBay](https://www.ebay.de/sch/i.html?_nkw=yb-esp32-s3) and [Ricardo.ch](https://www.ricardo.ch/en/s/YB-ESP32-S3).

Apart from the Espressif **ESP32-S3** MCU it provides a modern driver IC [Toshiba TBD62083A](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/TBD62083A_datasheet.pdf) **8-channel sink type DMOS transistor array**  similar to an ULN2003/ULN2803 (7/8-channel Darlington array) but with 8 MOSFET output stages, **USB-C connector** for software upload, serial output and/or feeding power to the ESP32 section on the board, **two status LEDs** and **GPIO pins** for free use. The USB-C is connected directly to the ESP32-S3 to allow for serial output and JTAG debugging. 

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_board_top.jpg" height="170"/></p>

There are plenty of GPIOs available so you can connect additional hardware to the board, e.g. TFT displays, buttons, switches, sensors, modules that communicate via I2C/SPI, etc. Please have a look at the provided [examples](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-DRV/examples) for PlatformIO resp. ArduinoIDE. 

The densly populated YB-ESP32-S3-DRV board is still highly [breadboard compatible](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_on_breadboard.jpg) for it leaves one row of accessible breadboard contacts on either side of the board. All I/O ports (GPIOx) are clearly labeled on both sides of the board. 

### Driving inductive loads

The boards driver IC is especially suited for driving inductive loads, e.g. **solenoids** or **stepper motors**. Since a common microcontroller can only drive a few 10mA and low voltages it is commonly paired with a driver IC that can handle higher current and/or voltage levels, e.g. the very popular ULN2003/ULN2803. Such chip can drive up to 500mA current and contains suppression (flyback/freewheeling) diodes. A major disadvantage of the ULN are the darlington transistors at the output which cause a high forward voltage when switched on and therefore a considerable waste of energy. To make matters worse, the high forward voltage (Vf) of about 1.5-2V notably reduces the working voltage of a connected device.  

This board features the **modern 8-channel Toshiba driver chip** with MOSFET outputs (~0.5Vf) and suppression diodes on each channel. 

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/Driver-ICs_compared.jpg)

Furthermore the board provides **two JST connectors** that match exactly the 5-wire connector of a common 5V/12V [**28BYJ-48 stepper motor**](doc/28BYJ-48-Stepper-Motor-Spec.pdf). This lets you easily plug two such motors into the board without the need for any extra wiring between board and steppers.

In addition, the board features **eight LEDs** (4 per connector) that indicate [channel activity](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_board_with_two_5V-steppers.mp4) and therefore provides a clear visual feedback as motors step or solenoids and other connected devices get activated. However, the LEDs stay dark when solder bridges LEDX/LEDY are opened.

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_board_with_two_5V-steppers.jpg)

**Please note:**  The USB cable connected to the board in the pic above in this scenario is only for data communication with a PC! The power for the two 5V stepper motors and the dev board here is provided by the breadboard power supply visible on the right hand side.

## YB-ESP32-S3-DRV board features in detail:
 - **ESP32-S3-WROOM-1-N8R2** module with 8MB Flash, 2MB PSRAM, WiFi PCB antenna
 - **Toshiba TBD62083A** driver IC, an 8-channel sink type DMOS transistor array with integrated suppression diodes for inductive load driving. Each channel is rated at 500mA max. output current resp. 50V max. output voltage. Outputs can be paralleled for higher current. This driver chip is similar to the commonly used ULN2803 but offers reduced forward voltage Vf at each output (when on) by using a DMOS MOSFET transistor instead of darlingtons. The chip is connected to the ESP32-S3 as follows:
   - *GPIO35...GPIO38* - for driving the 4-channel 'X' socket (X1...X4)
   - *GPIO4...GPIO7* - for driving the 4-channel 'Y' socket (Y1...Y4)

    Please note: The maximal power dissipation Pd of the chip must not exceed 0.96W at any time. If needed, a heat sink can be mounted on the chip. Folder [doc](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-DRV/doc) holds the driver IC's spec sheet.
 - **2 x JST XH connector** with 4 output channels each. They match exactly the 5-wire connector of a common 28BYJ-48 stepper motor.
 - **control LEDs**. One LED labeled 'P' is connected to the 3.3V rail to indicate board power and the other LED labeled 'IO47' is connected to GPIO47 which can be used as status LED.  
 - **USB-C** port connected directly to the ESP32-S3 for serial output, software upload (e.g. via ArduinoIDE, VSCode/PlatformIO etc) or JTAG debugging. Can also be used to power the onboard ESP32-S3 modul.

 - **Two pushbuttons**. One is labeled 'R' and resets the ESP32-S3 (shorts EN pin to ground) and the other one is labeled 'B' and shorts GPIO0 to ground when pressed. The latter is sometimes needed to force the board into boot mode.
 - **lots of available GPIOs**, useful for connecting LEDs, buttons, additional modules via SPI or I2C, e.g displays, rotary encoders, bluetooth, etc.

In summary, the board replaces a lot of separate modules & cables, saves space on a breadboard and might avoid the odd frustration to get it all running.  

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_modules_replacement.jpg)

## Board Pin Layout:
![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_pinlayout.jpg)  

The boards **pin layout**, **outline** and **schematic** files are all located in folder [doc](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-DRV/doc).

## Powering the board:
### ESP32 section:
There are different ways to provide power to the onboard ESP32-S3 module:
  - through the USB-C port or
  - 5VDC applied to the 5V pin  

The board uses a LDO to drop the external supply voltage to 3.3Volt needed by the ESP32-S3. 

### Driver section:
Board pins labeled *+V* are intended to provide power to external devices. They are connected to pin 5 of each XH connector. Since all components on this +V rail (C1, C6...C9, IC1) are rated for 50Vmax you might go even higher than 12VDC if necessary.  

**Important:** you must make sure your project never exceeds the electrical and thermal limits of the driver chip IC1.

### Recommended wiring for greater loads:
When the driver chip is supposed to carry more then just a few 10mA on each output channel than it is recommended to use a capable power supply on the +V rail and preferably an additional capacitor of 220uF or greater value across the +V/GND pins of the board as shown below:  

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_recommended_wiring.jpg)  

## Application hints: 

### Arduino IDE:
As of Arduino ESP32 Core V3.2.1 you open the board list, enter "yb-" and select "**Yellobyte YB-ESP32-S3-DRV**". Then choose the proper settings for COM port, debug level, flash size, PSRAM, etc. as shown below. Be aware, since the ESP32-S3 MCU is very versatile there are a lot of build options to play with. Espressif's homepage offers some help.

Correct **ArduinoIDE** settings for the **YB-ESP32-S3-DRV** board:  
- Board: *YB-ESP32-S3-DRV*
- USB CDC On Boot: *Enabled*
- Flash Size: *8MB (64Mb)*
- Partition Scheme: *8MB with spiffs (...)*
- PSRAM: *QSPI PSRAM*  
  
![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_ArduinoIDE_board_selection.jpg)

### PlatformIO:
Building with **PlatformIO** is easy as well. Starting with Arduino ESP32 Core v3.2.1 the VSCode/PlatformIO IDE provides the necessary variant file (pins_arduino.h) and board definition file (yb_esp32s3_drv.json).   

Just create a new project, give it a name, go to board selection, enter "yb-" and finally choose the YB-ESP32-S3-DRV board from the list that's popping up.

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/YB-ESP32-S3-DRV_PlatformIO_board_selection.jpg)

**Note:** If you build with an older platform release and the board is missing in the board list then please copy the provided files (pins_arduino.h & yb_esp32s3_drv.json) to PlatformIO's respective folders on your PC. 

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

The -N8R2 module on the board should provide enough memory even for demanding projects. Building e.g. software example [Stepper-1](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-DRV/examples/PlatformIO/Stepper-1) shows the following Flash/RAM usage:  
```
Executing task: C:\Users\tj\.platformio\penv\Scripts\platformio.exe run 

Processing release (platform: https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip; framework: arduino; board: yb_esp32s3_drv)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/yb_esp32s3_drv.html
PLATFORM: Espressif 32 (53.3.12) > YelloByte YB-ESP32-S3-DRV
HARDWARE: ESP32S3 240MHz, 320KB RAM, 8MB Flash
DEBUG: Current (cmsis-dap) External (cmsis-dap, esp-bridge, esp-builtin, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES:
 - framework-arduinoespressif32 @ 3.1.2
 - framework-arduinoespressif32-libs @ 5.3.0+sha.489d7a2b3a
 - tool-esptoolpy @ 4.8.6
 - tool-mklittlefs @ 3.2.0
 - tool-riscv32-esp-elf-gdb @ 14.2.0+20240403
 - tool-xtensa-esp-elf-gdb @ 14.2.0+20240403
 - toolchain-riscv32-esp @ 13.2.0+20240530
 - toolchain-xtensa-esp-elf @ 13.2.0+20240530
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 42 compatible libraries
Scanning dependencies...
Dependency Graph
|-- Stepper @ 1.1.3
Building in release mode
Retrieving maximum program size .pio\build\release\firmware.elf
Checking size .pio\build\release\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   6.1% (used 20016 bytes from 327680 bytes)
Flash: [=         ]   9.2% (used 306876 bytes from 3342336 bytes)
```

### Integrating this board into your own PCB design projects:

Its easy. Folder [doc](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-DRV/doc) provides the necessary Eagle library file **_yb-esp32-S3-drv.lbr_** containing the boards symbol and footprint. Most other PCB design software (e.g. KiCad) are able to import and use Eagle library files. 

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/Eagle_project_with_yb-esp32-s3-drv.jpg" height="250"/>&nbsp;<img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/boards/YB-ESP32-S3-DRV/doc/Eagle_project_with_yb-esp32-s3-drv2.jpg" height="250"/></p>

<!--## Final Remark:  -->

### General notes:

1) The ESP32-S3-WROOM-1 module family comprises several [**versions**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/ESP32_specs_and_manuals/ESP32-S3-WROOM-1(U)_Variants.jpg). The **-1** versions come with embedded PCB antenna, the **-1U** versions with IPEX antenna socket instead. The extension -Nx(Ry) defines the amount of integrated FLASH/PSRAM, e.g. -N4 (4MB Flash, no PSRAM), -N4R2 (4 MB Flash, 2MB PSRAM), -N8R2 (8 MB Flash, 2MB PSRAM) etc.  

2) The board supports USB Serial JTAG debugging. An example is provided [here](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YB-ESP32-S3-DRV/examples/PlatformIO/Debug-via-builtin-JTAG). 

3) If you want to learn more about stepper motors in general and how to drive them please have a look at the following very informative websites:  
https://randomnerdtutorials.com/stepper-motor-esp32-web-server/  
https://www.instructables.com/Controlling-a-Stepper-Motor-with-an-Arduino/  
https://www.instructables.com/How-to-Control-28BYJ-48-Stepper-Motor-With-ULN2003/  
https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/  




