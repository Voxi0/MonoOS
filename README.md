# MonoOS
MonoOS is a simple x86 64-Bit operating system written in C and a tiny bit of assembly (NASM). I am using Limine which boots directly into 64-Bits which reduces the amount of work I have to do compared to using GRUB which only goes upto 32-Bits and requires manual intervention in order to get to 64-Bits. SSFN is for loading fonts and rendering text while tiny-printf is a tiny printf implementation written in C meant for embedded systems. It's very easy to add and use in your project and the author has declared it as flawless now.

## Libraries Used
Limine (64-Bit Bootloader): https://github.com/limine-bootloader/limine
SSFN (Scalable Font 2 - Portable Single ANSI C/C++ Header File Scalable Bitmap and Vector Font Renderer): https://github.com/limine-bootloader/limine
Printf (Tiny, Fast, Non-Dependant and Fully Loaded Printf Implementation For Embedded Systems): https://github.com/limine-bootloader/limine
