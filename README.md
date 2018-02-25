# xsvfduino
An Arduino as an XSVF player to program CPLD's and FPGA's based on a reference implementation by Xilinx. It works by receiving an XSVF file via the USB-serial interface, interpreting it as a sequence of instructions, and then bit banging the JTAG signals that are needed to program the target device.

WARNING: This is a very early WIP, almost a proof of concept. It is working for me on an STM32 Arduino clone (the Blue Pill) but it's ugly, and probably will take a lot of effort before it becomes a decent tool. I tested it to program a Xilinx XC2C64A CPLD with success. I chose the Blue Pill because I had one taking dust on my benchtop, it has enough memory to hold the code and the data, and it's darn cheap. 

Be aware that most devices out there are 3.3V or less (some are 1.8V) and are not 5V-tolerant, so make sure you use adequate level shifters, if it's the case. The Blue Pill runs on 3.3V and my CPLD can take it, so I hooked it up directly to the GPIO ports.

DISCLAIMER: Use it at your own risk and don't blame me if it bricks your beloved CPLD/FPGA, or if it scares your dog, etc. It probably just won't work, but who knows?

