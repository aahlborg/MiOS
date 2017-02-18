# MiOS - A Raspberry Pi bare metal system

This code was written by me while trying to learn about the inner secrets of the Raspberry Pi, ARM assembler and embedded programming in general. It serves as a help for me to describe the most fundamental parts of embedded software, and I hope that it can be of use to others trying to learn about these subjects.

## Building
To build this code you need an ARM bare metal toolchain. I use the gcc-arm-none-eabi toolchain that comes with Ubuntu but other versions should work just fine.

Simply running make will create a build/kernel7.img that can be compied to an SD card which has been prepared with a boot partition and the raspberry pi bootloader.

## Licence
The code is provided as-is and you are free to use it for any purpose. I will provide a more formal licence soon, probably BSD.

## References
### Tutorials
- http://www.valvers.com/open-software/raspberry-pi/step01-bare-metal-programming-in-cpt1
- http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os

### Documentation
- http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
- https://github.com/raspberrypi/firmware/wiki/Mailboxes

### Multicore
- http://stackoverflow.com/questions/28610045/how-to-start-additional-cores-on-the-raspberry-pi-2-under-bare-metal
- https://github.com/jeffreyantony/multipi

### Interrupts
- https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=38076
- http://www.valvers.com/open-software/raspberry-pi/step04-bare-metal-programming-in-c-pt4
- https://github.com/dwelch67/raspberrypi/tree/master/blinker05
- https://www.ece.umd.edu/class/enee447.S2016/ARM-Documentation/ARM-Interrupts-1.pdf

### JTAG
- http://sysprogs.com/VisualKernel/tutorials/raspberry/jtagsetup

### Similar projects
- https://github.com/SharpCoder/rpi-kernel
- https://github.com/dwelch67/raspberrypi
