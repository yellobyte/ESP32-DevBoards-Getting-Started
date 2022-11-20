# Required preparations for online debugging a ESP32 board:

## Scenario 1: Debugging an ESP32-S3 via builtin JTAG adapter:

ESP32-S3 dev boards featuring two USB ports can be debugged without usage of an external JTAG adapter (e.g. ESP32-Prog). Only a few steps need to be done:

### a) Connect both USB ports on the dev board with the computer:  

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/Debugging_via_2nd_USB_port.jpg" height="260"/></p>  

With both USB ports connected you see two additional COM ports and a usb device "USB JTAG/serial debug unit". Your operating system might automatically assign a default driver to it. In most cases it's the wrong one and useless for our purposes.

### b) Assign libwdi driver to device "USB JTAG/serial debug unit":  
A special driver called **libwdi** (ex WinUSB) needs to be assigned to the device. There are two ways to do it:
- by using the well known **Zadig** tool and choosing the device "USB JTAG/serial debug unit (Interface 2)" or 
- by replacing the default driver with the one from the provided  [**zip file**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/debugging/JTAG%20adapter%20driver) (containing the original Espressif driver). Unzip the file content into a temporary folder and perform the necessary steps to install the driver. Under Windows just start device manager, right click on the device and select "driver update" -> "Search for driver on my PC" and select the just created temp folder.

After successfully installing the driver view the device properties and check if the driver has been assigned ok. As an example, on my Windows PC it looks like this:

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/Installation_WinUSB_for_builtin_JTAG_espressif_driver.jpg)

