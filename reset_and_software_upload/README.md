# Auto resetting the board and uploading software via USB

Almost every ESP32 dev board with an USB port hosts a little but nevertheless sophisticated circuitry which is located between the USB-to-UART bridge chip (usually CP2102, FT232 or CH340) and the ESP32. Such a chip has a lot of pins, the 4 most important ones are called **TX** (output, for data sent from PC to the ESP32), **RX** (input, for data sent from ESP32 to the PC), **DTR** (output, legacy "Data Terminal Ready") and **RTS** (output, legacy "Request To Send"). Back in the computer stone age the signals DTR & RTS (among others) were used to control the connection between Computers/Terminals and analog modems.

The whole magic is such that the little tool **esptool** (which is part of the development environment) can independently set those signals to active (0=low) or passive (1=high) which tells the ESP32 to reboot or go into upload mode. 

The above mentioned circuitry is shown below. DTR & RTS are connected to the USB bridge chip and EN & IO0 are connected to the ESP32.

![](ESP32_reset_and_upload_circuitry.jpg)

That way an IDE can work with a dev board without prompting to press any physical buttons, means one can work remotely on a PC that is running the IDE and is connected to a dev board. Lying comfortably with a laptop on a couch at home while the dev board is far away (e.g. in an office) would be possible. In my case I additionally have a USB camera attached which enables me to see the onboard LEDs or displays. :-)  

For the following demonstrations a program is running on the board (or gets uploaded) which simply switches the onboard LED on & off and produces some serial output. After starting the Serial Monitor task in PlatformIO this is shown:

```
--- Terminal on COM4 | 115200 8-N-1
--- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
16:32:43.709 > LED off
16:32:44.887 > LED on
16:32:45.887 > LED off
16:32:46.880 > LED on
....
```
As one can see the serial monitor task just starts to display the serial output from the board and no reset happens.  

## Resetting the board:

The dev board will perform a reset when you enter the key combinations CTRL-T followed by CTRL-D and again CTRL-T followed by CTRL-D as shown below:

```
...
16:33:25.122 > LED off
16:33:26.120 > LED on
--- DTR inactive ---
--- DTR active ---
16:34:21.426 > ets Jun  8 2016 00:22:57
16:34:21.429 >
16:34:21.429 > rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
16:34:21.434 > configsip: 0, SPIWP:0xee
16:34:21.436 > clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
16:34:21.443 > mode:DIO, clock div:2
16:34:21.445 > load:0x3fff0030,len:1184
16:34:21.447 > load:0x40078000,len:13132
16:34:21.449 > load:0x40080400,len:3036
16:34:21.451 > entry 0x400805e4
16:34:21.588 > ESP32-DevKitC-V1 board LED test started. MODE = 0
16:34:21.594 > LED off
16:34:22.581 > LED on
16:34:23.581 > LED off
....
```
How come ? Let's look at the signals on the dev board while doing above key combinations.  

Without serial monitor task being started, DTR & RTS signals are usually both inactive (1=high). Starting the serial monitor will change the signals DTR & RTS to active (0=low). In both cases ESP32 pins EN & IO0 see a logical high(1) and the ESP32 continues to run normally and unaffected by the change.  

Entering CTRL-T followed by CTRL-D for the first time makes the signal DTR change to inactive (1=high). This makes the signal EN go low(0) and the ESP32 stops running. Only after entering CTRL-T followed by CTRL-D for the second time the signal DTR becomes active again (0=low) and the signal EN on the ESP32 rises to high(1). The ESP32 is not resetted anymore and re-starts the program as shown above.

![](Reset-via-USB.jpg)

In some rare cases the dev board resets after the serial monitor task has been started, which might be annoying. But why is it so ?  

Have a look at the signal snapshot below which was taken the moment the serial monitor task was started. As expected both signals DTR & RTS go from inactive (1=high) to active (0=low) but this time with a slight delay in between. This causes a short negative impulse on the EN line and resets the ESP32. Such small impulses happen only now and then though. It took me about 20 serial monitor starts to capture one. 

