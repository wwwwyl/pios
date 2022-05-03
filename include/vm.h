#include "types.h"
#include "mmu.h"

/*
计划中为4级页表
虚拟地址划分：

 	 63         48  47    39 38   30  29    21 20    12 11        0
	|-------------||-------||-------||-------||-------||----------|
长度：     16           9        9        9        9         12
功能：   全0或全1      idx0      idx1     idx2     idx3     offset

一些事实：
	每个页4KB
	每个页表2^9 = 512个pte
	每个pte占64位
	512*64 = 4KB
	也就是说每个页表正好占1页

*/



void kvminit();   // 内核页表初始化
void uvminit();   // 进程页表初始化
void pgdir_switch(Pte *pgdir);  // 切换页表，大概是某种汇编语句
void vmprint(Pte *pgdir);   // 打印页表，调试用