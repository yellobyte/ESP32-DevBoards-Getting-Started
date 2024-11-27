# Debugging without JTAG

Following a simple example program (running on an ESP32-S3) with two user defined functions, one of them causing a core to panic:
```
int iLoopCount = 0, iVarZero = 0, iVarNotZero = 1, iVarResult;  

void myFunction1(int iParam1) {
  if (iParam1 >= 3) {
    iVarResult = iParam1 / iVarZero;         // <-- crash
  }
}

void myFunction2(int iParam2) {
  iVarResult = iParam2 / iVarNotZero;        // <-- no crash
}

void setup() {
  ...
}

void loop() {
  ++iLoopCount;
  myFunction1(iLoopCount);
  myFunction2(iLoopCount);
  ...
}
```
Your serial monitor connected via USB to a ESP32(-Sx) dev board will show the following core panic message:
```
00:20:48.497 > Guru Meditation Error: Core  1 panic'ed (IntegerDivideByZero). Exception was unhandled.
00:20:48.554 >
00:20:48.554 > Core  1 register dump:
00:20:48.557 > PC      : 0x4200193f  PS      : 0x00060430  A0      : 0x820019bb  A1      : 0x3fcebf40
00:20:48.566 > A2      : 0x00000003  A3      : 0x00000001  A4      : 0x00000000  A5      : 0x0000e100  
00:20:48.574 > A6      : 0x0000002b  A7      : 0x3fc9221c  A8      : 0x00000000  A9      : 0x3fcebf10
00:20:48.574 > A10     : 0x00000000  A11     : 0xffc00000  A12     : 0x00000031  A13     : 0x00000000
00:20:48.587 > A14     : 0x00008000  A15     : 0x00000000  SAR     : 0x00000011  EXCCAUSE: 0x00000006  
00:20:48.589 > EXCVADDR: 0x00000000  LBEG    : 0x400556d5  LEND    : 0x400556e5  LCOUNT  : 0xffffffff
00:20:48.604 >
00:20:48.604 > Backtrace: 0x4200193c:0x3fcebf40 0x420019b8:0x3fcebf60 0x42004515:0x3fcebf80
00:20:48.622 >
00:20:48.622 > ELF file SHA256: 129fad01a0a7044c
00:20:48.622 >
00:20:48.768 > Rebooting...
```
## Why a ESP32 core is panicing:

In most cases it's caused by popular coding errors like 
 - uninitialized pointers, division by zero, missing brackets around code sections, etc.

or in more complicated cases by
 - task watchdogs, stack overflow, illegal Instructions, memory shortage, etc.

Espressif's help page [**Fatal Errors**](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/fatal-errors.html) gives you a much more detailed overview of possible crash reasons.

## Debugging with the ESP32 Exception decoder:

A much appreciated tool called **ESP32 Exception Decoder** can help to pin down the function in which the exception occurred.

### Using ArduinoIDE:

