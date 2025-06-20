# Lua-OS
From Lua, for Lua -- An OS written by Lua and run Lua program.

Lua-OS is an OS mostly written by Lua, actually just not more than 400 lines C + Lua code.

Although it is tiny, but has most functionalities an OS has, file system, program execution, io and a shell. You can easily write your own Lua code, build it into the static file system and run it.

It can run on the bare metal aarch64 machine, in this project, for the convenience, it use QEMU to emulate an aarch64 board. But you definetly can burn the image into a real bare metal board to run it.

# How to use?

* 1 install qemu-system-arm
* 2 install arm toolchain "AArch64 bare-metal target (aarch64-none-elf)" verion
* 3 checkout the code and change the Makefile to provie your toolchain path, then run "make run"
  
# Screenshot

![image](https://github.com/user-attachments/assets/c93cf99d-e7fe-4d26-8429-c78ca4be583f)

