INCLUDES := -I../include

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: main.o init.o

clean:
	rm -rf *~ *.o


include ../include.mk