Download the EspExceptionDecoder from [here](https://github.com/me-no-dev/EspExceptionDecoder)  and follow the instructions given.

### Using PlatformIO:

Make sure your project file platformio.ini has the following settings:

monitor_filters = time, colorize, *esp32_exception_decoder*  
build_type = *debug*

In most cases the core panic message in your serial monitor will now look slightly different. You will see a few additional lines, the *call stack*, just below the backtrace info. 
```
00:20:46.493 > ESP32-S3 DEBUG test started.
00:20:48.497 > Guru Meditation Error: Core  1 panic'ed (IntegerDivideByZero). Exception was unhandled.
00:20:48.554 >
00:20:48.554 > Core  1 register dump:
00:20:48.557 > PC      : 0x4200193f  PS      : 0x00060430  A0      : 0x820019bb  A1      : 0x3fcebf40
00:20:48.566 > A2      : 0x00000003  A3      : 0x00000001  A4      : 0x00000000  A5      : 0x0000e100  
00:20:48.574 > A6      : 0x0000002b  A7      : 0x3fc9221c  A8      : 0x00000000  A9      : 0x3fcebf10
00:20:48.574 > A10     : 0x00000000  A11     : 0xffc00000  A12     : 0x00000031  A13     : 0x00000000
00:20:48.587 > A14     : 0x00008000  A15     : 0x00000000  SAR     : 0x00000011  EXCCAUSE: 0x00000006  
00:20:48.589 > EXCVADDR: 0x00000000  LBEG    : 0x400556d5  LEND    : 0x400556e5  LCOUNT  : 0xffffffff
00:20:48.604 >
00:20:48.604 > Backtrace: 0x4200193c:0x3fcebf40 0x420019b8:0x3fcebf60 0x42004515:0x3fcebf80
  #0  0x42004515 in loopTask(void*) at C:/Users/tj/.platformio/packages/framework-arduinoespressif32/cores/esp32/main.cpp:50

00:20:48.622 >
00:20:48.622 > ELF file SHA256: 129fad01a0a7044c
00:20:48.622 >
00:20:48.768 > Rebooting...
```
The serial output shows the actual CPU register values at the time of the crash. Line "Backtrace" below contains PC:SP pairs, with Program Counter (PC) and Stack Pointer (SP) of the current task and further down you see the actual function call stack. 

Under different circumstances you might see a long list of functions in your call stack and you can pinpoint the problem right away (probably a recent change).

Well, in our case it doesn't tell a lot. At least it tells us the exception obviously happened in function loop(). The question is: in which subfunction, where precisely ?  

## Evaluating the map file:

It's time to evaluate the map file which was generated during the build. It holds a lot of debug info which can easily help to find the cause of the crash.  

The following steps are done under PlatformIO but similarly apply to ArduinoIDE and should be understand as a rough guide only. You will get the idea though.

#### 1) Make sure you find the following two files in your project's subdirectory *.pio/build/debug*:
  - *firmware.elf* and *firmware.map*

#### 2) Open *firmware.map* and search for section "Memory Configuration"
In our case it looks like this:
```
Name             Origin             Length             Attributes
iram0_0_seg      0x0000000040374000 0x0000000000058700 xr
iram0_2_seg      0x0000000042000020 0x00000000007fffe0 xr
dram0_0_seg      0x000000003fc88000 0x0000000000054700 rw
drom0_0_seg      0x000000003c000020 0x0000000001ffffe0 r
rtc_iram_seg     0x00000000600fe000 0x0000000000001ff0 xrw
rtc_data_seg     0x00000000600fe000 0x0000000000001ff0 rw
rtc_slow_seg     0x0000000050000000 0x0000000000002000 rw
*default*        0x0000000000000000 0xffffffffffffffff
```
The PC (Program Counter) value at the time of the crash was 0x4200193f which (not unexpectedly) happens to be in section "iram0_2_seg" ranging from 0x42000020 to 0x427FFFFF and holding instructions.  

Since the exception lies within loop() as seen earlier we look further, this time for the entry point of this function. Just to get a first overview.
```
 .text._Z4loopv
                0x00000000420019a0       0x2f .pio\build\debug\src\main.cpp.o
                                         0x3f (size before relaxing)
                0x00000000420019a0                loop()
 *fill*         0x00000000420019cf        0x1 
 .text._ZN14HardwareSerial14_uartEventTaskEPv
                0x00000000420019d0      0x23c .pio\build\debug\libFrameworkArduino.a(HardwareSerial.cpp.o)
                                        0x254 (size before relaxing)
                0x00000000420019d0                HardwareSerial::_uartEventTask(void*)
```
Function loop() stretches from 0x420019a0 to 0x420019ce as the following byte at 0x420019cf is only a filler. 

This tells us that the crash at 0x4200193f doesn't happen directly in function loop() but in a function called from within loop(). Our goal is to find this function which code stretches over address 0x4200193f. Since our software example is so small it pays out to look at the near vicinity of loop() and voilà, a few line earlier we find the following lines in firmware.map:
```
 .text._Z11myFunction1i
                0x0000000042001934       0x19 .pio\build\debug\src\main.cpp.o
                0x0000000042001934                myFunction1(int)
 *fill*         0x000000004200194d        0x3 
 .text._Z11myFunction2i
                0x0000000042001950       0x15 .pio\build\debug\src\main.cpp.o
                0x0000000042001950                myFunction2(int)
 *fill*         0x0000000042001965        0x3 
```
Function myFunction1(int) stretches from address 0x42001934 to 0x4200194c so we will focus on this function. The map file also gives the info as to the source code file which holds this function. In our case it is file *main.cpp*. 

Out of curiosity we could have a look at the (usually very long) *Cross Reference Table* in firmware.map. It confirmes that myFunction1() is only called from within main.cpp(.o) and not from any other module:
```
...
multi_heap_set_lock             C:\Users\tj\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32s3\lib\libheap.a(multi_heap.c.obj)
                                C:\Users\tj\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32s3\lib\libheap.a(heap_caps_init.c.obj)
myFunction1(int)                .pio\build\debug\src\main.cpp.o
myFunction2(int)                .pio\build\debug\src\main.cpp.o
...
```
#### 3) Find out which directory holds the used toolchain programs
For this particular project on my PC it is "C:/Users/tj/.platformio/packages/toolchain-xtensa-esp32s3/bin". Two easy ways to get this info:  
a) Within the project directory go to .vscode, open file *lauch.json* and look for the line starting with "toolchainBinDir":...  
b) Start Sysinternal's tool Procmon and see where the toolchain programs run from

