#ifndef _MMU_H_
#define _MMU_H_

#include "types.h"

#define PGSIZE		4096
#define PGSHIFT		12

#define PTE_VALID 1
#define PTE_TABLE (1 << 1)

// AP (Access Permission) 标志位 [7:6]
#define PTE_KERN (0 << 6) // 仅 EL1 + 访问
#define PTE_USER (1 << 6) // EL0 + 可访问
#define PTE_RW (0 << 7) // 读写
#define PTE_RO (1 << 7) // 只读

// SH 共享标记 [9:8]
#define PTE_OUTER_SHARE (2 << 8) // 外部共享（核心 Cluster 间）需要给设备内存标记
#define PTE_INNER_SHARE (3 << 8) // 内部共享（Cluster 内）需要给普通内存标记

// AF (Access Flag) 标志位 [10]
#define PTE_AF (1 << 10) // 用户控制的可访问标识（AccessFlag）

// PXN/UXN 标志位 [54:53]
#define PTE_PXN (1UL << 53) // 不可执行标记 (PrivilegedExecute-Never)
#define PTE_UXN (1UL << 54) // EL0 不可执行标记 (Unprivileged Execute-Never)

#endif // !_MMU_H_

typedef uint64 Pte;
typedef uint64* Pgdir;

#define E_UNSPECIFIED	1	// Unspecified or unknown problem
#define E_BAD_ENV       2       // Environment doesn't exist or otherwise
				// cannot be used in requested action
#define E_INVAL		3	// Invalid parameter
#define E_NO_MEM	4	// Request failed due to memory shortage
#define E_NO_FREE_ENV   5       // Attempt to create a new environment beyond
				// the maximum allowed

// // 间接内存属性寄存器
// const uint64 MAIR_EL1 = 0x440488;

// // 翻译控制寄存器 
// const uint64 TCR_EL1 = 0x603A193A19;

// // 系统控制寄存器
// const uint64 SCR_EL1 = 0x30D01825;

/*
                                 计划中的内存布局

 o       虚存顶端 -----> +----------------------------+------------0xFFFF FFFF FFFF FFFF
 o                      |       外设，和一些乱七八糟的  |  
 o       KERNTOP -----> +----------------------------+------------0xFFFF 0000 0400 0000  ==  Physics Memory Max		
 o                      |       Kernel Stack         |                 		                                     
 o                      +----------------------------+----------               64MB，线性映射到物理内存
 o                      |       Kernel Text          |                        			
 o      KERNBASE -----> +----------------------------+------------0xFFFF 0000 0000 0000 
 o                      |                            | 
 o                      |                            | 
 o                      |                            | 
 o                      |                            | 
 o                      |                            |               体系结构不让用
 o                      |       disabled             |  
 o                      |                            | 
 o                      |                            | 
 o                      |                            | 
 o                      |                            | 
 o      USERTOP         +----------------------------+------------0x0000 0000 FFFF FFFF 	
 o                      |       用途未知              |  
 o      USERBASE        +----------------------------+------------0x0000 0000 0000 0000	
*/

