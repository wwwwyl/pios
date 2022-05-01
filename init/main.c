#include "uart.h"
#include "printf.h"
#include "pmap.h"

void main(){
    uart_init();

	printf("main.c:\tmain is start ...\n");

	aarch64_init();
	panic("main is over is error!");

	return 0;
}