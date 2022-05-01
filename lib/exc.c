/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "printf.h"

/**
 * common exception handler
 */
void exc_handler(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far)
{
    printf("Exception ");
    // print out interruption type
    switch(type) {
        case 0: printf("Synchronous: "); break;
        case 1: printf("IRQ: "); break;
        case 2: printf("FIQ: "); break;
        case 3: printf("SError: "); break;
    }
    // decode exception type (some, not all. See ARM DDI0487B_b chapter D10.2.28)
    switch(esr>>26) {
        case 0b000000: printf("Unknown"); break;
        case 0b000001: printf("Trapped WFI/WFE"); break;
        case 0b001110: printf("Illegal execution"); break;
        case 0b010101: printf("System call"); break;
        case 0b100000: printf("Instruction abort, lower EL"); break;
        case 0b100001: printf("Instruction abort, same EL"); break;
        case 0b100010: printf("Instruction alignment fault"); break;
        case 0b100100: printf("Data abort, lower EL"); break;
        case 0b100101: printf("Data abort, same EL"); break;
        case 0b100110: printf("Stack alignment fault"); break;
        case 0b101100: printf("Floating point"); break;
        default: printf("Others Unknown"); break;
    }
    // decode data abort cause
    if(esr>>26==0b100100 || esr>>26==0b100101) {
        printf(", ");
        switch((esr>>2)&0x3) {
            case 0: printf("Address size fault"); break;
            case 1: printf("Translation fault"); break;
            case 2: printf("Access flag fault"); break;
            case 3: printf("Permission fault"); break;
        }
        switch(esr&0x3) {
            case 0: printf(" at level 0"); break;
            case 1: printf(" at level 1"); break;
            case 2: printf(" at level 2"); break;
            case 3: printf(" at level 3"); break;
        }
    }
    // dump registers
    printf(":\n  ESR_EL1 %016x  ELR_EL1 %016x\n SPSR_EL1 %016x  FAR_EL1 %016x\n", esr, elr, spsr, far);
    while(1);
}
