#include "vm.h"
#include "pm.h"

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
int vm_map(Pgdir pgdir, uint64 va, uint64 pa, u_int perm) {
    return 0;
}

void tlb_invalidate(Pgdir pgdir, uint64 va) {

}

int page_insert(Pgdir pgdir, struct Page *pp, uint64 va, u_int perm)
{
	u_int PERM;
	Pte *pte;
	PERM = perm | PTE_VALID;

	/* Step 1: Get corresponding page table entry. */
	pgdir_walk(pgdir, va, 0, &pte);

	if (pte != 0 && (*pte & PTE_VALID) != 0) { // 如果va已经映射到某个页面
		if (pa2page(*pte) != pp) {  // 如果va没有映射到pp
			page_remove(pgdir, va);           // 就取消这个映射
		} else	{    // 如果va已经映射到pp
			tlb_invalidate(pgdir, va);    
			*pte = (page2pa(pp) | PERM);  // perm可能会变更
			return 0;
		}
	}

	/* Step 2: Update TLB. */
	tlb_invalidate(pgdir,va);  // 

	/* Step 3: Do check, re-get page table entry to validate the insertion. */
	int ret;
	if ((ret=pgdir_walk(pgdir, va, 1, &pte)) < 0) {
		return ret;
	}
	/* Step 3.2 Insert page and increment the pp_ref */
	*pte = (page2pa(pp) | PERM);
	pp->pp_ref++;
	//myPrintf("----- page_insert end\n");
	return 0;
}

int pgdir_walk(Pgdir pgdir, uint64 va, int create, Pte **pppte) {
    int ret;
    for (int level=2; level>0; level--) {
        Pte *pte = &pgdir[PX(level, va)];
        if (*pte & PTE_VALID) 
            pgdir = (Pgdir)PTE2PA(*pte);
        else {
            struct Page *page;
            if (create) {
                if ((ret=page_alloc(&page))<0)
                    return ret;
                uint64 pa = page2pa(page);
                pgdir = (Pgdir)pa;
                *pte = PA2PTE(pa) | PTE_VALID | PTE_TABLE;
            }
            else {
                *pppte = 0;
                return 0;
            }   
        }
    }
    *pppte = &pgdir[PX(0,va)];
    return 0;
}

void page_remove(Pgdir pgdir, uint64 va)
{
	Pte *pte;
	struct Page *ppage;

	/* Step 1: Get the page table entry, and check if the page table entry is valid. */

	ppage = page_lookup(pgdir, va, &pte);

	if (ppage == 0) {
		return;
	}

	/* Step 2: Decrease `pp_ref` and decide if it's necessary to free this page. */

	/* Hint: When there's no virtual address mapped to this page, release it. */
	ppage->pp_ref--;
	if (ppage->pp_ref == 0) {
		page_free(ppage);
	}

	/* Step 3: Update TLB. */
	*pte = 0;
	tlb_invalidate(pgdir, va);
	return;
}

struct Page *page_lookup(Pgdir pgdir, uint64 va, Pte **ppte)
{
	struct Page *page;
	Pte *pte;

	/* Step 1: Get the page table entry. */
	pgdir_walk(pgdir, va, 0, &pte);

	/* Hint: Check if the page table entry doesn't exist or is not valid. */
	if (pte == 0 || (*pte & PTE_VALID) == 0) {
		return 0;
	}

	/* Step 2: Get the corresponding Page struct. */

	/* Hint: Use function `pa2page`, defined in include/pmap.h . */
	page = pa2page(PTE2PA(*pte));
	if (ppte) {
		*ppte = pte;
	}

	return page;
}

void vm_print_recursive(Pgdir pgdir, int level) {
    for (int i=0; i<512; i++) {
        if (pgdir[i] | PTE_VALID) {
            if (level==1) printf(".. ");
            if (level==0) printf(".. ");
            printf("0x%x\n",pgdir[i]);
            if (pgdir[i] & PTE_TABLE) {
                vm_print_recursive((Pgdir)pgdir[i],level-1);
            }
        }
    }
}

void vm_print(Pgdir pgdir) {
    printf("vm_print 0x%x\n...", pgdir);
    vm_print_recursive(pgdir, 2);
}



