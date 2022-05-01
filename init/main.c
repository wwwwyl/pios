#include "uart.h"
#include "printf.h"

void main(){
    // set up serial console
    uart_init();

    printf("Hello world! printf\n");

    // echo everything back
    while(1) {
        char c;
        uart_send(c=uart_getc());
        if(c=='q')break;
    }
    panic("~~~~~~~~~~main end~~~~~~~~~~");
}