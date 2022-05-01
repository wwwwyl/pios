INCLUDES := -I./ -I../ -I../include/
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: mbox.o uart.o print.o printf.o exc.o

clean:
	rm -rf *~ *.o


include ../include.mk