![](Monitor_Task_started(PlatformIO)-reset.jpg)  

## Uploading software to the board:

When a Development Environment (PlatformIO, Arduino IDE) performs a software upload to the board the same signals are used to put the ESP32 into programming mode before the upload.  

An upload usually looks like this:

```
Configuring upload protocol...
AVAILABLE: cmsis-dap, esp-bridge, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
CURRENT: upload_protocol = esptool
Looking for upload port...
Using manually specified: COM4
Uploading .pio\build\esp32dev\firmware.bin
esptool.py v4.2.1
Serial port COM4
Connecting............
Chip is ESP32-D0WDQ6 (revision 1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: 24:6f:28:b2:11:50
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 921600
Changed.
Configuring flash size...
Flash will be erased from 0x00001000 to 0x00005fff...
Flash will be erased from 0x00008000 to 0x00008fff...
Flash will be erased from 0x0000e000 to 0x0000ffff...
Flash will be erased from 0x00010000 to 0x0004cfff...
Compressed 17440 bytes to 12127...
Writing at 0x00001000... (100 %)
Wrote 17440 bytes (12127 compressed) at 0x00001000 in 0.3 seconds (effective 438.4 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 128...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (128 compressed) at 0x00008000 in 0.0 seconds (effective 611.5 kbit/s)...
Hash of data verified.
Compressed 8192 bytes to 47...
Writing at 0x0000e000... (100 %)
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.1 seconds (effective 865.8 kbit/s)...
Hash of data verified.
Compressed 247776 bytes to 136936...
Writing at 0x00010000... (11 %)
Writing at 0x0001c6f8... (22 %)
Writing at 0x00024ba5... (33 %)
Writing at 0x00029c94... (44 %)
Writing at 0x0002f14c... (55 %)
Writing at 0x00037886... (66 %)
Writing at 0x0003fb24... (77 %)
Writing at 0x00044fb9... (88 %)
Writing at 0x0004a87c... (100 %)
Wrote 247776 bytes (136936 compressed) at 0x00010000 in 2.1 seconds (effective 934.3 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
============================================================== [SUCCESS] Took 8.73 seconds =======================================================
```

The sigal trace shows that in this case Esptool needs a second attempt to put the ESP32 into programming mode. The second time signal IO0 stays low(0) a bit longer. However, the upload then is ok and is trailed by a hard reset accomplished with RTS briefly going active (0=low) while DTR stays inactive (1=high).

![](Software_Upload_Esptool(PlatformIO).jpg)

Zooming in shows in more detail how the ESP32 is put into upload mode. Immediately after the EN pin goes high an ESP32 starts running and checks the status of the IO0 pin first. If high(1) it will run normally, if low(0) it will enter upload mode.

![](ESP32_put_into_upload_mode.jpg)

## Troubleshooting:

Some dev boards notoriously defy to be flashed and all attempts of Esptool to upload software permanently fail. It then looks like this:
```
Configuring upload protocol...
AVAILABLE: cmsis-dap, esp-bridge, esp-prog, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
CURRENT: upload_protocol = esptool
Looking for upload port...
Using manually specified: COM4
Uploading .pio\build\esp32dev\firmware.bin
esptool.py v4.2.1
Serial port COM4
Connecting...............................................
```
Finally Esptool gives up and prints an error message. This happens sometimes with various dev boards but always with the original ESP32-T dev board equipped with an ESP32-Bit module (shown below). Which is very annoying for it is a slim board, can be equipped with different ESP32 modules and therefore I have a preference for it and use it in many projects.

Looking at the signals EN & IO0 the idea came up to delay the reset signal a bit. I gave it a try and soldered a small capacitor of 100nF between EN & GND. This did the trick. Now the automatic software upload with Esptool works flawlessly on all modified ESP32-T boards. Maybe this solution might help with other renitent boards.

![](ESP32-T_board_modified.jpg)

