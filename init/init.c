#include "printf.h"
#include "pmap.h"

void exc_test(){
    printf("init.c:\texc_test() is called\n");
    printf("there shoule be an Exception below\n");
    unsigned int r;
    // generate a Data Abort with a bad address access
    r=*((volatile unsigned int*)0xFFFFFFFFFF000000);
    // lol, shut up gcc
    r++;
    panic("init.c:\tend of exc_test() reached!");
}

void aarch64_init(){
    printf("init.c:\taarch64_init() is called\n");
    aarch64_detect_memory();
    pm_init();
    // printf("%d\n",asm_test(1,2));
    vm_init();

    exc_test();


    panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of aarch64_init() reached!");
}