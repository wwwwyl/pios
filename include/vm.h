#include "types.h"
#include "mmu.h"

void kvminit();   // 内核页表初始化
void uvminit();   // 进程页表初始化
void pgdir_switch(Pte *pgdir);  // 切换页表，大概是某种汇编语句
void vmprint(Pte *pgdir);   // 打印页表，调试用