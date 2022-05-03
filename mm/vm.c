#include <vm.h>

// ---------------------------------------------------
//  virtual memory
//  虚拟内存管理 
// ---------------------------------------------------

// 设置内核页表基址，理论上是这三行汇编：
//      将页表基址填入x8寄存器（或任何一个通用寄存器）
//      msr ttbr1_el1, x8
//      isb
// 但暂时没研究明白内联汇编怎么写...
void set_ttbr1_el1(Pte pgdir) {

}

// 某种例子
// int add(int i, int j)
// {
//   int res = 0;
//   __asm ("ADD %[result], %[input_i], %[input_j]"
//     : [result] "=r" (res)
//     : [input_i] "r" (i), [input_j] "r" (j)
//   );
//   return res;
// }

// 建立范围映射
int vm_map(Pgdir pgdir0, uint64 va, uint64 pa, u_int perm) {
    return 0;
}

int page_insert() {
    return 0;
}

int vm_walk(Pgdir pgdir0, uint64 va, int create, Pte **pppte) {
    return 0;
}



