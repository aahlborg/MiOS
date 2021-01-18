# MiOS - A Raspberry Pi bare metal system

This code was written by me while trying to learn about the inner secrets of the Raspberry Pi, ARM assembler and embedded programming in general. It serves as a help for me to describe the most fundamental parts of embedded software, and I hope that it can be of use to others trying to learn about these subjects.

## Building

To build this code you need an ARM bare metal toolchain. I use the gcc-arm-none-eabi toolchain that comes with Ubuntu but other versions should work just fine.

Simply running make will create a build/kernel7.img that can be copied to an SD card which has been prepared with a boot partition and the Raspberry Pi bootloader.

## Emulation

Run qemu-system-arm with machine raspi2 and UART1 mapped to stdio:

    qemu-system-arm -M raspi2 -serial null -serial stdio -bios build/kernel7.img

To debug the system, add -s -S to the qemu command and connect gdb-multiarch to the gdbserver

    # gdb-multiarch build/kernel7.img.elf
    (gdb) set arch arm
    (gdb) target remote :1234

Or set up VS Code to connect using

    "miDebuggerPath": "/usr/bin/gdb-multiarch",
    "miDebuggerServerAddress": ":1234"

## Licence

The code is provided as-is and you are free to use it for any purpose. I will provide a more formal licence soon, probably BSD.

## References

### Tutorials

- http://www.valvers.com/open-software/raspberry-pi/step01-bare-metal-programming-in-cpt1
- http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os
- https://azeria-labs.com/writing-arm-assembly-part-1/

### Documentation

- http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
- https://elinux.org/BCM2835_datasheet_errata
- https://github.com/raspberrypi/firmware/wiki/Mailboxes
- https://s-matyukevich.github.io/raspberry-pi-os/

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
- https://bitbucket.org/csowter/redkernel/src/master/
- https://github.com/bztsrc/raspi3-tutorial
