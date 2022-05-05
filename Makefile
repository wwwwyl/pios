# Main makefile
#
# Copyright (C) 2007 Beihang University
# Written by Zhu Like ( zlike@cse.buaa.edu.cn )
#

# drivers_dir	  := drivers
boot_dir	  := boot
init_dir	  := init
lib_dir		  := lib
mm_dir		  := mm
tools_dir	  := tools

link_script   := $(tools_dir)/link.ld

modules		  := boot init lib mm
objects		  := $(boot_dir)/start.o			  \
				 $(init_dir)/*.o			  \
				 $(lib_dir)/*.o               \
				 $(mm_dir)/*.o

.PHONY: all $(modules) clean

all: $(modules) kernel run

kernel: $(modules)
	mkdir -p qemu
	$(LD) -N $(objects) -T $(link_script) -o qemu/kernel.elf
	$(OBJCOPY) -O binary qemu/kernel.elf qemu/kernel.img
	$(OBJDUMP) -S qemu/kernel.elf > qemu/kernel.asm
	$(OBJDUMP) -t qemu/kernel.elf                     > qemu/kernel.sym
# | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' 

$(modules):
	$(MAKE) --directory=$@

clean:
	for d in $(modules);	\
		do					\
			$(MAKE) --directory=$$d clean; \
		done; \
	rm -rf *.o *~ $(vmlinux_elf)
	rm qemu/kernel.asm
	rm qemu/kernel.sym

run:
	qemu-system-aarch64 -M raspi3 -kernel qemu/kernel.img -serial stdio

gdb: $(modules) kernel run_qemu_for_gdb

run_qemu_for_gdb:
	qemu-system-aarch64 -M raspi3 -kernel qemu/kernel.elf -nographic -s -S


include include.mk
