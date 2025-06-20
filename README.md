# Lua-OS
From Lua, for Lua -- An OS written by Lua and run Lua program.

Lua-OS is an OS mostly written by Lua, actually just not more than 400 lines C + Lua code.

Although it is tiny, but has most functionalities an OS has, file system, program execution, io and a shell. You can easily write your own Lua code, build it into the static file system and run it.

It can run on the bare metal aarch64 machine, in this project, for the convenience, it use QEMU to emulate an aarch64 board. But you definetly can burn the image into a real bare metal board to run it.

# How to use?

* 1 install qemu-system-arm
* 2 install arm toolchain "AArch64 bare-metal target (aarch64-none-elf)" verion
* 3 checkout the code and change the Makefile to provie your toolchain path, then run "make run"

# Write your own Lua code and run
* 1 write your Lua code
* 2 put it into rootfs folder in this project
* 3 make run, you will see your file there, "ls" command to list it, "cat <file name>" to show the content, <file name> to execute it.
# Screenshot
![image](https://github.com/user-attachments/assets/6201c8b6-d035-4fa7-818a-5b7bb91f3f65)

