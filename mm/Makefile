INCLUDES	:= -I./ -I../ -I../include/

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: pm.o vm.o

%.s: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -S $<


clean:
	rm -rf *~ *.o *.s


include ../include.mk
