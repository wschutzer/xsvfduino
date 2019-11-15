xsvfduino
=========

An Arduino (currently the Blue Pill variant) as an XSVF player to program CPLD's and FPGA's based on a reference implementation by Xilinx (Keep reading for Altera). It works by receiving an XSVF file via the USB-serial interface, interpreting it as a sequence of instructions, and then bit banging the JTAG signals that are needed to program the target device.

I have tested and successfully programmed the following CPLDs:

- Xilinx XC2C64A
- Xilinx XC9572XL
- Altera MAX II EPM240T100C5 (I used dangerous prototypes svf2xsvf converter to convert Altera's svf file to the compressed xsvf file, although that program doesn't claim to support this device)

INSTRUCTIONS
============

1. Know your Arduino: http://wiki.stm32duino.com/index.php?title=Blue_Pill
2. Download the project contents somewhere on your computer.
3. If you don't have it, install Roger Clark's support for Arduino available from https://github.com/rogerclarkmelbourne/Arduino_STM32
4. Open the xsvduino.ino on the Arduino IDE
5. On the tools menu, use the following settings: 
   Board: "Generic STM32F103CB series"
6. On the same menu, choose your upload method. I find the ST-Link method to be the easiest, but look there for other options or check out this link: https://medium.com/@paramaggarwal/converting-an-stm32f103-board-to-a-black-magic-probe-c013cf2cc38c
7. Unplug the ST-Link programmer if you are using one, and then plug the Blue Pill to your computer with a USB cable. An LED
starts blinking on the Blue Pill it should be seen by the OS as a new serial device. On OSX this is something like /dev/tty.usbmodem1411, and on Linux something like /dev/ttyACM0. I'm not sure how this works on Windows, but I guess a new COM port should present itself to you (hopefully).
8. Find the send_xsvf tool on the tools folder. This a Python script to upload a file to the xsvfduino.
9. Upload your XSVF file using a command like this:
`./send_xsvf -p /dev/tty.your_arduino_serial_port somefile.xsvf`

JTAG INTERFACE PINOUT
=====================

This is defined in ports.h:

- **TCK** -- PB6 (output)
- **TMS** -- PB7 (output)
- **TDI** -- PB8 (output)
- **TDO** -- PB5 (input with internal pullup)

WARNING
=======
This is a very early WIP, almost a proof of concept. It is working for me on an STM32 Arduino clone (the Blue Pill) but it's ugly, and probably will take a lot of effort before it becomes a decent tool. I tested it to program a Xilinx XC2C64A CPLD, a Xilinx XC9572XL and even an Altera MAX II EPM240T100C5 with success. I chose the Blue Pill because I had one taking dust on my benchtop, it has enough memory to hold the code and the data, and it's darn cheap. 

I tried using Daniel Fekete's Generic STM32 support, but unfortunately I came accross some bug in the pinMode/digitalRead functions which caused a pin which had been set as INPUT_PULLUP to read as zero even if nothing was connected to it. Clark's package works fine.

Be aware that most devices out there run on 3.3V or less (some are 1.8V) and might not be 5V-tolerant, so make sure you use adequate level shifters, if they are required for your chip. Otherwise you could see some magic smoke! The Blue Pill runs on 3.3V, the same voltage of all the CPLDs I tested, so I just hooked them up directly to the indicated GPIO pins.

DISCLAIMER
==========
I am not a developer and programming is not my thing. I am posting this in the hopes that it might be useful to someone out there. Use it at your own risk and don't blame me if it bricks your beloved CPLD/FPGA, if it scares your dog, or anything. It probably just won't work as you'd expect, but who knows?

CREDITS
=======
This project was inspired by Mike Tsao's [JTAGWhispeer](https://github.com/sowbug/JTAGWhisperer) which sadly seems to have been discontinued. Mine is a crude attempt at picking up where Mike has left off.
