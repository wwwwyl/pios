#include "mmu.h"
#include "pmap.h"
// #include "pm.h"
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

void bzero(void *m, int n) {
	char *c = (char*)m;
	for (int i=0; i<n; i++)
		*(c+i) = 0;
}


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
	npage = basemem >> PGSHIFT;

    printf("Physical memory: %dK available, ", (int)(maxpa / 1024));
    printf("base = %dK, extended = %dK\n", (int)(basemem / 1024),
           (int)(extmem / 1024));
    printf("_end = 0x%08x\n", (uint64)&_end);
}

// 物理内存链表建好之前，使用此函数分配内存
static void *alloc(u_int n, u_int align)
{
	// return NULL;
	u_long alloced_mem;
	/* Initialize `freemem` if this is the first time. The first virtual address that the
	 * linker did *not* assign to any kernel code or global variables. */
	if (freemem == 0) {
		freemem = (uint64)&_end;
	}
	printf("freemem = 0x%x\n", freemem);
	/* Step 1: Round up `freemem` up to be aligned properly */
	freemem = ROUND(freemem, align);
	/* Step 2: Save current value of `freemem` as allocated chunk. */
	alloced_mem = freemem;
	/* Step 3: Increase `freemem` to record allocation. */
	freemem = freemem + n;
	/* Check if we're out of memory. If we are, PANIC !! */
	if (freemem >= maxpa) {  
		panic("out of memory\n");
		return (void *)-E_NO_MEM;
	}
	/* Step 4: Clear allocated chunk */
	bzero((void *)alloced_mem, n);
	/* Step 5: return allocated chunk. */
	return (void *)alloced_mem;
}



void pm_init() {
	printf("physical memory init begin...\n");
	// 给物存管理数组pages分配空间
	pages = (struct Page *)alloc(npage*sizeof(struct Page), PGSIZE);
	if (pages==(void*)-E_NO_MEM)
		panic("aarch64_pm_init: fail\n");
	// 空闲页面全塞进链表
	LIST_INIT(&page_free_list);
	freemem = ROUND(freemem, PGSIZE);
	int i;
	for (i=0; page2pa(&pages[i])<freemem; i++)
		pages[i].pp_ref = 1;
	printf("0x%x mem has been used.\n");
	int cnt = 0;
	for (; i<npage; i++) {
		pages[i].pp_ref = 0;
		LIST_INSERT_HEAD(&page_free_list, &pages[i], pp_link);
		cnt++;
	}
	printf("%d pages remain, insert them to page_free_list.\n",cnt);
	printf("physical memory init success.\n");
	return;
}

// 释放物理页面
void page_free(struct Page *p) {
	/* Step 1: If there's still virtual address referring to this page, do nothing. */
	if (p->pp_ref>0)
		return;
	/* Step 2: If the `pp_ref` reaches 0, mark this page as free and return. */
	if (p->pp_ref==0) {
		LIST_INSERT_HEAD(&page_free_list, p, pp_link);
		return;
	}
	/* If the value of `pp_ref` is less than 0, some error must occurr before,
	 * so PANIC !!! */
	panic("cgh:pp->pp_ref is less than zero\n");
	return;
}

// 分配一个物理页面
int page_alloc(struct Page **pp) {
	struct Page *ppage_temp;
	/* Step 1: Get a page from free memory. If fail, return the error code.*/
	if (LIST_EMPTY(&page_free_list))
		return -E_NO_MEM;
	ppage_temp = LIST_FIRST(&page_free_list);
	LIST_REMOVE(ppage_temp, pp_link);
	/* Step 2: Initialize this page. */
	bzero((void*)page2pa(ppage_temp), PGSIZE);

	*pp = ppage_temp;
	return 0;
}

int asm_test(int i, int j)
{
	int res = 0;
	__asm ("ADD %[result], %[input_i], %[input_j]"
		: [result] "=r" (res)
		: [input_i] "r" (i), [input_j] "r" (j)
	);
	__asm ("ADD %[result], %[input_i], %[input_j]"
		: [result] "=r" (res)
		: [input_i] "r" (i), [input_j] "r" (res)
	);
  return res;
}

