# xsvfduino
An Arduino as an XSVF player to program CPLD's and FPGA's based on a reference implementation by Xilinx.

WARNING: (WIP) This is working for me on an STM32 Arduino clone (the blue pill) but it is ugly, and probably will take a lot of effort before it becomes a decent tool. I used the bluepill because it was lying on my benchtop and I had nothing better to lay my hands on. Use it at your own risk and don't blame me if it bricks your beloved CPLD/FPGA, or if it scares your dog, etc.
Be aware that most devices out there are 3.3V or less (some are 1.8V) and not 5V-tolerant, so make sure to use adequate level shifters.
