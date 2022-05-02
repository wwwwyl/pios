#include "uart.h"
#include "printf.h"
#include "pm.h"

int main(){
    uart_init();

	printf("main.c:\tmain is start ...\n");
    show_el();
    

	aarch64_init();

	panic("main is over is error!");

	return 0;
}