### c) Add a few needed lines to the [env:debug] section of the project's platformio.ini file: 
This requests OpenOCD tool be installed (if it's still missing) and sets the proper values for debugging.
```
[env:debug]
platform_packages = platformio/tool-openocd
debug_tool = custom
;debug_server = 
debug_port = localhost:3333
; for initial breakpoint at setup()
debug_init_break = tbreak setup
; for initial breakpoint at loop()
;debug_init_break = break loop
build_type = debug
```
Have a look at the platformio.ini file of this [**example**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YD-ESP32-S3(VCC-GND.COM)/examples/Test-YD-ESP32-S3-N8R2_Debug_via_builtin_JTAG) to get more info.

## Scenario 2: Debugging an ESP32 using the external JTAG adapter "ESP32-Prog":

Debugging an ESP32 development board featuring only one USB port for flushing and serial output requires an external JTAG adapter. 

A widely used and affordabale external JTAG adapter is Espressif's own development and debugging tool **ESP32-Prog**. This JTAG adapter connects to the computer via USB and on the other side via it's JTAG connector to the four ESP32 JTAG pins (TDO, TDI, TCK, TMS) and usually dev board GND.

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/JTAG adapter docs/ESP-Prog.jpg" height="300"/></p>

Espressifs own webpage [ESP32-Prog Reference]( https://espressif-docs.readthedocs-hosted.com/projects/espressif-esp-iot-solution/en/latest/hw-reference/ESP-Prog_guide.html) provides a very comprehensive introduction to the ESP32-Prog adapter. Above picture has been taken from there.

### a) Connect the ESP32-Prog adapter to the computer:
A new and unknown device will appear "Dual RS232-HS ......".

### b) Assign libwdi driver to device "Dual RS232-HS (Interface 0)":  
A special driver called **libwdi** (ex WinUSB) needs to be assigned to the device.  

Easiest way is to use the **Zadig** tool. Start it as Administrator, activate "Show all devices" and a list of various devices should bee seen.  Choose the device "Dual RS232-HS (Interface 0)", select "WinUSB (v6.1.7600.16385)" and hit the button "Replace Driver".

### c) Connect the ESP32-Prog adapter to the ESP32 target board as shown below:

<p align="center"><img src="https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/ESP-Prog_wiring_to_ESP32_dev_board.jpg" height="500"/></p>

### d) Add some needed lines to the [env:debug] section of the project's platformio.ini file: 
```
[env:debug]
platform_packages = platformio/tool-openocd
; tells the debugger which JTAG adapter is used
debug_tool = esp-prog
; sets an initial breakpoint at setup()
debug_init_break = tbreak setup
; sets an initial breakpoint at loop()
;debug_init_break = break loop
build_type = debug
```

# Start the OpenOCD task:
GDB needs the OpenOCD task to run in the background before debugging can be started. It connects GDB with the various JTAG adapters and seems to act as an abstraction layer. Simplest way to start OpenOCD is entering the following command in a PlatformIO CLI window:  
&nbsp;&nbsp;&nbsp;&nbsp; $ pio run --target openocd --environment debug  
 
The alternative way is to prepare a script file with above command and then add the line "extra_scripts = name_of_script_file.py" to the [env] section in platformio.ini. 
With this done you only need to hit the respective entry under "PlatformIO -> PROJECT TASKS -> Custom" every time you want to start OpenOCD. Have a look at the file [**add_tasks.py**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/blob/main/boards/YD-ESP32-S3(VCC-GND.COM)/examples/Test-YD-ESP32-S3-N8R2_Debug_via_builtin_JTAG/add_tasks.py) in the examples root directory for further details.

**Important:**  
The automatically created "Start OpenOCD (debug) task" terminal logs errors, warnings and basic infos while OpenOCD is starting up and running.   
If any errors show up indicating a problem with OpenOCD or with it's actions performed then I urge you to find the problems and fix them, otherwise you definitely will come a gutser later on.

A successful start of OpenOCD without any errors is shown below:
```
Executing task: C:\Users\tj\.platformio\penv\Scripts\platformio.exe run --target openocd --environment debug 

Processing debug (platform: espressif32; board: esp32-s3-devkitc-1; framework: arduino)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/esp32-s3-devkitc-1.html
PLATFORM: Espressif 32 (5.2.0) > Espressif ESP32-S3-DevKitC-1-N8 (8 MB QD, No PSRAM)
HARDWARE: ESP32S3 240MHz, 320KB RAM, 8MB Flash
DEBUG: Current (custom) On-board (esp-builtin) External (cmsis-dap, esp-bridge, esp-prog, 
...
...
Building in release mode
C:\Users\tj\.platformio\packages\tool-openocd-esp32\bin\openocd.exe -f board\esp32s3-builtin.cfg
Open On-Chip Debugger  v0.11.0-esp32-20220706 (2022-07-06-15:48)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : only one transport option; autoselect 'jtag'
Info : esp_usb_jtag: VID set to 0x303a and PID to 0x1001        
Info : esp_usb_jtag: capabilities descriptor set to 0x2000      
Warn : Transport "jtag" was already selected
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : esp_usb_jtag: serial (7C:DF:A1:F2:FD:28)
Info : esp_usb_jtag: Device found. Base speed 40000KHz, div range 1 to 255
Info : clock speed 40000 kHz
Info : JTAG tap: esp32s3.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : JTAG tap: esp32s3.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : [esp32s3.cpu0] Target halted, PC=0x40000400, debug_reason=00000000
Info : [esp32s3.cpu1] Target halted, PC=0x40000400, debug_reason=00000000
Info : starting gdb server for esp32s3.cpu0 on 3333
Info : Listening on port 3333 for gdb connections
```
Above lines state that OpenOCD found the ESP32-S3 builtin JTAG interface, connected successfully to it, identified 2 ESP32 cores, stopped the target chip, opened 3 tcp sockets and started listening on them.  
Looking at the task manager at this point shows that the OpenOCD task is running and the GDB task is still not. 

# Start the GDB:
Select PlatformIO -> Run -> Start Debugging or simply press F5. This will start the GDB and the actual ESP32 debugging session begins.  

The "Start OpenOCD (debug) task" terminal (if still kept open) continues with logging the interaction between GDB and OpenOCD, giving you always an idea whats going on:

```
Info : Listening on port 3333 for gdb connections
Info : accepting 'gdb' connection on tcp/3333       <---------- GDB started !!!
Warn : No symbols for FreeRTOS!
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : Set GDB target to 'esp32s3.cpu0'
Info : Flash mapping 0: 0x10020 -> 0x3c030020, 50 KB
Info : Flash mapping 1: 0x20020 -> 0x42000020, 134 KB
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : Auto-detected flash bank 'esp32s3.cpu0.flash' size 8192 KB
Info : Using flash bank 'esp32s3.cpu0.flash' size 8192 KB
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : Flash mapping 0: 0x10020 -> 0x3c030020, 50 KB
Info : Flash mapping 1: 0x20020 -> 0x42000020, 134 KB
Info : Using flash bank 'esp32s3.cpu0.irom' size 136 KB
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : Flash mapping 0: 0x10020 -> 0x3c030020, 50 KB
Info : Flash mapping 1: 0x20020 -> 0x42000020, 134 KB
Info : Using flash bank 'esp32s3.cpu0.drom' size 52 KB
Info : New GDB Connection: 1, Target esp32s3.cpu0, state: halted
Info : JTAG tap: esp32s3.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : JTAG tap: esp32s3.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : [esp32s3.cpu0] requesting target halt and executing a soft reset
Info : [esp32s3.cpu0] Target halted, PC=0x400559E3, debug_reason=00000000
Info : [esp32s3.cpu1] requesting target halt and executing a soft reset
Info : [esp32s3.cpu0] Core was reset.
Info : [esp32s3.cpu0] Target halted, PC=0x40000400, debug_reason=00000000
Info : [esp32s3.cpu1] Core was reset.
Info : [esp32s3.cpu1] Target halted, PC=0x40000400, debug_reason=00000000
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : Flash mapping 0: 0x10020 -> 0x3c030020, 50 KB
Info : Flash mapping 1: 0x20020 -> 0x42000020, 134 KB
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : Auto-detected flash bank 'esp32s3.cpu1.flash' size 8192 KB
Info : Using flash bank 'esp32s3.cpu1.flash' size 8192 KB
** Programming Started **
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : PROF: Erased 16384 bytes in 389.066 ms
Info : PROF: Data transferred in 173.722 ms @ 92.1012 KB/s
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : PROF: Wrote 16384 bytes in 569.57 ms (data transfer time included)
** Programming Finished in 1790 ms **
** Verify Started **
Info : [esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Info : PROF: Flash verified in 197.785 ms 
** Verify OK **
Info : JTAG tap: esp32s3.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : JTAG tap: esp32s3.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
...and much more...
```

The DEBUG CONSOLE as well starts to produce a lot of similar output:
```
Reading symbols from d:\Projects\VSCode\ESP32\ESP32-S3 YD-DevBoard (VCC-GND.COM)\Test-ESP32-S3-N8R2_Debug_via_builtin_JTAG\.pio\build\debug\firmware.elf...
PlatformIO Unified Debugger -> https://bit.ly/pio-debug
PlatformIO: debug_tool = custom
PlatformIO: Initializing remote target...
0x40000400 in ?? ()
JTAG tap: esp32s3.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
JTAG tap: esp32s3.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
[esp32s3.cpu0] requesting target halt and executing a soft reset
[esp32s3.cpu0] Debug controller was reset.
[esp32s3.cpu0] Core was reset.
[esp32s3.cpu0] Target halted, PC=0x500000EF, debug_reason=00000000
[esp32s3.cpu1] requesting target halt and executing a soft reset
[esp32s3.cpu0] Core was reset.
[esp32s3.cpu0] Target halted, PC=0x40000400, debug_reason=00000000
[esp32s3.cpu1] Debug controller was reset.
[esp32s3.cpu1] Core was reset.
[esp32s3.cpu1] Target halted, PC=0x40000400, debug_reason=00000000
[esp32s3.cpu0] Target halted, PC=0x403B2482, debug_reason=00000001
Flash mapping 0: 0x10020 -> 0x3c030020, 50 KB
Flash mapping 1: 0x20020 -> 0x42000020, 134 KB
...
Temporary breakpoint 1 at 0x420014fc: file src/main.cpp, line 14.
PlatformIO: Initialization completed
PlatformIO: Resume the execution to `debug_init_break = tbreak setup`
PlatformIO: More configuration options -> https://bit.ly/pio-debug
Note: automatically using hardware breakpoints for read-only addresses.
[esp32s3.cpu1] Debug controller was reset.
[esp32s3.cpu1] Core was reset.
...
...
Thread 
1 "IDLE" received signal SIGINT, Interrupt.
0x42020fea in esp_pm_impl_waiti () at /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/hal/esp32s3/include/hal/cpu_ll.h:182
182	in /Users/ficeto/Desktop/ESP32/ESP32S2/esp-idf-public/components/hal/esp32s3/include/hal/cpu_ll.h
[esp32s3.cpu0] Target halted, PC=0x4037BA92, debug_reason=00000000
Set GDB target to 'esp32s3.cpu0'
[esp32s3.cpu1] Target halted, PC=0x4200156E, debug_reason=00000001
[Switching to Thread 1070477408]

Thread 3 "loopTask" hit Breakpoint 10, loop () at src/main.cpp:34
34	  digitalWrite(GPIO_USED, !digitalRead(GPIO_USED));
...
...and so on...
```
It's best to always keep an eye on both outputs as it provides a lot of useful information that might help you.  
At this stage you know everything is configured and set up properly. You are ready for action.

## Start debugging your program:

Once GDB has started you can set breakpoints, watch variables and much much more. A comprehensive & detailed introduction into debugging with PlatformIO/GDB is given [**here**](https://docs.platformio.org/en/stable/plus/debugging.html#debugging-tools). 

Just as an example below a snapshot of my latest debugging session with an [**YD-ESP32-S3-N8R2**](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/tree/main/boards/YD-ESP32-S3(VCC-GND.COM)) dev board featuring 2 USB ports and an ESP32-S3 with builtin JTAG adapter:

![](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/debugging/DebugSession_using_ESP32-S3_builtin_JTAG.jpg)
