#include "mmu.h"
#include "pmap.h"
#include "printf.h"
#include "types.h"

/* These variables are set by mips_detect_memory() */
u_int64_t maxpa;            /* Maximum physical address */
u_int64_t npage;            /* Amount of memory(in pages) */
u_int64_t basemem;          /* Amount of base memory(in bytes) */
u_int64_t extmem;           /* Amount of extended memory(in bytes) */

extern volatile unsigned char _data;
extern volatile unsigned char _end;

/* Overview:
 	Initialize basemem and npage.
 	Set basemem to be 64MB, and calculate corresponding npage value.*/
void aarch64_detect_memory()
{
    // TODO: unfinlished
    /* Step 1: Initialize basemem.
     * (When use real computer, CMOS tells us how many kilobytes there are). */
	maxpa = 0x4000000;
	basemem = 0x4000000;
	extmem = 0;

    // Step 2: Calculate corresponding npage value.
	// npage = basemem >> PGSHIFT;

    printf("Physical memory: %dK available, ", (int)(maxpa / 1024));
    printf("base = %dK, extended = %dK\n", (int)(basemem / 1024),
           (int)(extmem / 1024));
    printf("_end = 0x%08x\n", (u_int64_t)&_end);
}