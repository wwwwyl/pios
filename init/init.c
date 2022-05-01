#include "printf.h"
#include "pmap.h"

void aarch64_init(){
    printf("init.c:\taarch64_init() is called\n");




    panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of aarch64_init() reached!");
}