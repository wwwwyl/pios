#ifndef _MMU_H_
#define _MMU_H_

#include "queue.h"
#include "types.h"

// -------------------页尺寸----------------------- //
#define PGSIZE		4096
#define PGSHIFT		12

// -------------------虚存类型----------------------- //
typedef uint64 Pte;
typedef uint64* Pgdir;

// -------------------PTE标志位----------------------- //

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



// -------------------辅助函数---------------------- //

// macro copy from xv6
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))
#define PXMASK          0x1FF // 9 bits
#define PXSHIFT(level)  (PGSHIFT+(9*(level)))
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

// write by lyh
// 
#define PTE2PA(pte) ((((pte)>>12) & 0xFFFFFFFFF) << PGSHIFT)
#define PA2PTE(pa)  ((((pa)>>PGSHIFT) & 0xFFFFFFFFF) << 12)

// -------------------错误代码---------------------- //

#define E_UNSPECIFIED	1	// Unspecified or unknown problem
#define E_BAD_ENV       2       // Environment doesn't exist or otherwise
				// cannot be used in requested action
#define E_INVAL		3	// Invalid parameter
#define E_NO_MEM	4	// Request failed due to memory shortage
#define E_NO_FREE_ENV   5       // Attempt to create a new environment beyond
				// the maximum allowed



/*
                                 计划中的内存布局

 o       虚存顶端 -----> +----------------------------+------------0xFFFF FFFF FFFF FFFF
 o                      |       外设，和一些乱七八糟的  |  
 o      USERTOP         +----------------------------+------------0xFFFF 0000 FFFF FFFF 	
 o                      |       用途未知              |  
 o      USERBASE        +----------------------------+------------0xFFFF 0000 0000 0000	
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
 o       KERNTOP -----> +----------------------------+------------0x0000 0000 0400 0000  ==  Physics Memory Max		
 o                      |       Kernel Stack         |                 		                                     
 o                      +----------------------------+----------               64MB，线性映射到物理内存
 o                      |       Kernel Text          |                        			
 o      KERNBASE -----> +----------------------------+------------0x0000 0000 0000 0000 
*/

