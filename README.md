xsvfduino
=========

An Arduino (currently the Blue Pill variant) as an XSVF player to program CPLD's and FPGA's based on a reference implementation by Xilinx. It works by receiving an XSVF file via the USB-serial interface, interpreting it as a sequence of instructions, and then bit banging the JTAG signals that are needed to program the target device.

INSTRUCTIONS
============

1. Know your Arduino: http://wiki.stm32duino.com/index.php?title=Blue_Pill
2. Download the project contents somewhere on your computer.
3. If you don't have it, install Daniel Fekete's Generic STM32 support for Arduino available from https://github.com/danieleff/STM32GENERIC
4. Open the xsvduino.ino on the Arduino IDE
5. On the tools menu, use the following settings: 
   Board: "BluePill F103CB", 
   USB: "Serial [Virtual COM port, pins PA11/PA12 pins]", 
   Serial Communication: "SerialUSB"
6. On the same menu, choose your upload method. I find the ST-Link method to be the easiest, but look here for other options: https://medium.com/@paramaggarwal/converting-an-stm32f103-board-to-a-black-magic-probe-c013cf2cc38c
7. Unplug the ST-Link programmer if you are using one, and then plug the Blue Pill to your computer with a USB cable. It should be seen as a new serial device. On OSX this is something like /dev/tty.usbmodem1411, and on Linux something like /dev/ttyACM0. I'm not sure how this works on Windows, but I guess a new COM port should present itself to you.
8. Find the send_xsvf tool on the tools folder. This a Python script to upload a file to the xsvfduino.
9. Upload your XSVF file using a command like this:
`./send_xsvf -p /dev/tty.your_arduino_serial_port somefile.xsvf`

JTAG INTERFACE PINOUT
=====================

This is defined in ports.h:

- **TCK** -- PB6 (output)
- **TMS** -- PB7 (output)
- **TDI** -- PB8 (output)
- **TDO** -- PB9 (input with internal pullup)

WARNING
=======
This is a very early WIP, almost a proof of concept. It is working for me on an STM32 Arduino clone (the Blue Pill) but it's ugly, and probably will take a lot of effort before it becomes a decent tool. I tested it to program a Xilinx XC2C64A CPLD with success. I chose the Blue Pill because I had one taking dust on my benchtop, it has enough memory to hold the code and the data, and it's darn cheap. 

Be aware that most devices out there are 3.3V or less (some are 1.8V) and are not 5V-tolerant, so make sure you use adequate level shifters, if it's the case. The Blue Pill runs on 3.3V and my CPLD can take it, so I hooked it up directly to the GPIO ports.

DISCLAIMER
==========
Use it at your own risk and don't blame me if it bricks your beloved CPLD/FPGA, or if it scares your dog, etc. It probably just won't work, but who knows?

CREDITS
=======
This project was inspired by Mike Tsao's [JTAGWhispeer](https://github.com/sowbug/JTAGWhisperer) which sadly seems to have been discontinued. Mine is a crude attempt at picking up where Mike has left off.
