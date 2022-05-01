/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <print.h>
#include <uart.h>


static void myoutput(void *arg, char *s, int l)
{
    int i;

    // special termination call
    if ((l==1) && (s[0] == '\0')) return;
    
    for (i=0; i< l; i++) {
    if (s[i] == '\n') uart_send('\r');
	uart_send(s[i]);
    }
}

void printf(char *fmt, ...)
{
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    lp_Print(myoutput, 0, fmt, ap);
    __builtin_va_end(ap);
}

void
_panic(const char *file, int line, const char *fmt,...)
{
	__builtin_va_list ap;


	__builtin_va_start(ap, fmt);
	printf("\npanic at %s:%d: ", file, line);
	lp_Print(myoutput, 0, (char *)fmt, ap);
	printf("\n");
	__builtin_va_end(ap);


	for(;;);
}

void show_el(){
    unsigned long el;
    asm volatile ("mrs %0, CurrentEL" : "=r" (el));

    printf("Current EL is: %x\n", (el>>2)&3);
}
