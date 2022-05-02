#include "mmu.h"
#include "pm.h"
#include "printf.h"
#include "types.h"


// ---------------------------------------------------
//  physical memory 
//  物理内存管理 
// ---------------------------------------------------


/* These variables are set by mips_detect_memory() */
uint64 maxpa;            /* Maximum physical address */
uint64 npage;            /* Amount of memory(in pages) */
uint64 basemem;          /* Amount of base memory(in bytes) */
uint64 extmem;           /* Amount of extended memory(in bytes) */

extern volatile unsigned char _data;
extern volatile unsigned char _end;

struct Page *pages;
static uint64 freemem;
static struct Page_list page_free_list;


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
    printf("_end = 0x%08x\n", (uint64)&_end);
}

static void *alloc(u_int n, u_int align, int clear)
{
	return NULL;
	// u_long alloced_mem;
	// /* Initialize `freemem` if this is the first time. The first virtual address that the
	//  * linker did *not* assign to any kernel code or global variables. */
	// if (freemem == 0) {
	// 	freemem = (u_long)_end;
	// }
	// /* Step 1: Round up `freemem` up to be aligned properly */
	// freemem = ROUND(freemem, align);
	// /* Step 2: Save current value of `freemem` as allocated chunk. */
	// alloced_mem = freemem;
	// /* Step 3: Increase `freemem` to record allocation. */
	// freemem = freemem + n;
	// /* Check if we're out of memory. If we are, PANIC !! */
	// if (PADDR(freemem) >= maxpa) {
	// 	panic("out of memory\n");
	// 	return (void *)-E_NO_MEM;
	// }
	// /* Step 4: Clear allocated chunk if parameter `clear` is set. */
	// if (clear) {
	// 	bzero((void *)alloced_mem, n);
	// }
	// /* Step 5: return allocated chunk. */
	// return (void *)alloced_mem;
}

// 物理内存初始化
void aarch64_pm_init() {
	return;
}

// 空闲页面全塞进链表
void page_init() {
	return;
}

// 释放物理页面
void page_free(struct Page *p) {
	return;
}

// 分配一个物理页面
int page_alloc(struct Page **pp) {
	return 0;
}

