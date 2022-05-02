# Common includes in Makefile
#
# Copyright (C) 2007 Beihang University
# Written By Zhu Like ( zlike@cse.buaa.edu.cn )



# Exercise 1.1. Please modify the CROSS_COMPILE path.

CROSS_COMPILE :=  ~/buaaOS/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
CC            := $(CROSS_COMPILE)gcc
CFLAGS        := -Wall -O2 -ffreestanding -fno-stack-protector -nostdinc -nostdlib -nostartfiles
LD            := $(CROSS_COMPILE)ld
OBJCOPY       := $(CROSS_COMPILE)objcopy
