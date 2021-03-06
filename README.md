# PIOS---BUAA-OS-lab-2022挑战性任务

pios是一个将buaa-os-lab-2022实验的MOS系统移植到raspi3-Aarch64环境下的挑战性任务。

其主要任务流程为：

* Lab0: 实验环境配置
* Lab1: 内核启动和 Printf 的实现
* Lab2: MMU 设置和内存管理
* Lab3: 异常处理和进程管理
* Lab4: 系统调用 / IPC 和 Fork
* Lab5: 文件系统
* Lab6: Shell

任务进度： 

* 2022-5-1 Lab1 printf实现



## 关于环境

#### 交叉编译器

* Arm GNU Toolchain
  * 实验环境选择x86_64 Linux hosted cross toolchains for AArch64 bare-metal target 2022.02
  * [Download](https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf.tar.xz)
  * 在include.mk中配置交叉编译器环境
  
    ex.
  
    ```makefile
    CROSS_COMPILE :=  /gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
    CC            := $(CROSS_COMPILE)gcc
    CFLAGS        := -Wall -O2 -ffreestanding -fno-stack-protector -nostdinc -nostdlib -nostartfiles
    LD            := $(CROSS_COMPILE)ld
    OBJCOPY       := $(CROSS_COMPILE)objcopy
    # CFLAGS没有引入标准库，可能需要改
    ```

    替换或添加一些内容以生成反汇编文件qemu/kernel.asm
    ```makefile
    # 抄自xv6
    CFLAGS        := -Wall -Werror -O -ffreestanding 
    CFLAGS        += -fno-omit-frame-pointer 
    CFLAGS        += -fno-stack-protector -nostdinc -nostdlib -nostartfiles
    CFLAGS        += -ggdb
    OBJDUMP       := $(CROSS_COMPILE)objdump
    ```

#### 硬件模拟器

* QEMU 5.0.0
  * 编译安装见指导书
  * 通常需要安装libglibc，libpixman之类，apt安装对应版本dev包即可，不需要自行编译

#### gdb

除参考书中内容外，创建~/.gdbinit：
```
target remote:1234
set architecture aarch64
add-symbol-file qemu/kernel.elf 0x80000
layout split
```
可进行c语言级、汇编级调试，qemu+gdb比gxemul强到不知道哪里去了

## 参考
* BUAA-OS-LAB-2022 MOS
* Github:[bztsrc/raspi3-tutorial](https://github.com/bztsrc/raspi3-tutorial)