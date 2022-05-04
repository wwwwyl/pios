#include "types.h"
#include "mmu.h"

/*
3级页表
虚拟地址划分：

 	 63         48  47    39 38   30  29    21 20    12 11        0
	|-------------||-------||-------||-------||-------||----------|
长度：     16           9        9        9        9         12
功能：   全0或全1      不使用    idx1     idx2     idx3     offset

每个页4KB
每个页表2^9 = 512个pte
每个pte占64位
512*64 = 4KB
也就是说每个页表正好占1页

*/

// macro copy from xv6
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))
#define PXMASK          0x1FF // 9 bits
#define PXSHIFT(level)  (PGSHIFT+(9*(level)))
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

// write by lyh
// 
#define PTE2PA(pte) ((((pte)>>12) & 0xFFFFFFFFF) << PGSHIFT)
#define PA2PTE(pa)  ((((pa)>>PGSHIFT) & 0xFFFFFFFFF) << 12)


void kvminit();   // 内核页表初始化
void uvminit();   // 进程页表初始化
void pgdir_switch(Pte *pgdir);  // 切换页表，大概是某种汇编语句
void vmprint(Pte *pgdir);   // 打印页表，调试用
int pgdir_walk(Pgdir pgdir, uint64 va, int create, Pte **pppte);
void page_remove(Pgdir pgdir, uint64 va);
struct Page *page_lookup(Pgdir pgdir, uint64 va, Pte **ppte);