#ifndef _PMAP_H_
#define _PMAP_H_


#include "types.h"
#include "queue.h"
#include "mmu.h"
#include "printf.h"

LIST_HEAD(Page_list, Page);
typedef LIST_ENTRY(Page) Page_LIST_entry_t;

struct Page {
	Page_LIST_entry_t pp_link;	/* free list link */
	u_short pp_ref;
};

#define PPN(va)		(((u_long)(va))>>12)

extern struct Page *pages;
extern uint64 npage;

static inline uint64
page2ppn(struct Page *pp)
{
	return pp - pages;
}

static inline uint64
page2pa(struct Page *pp)
{
	return page2ppn(pp)<<12;
}

static inline struct Page *
pa2page(u_long pa)
{
	if (PPN(pa) >= npage)
		panic("pa2page called with invalid pa: %x", pa);
	return &pages[PPN(pa)];
}

// -------------------mmu属性设置----------------------- //
// 间接内存属性寄存器
#define MAIR_EL1 0x440488
// 翻译控制寄存器 
#define TCR_EL1 0x603A193A19
// 系统控制寄存器
#define SCTLR_EL1 0x30D01825

// --------------------物理内存相关函数---------------------//
// void aarch64_init();
void aarch64_detect_memory();
void pm_init();
// void page_init();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);
int asm_test(int i,int j);

// --------------------虚拟内存相关函数---------------------//
// void uvminit();   // 进程页表初始化
void vm_init();
void kvm_init();   // 内核页表初始化
void set_ttbr0_el1(Pgdir pgdir);  // 切换页表
void set_ttbr1_el1(Pgdir pgdir);
void vm_print(Pte *pgdir);   // 打印页表，调试用
int pgdir_walk(Pgdir pgdir, uint64 va, int create, Pte **pppte);
void page_remove(Pgdir pgdir, uint64 va);
struct Page *page_lookup(Pgdir pgdir, uint64 va, Pte **ppte);
int pages_map(Pgdir pgdir, uint64 va, uint64 pa, uint64 length,u_int perm);
#endif /* _PMAP_H_ */