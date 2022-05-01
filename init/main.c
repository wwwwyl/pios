#include "uart.h"
#include "mbox.h"

void main(){
    // set up serial console
    uart_init();
    
    uart_puts("Hello world!");

    // echo everything back
    while(1) {
        uart_send(uart_getc());
    }
}