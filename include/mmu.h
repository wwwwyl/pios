#ifndef _MMU_H_
#define _MMU_H_

#define BY2PG		4096

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