#### 4) Open a command terminal from within the project directory
...and enter the following commands. The following applies to my PC and particular project ! Please change according to your environment.
```
D:\Projects\VSCode\YB-ESP32-S3-AMP\Test-Debug>cd .pio

D:\Projects\VSCode\YB-ESP32-S3-AMP\Test-Debug\.pio>cd build

D:\Projects\VSCode\YB-ESP32-S3-AMP\Test-Debug\.pio\build>cd debug

D:\Projects\VSCode\YB-ESP32-S3-AMP\Test-Debug\.pio\build\debug> C:\Users\tj\.platformio\packages\toolchain-xtensa-esp32s3\bin\xtensa-esp32s3-elf-objdump.exe -S -C -d src\main.cpp.o > main.obj.log
```
This will produce a file with name *main.obj.log*.

#### 5) Open the just created file main.obj.log

Search for the disassembly of function *myFunction1* which obviously caused the core panic.
```
Disassembly of section .text._Z11myFunction1i:

00000000 <myFunction1(int)>:
#define MY_LED_BUILTIN  47

int iLoopCount = 0, iVarZero = 0, iVarNotZero = 1, iVarResult;

void myFunction1(int iParam1)
{
   0:	004136        	entry	a1, 32
  if (iParam1 >= 3) {
   3:	0d32a6        	blti	a2, 3, 14 <myFunction1(int)+0x14>
    iVarResult = iParam1 / iVarZero;         // <-- crash
   6:	000081        	l32r	a8, fffc0008 <myFunction1(int)+0xfffc0008>
   9:	0888      	    l32i.n	a8, a8, 0
   b:	d22280        	quos	a2, a2, a8
   e:	000081        	l32r	a8, fffc0010 <myFunction1(int)+0xfffc0010>
  11:	006822        	s32i	a2, a8, 0
  }
}
  14:	0020c0        	memw
  17:	f01d      	    retw.n
```
We know the PC register value was 0x4200193f when the crash occurred and function myFunction1() starts from address 0x42001934.

Now we subtract the functions start address 0x42001934 from the crash address 0x4200193f which results in the offset 0x0b. Around this offset the following assembler code is listed:
```
   ...
   9:	0888      	    l32i.n	a8, a8, 0
   b:	d22280        	quos	a2, a2, a8
   ...
```
Looks like registers A2 & A8 are part of an instruction which is dividing them with the result staying in A2. However, these instructions belong to this line of code as shown above: 
```
iVarResult = iParam1 / iVarZero;
```
So it seems, we found the culprit. The next logical step would be to have a closer look at your source code and make sure a divider value isn't zero when in action or reverse any hasty code changes you did before you had your last coffee... 

Please note:  
If you are not an assembler freak but want to dig a bit deeper you can always have a look at the Xtensa®Instruction Set Architecture's [reference manual](https://github.com/yellobyte/ESP32-DevBoards-Getting-Started/raw/main/ESP32_specs_and_manuals/Xtensa_Instruction_Set_Architecture.pdf) and look up the exact instruction and what it does. In the above case the manual states the following:
```
Table 4–36. 32-bit Integer Divide Option Instruction Additions
---------------------------------------------------------------------
Instruction1 Format Definition
---------------------------------------------------------------------
QUOS         RRRR   Quotient Signed (divide giving 32-bit quotient)
QUOU         RRRR   Quotient Unsigned (divide giving 32-bit quotient)
....
```

## Conclusion:
There you have it. A little bit of knowledge makes seeing a core crash much less terrifying.  And with only moderate effort in time it is possible to get quickly to the root of the problem.  
Of course, bigger programs create bigger files and more data to evaluate, the main procedure stays the same though.
