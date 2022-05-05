#include "mmu.h"
#include "pmap.h"
// #include "vm.h"

// ---------------------------------------------------
//  virtual memory
//  虚拟内存管理 
// ---------------------------------------------------

// 设置低地址页表寄存器
void set_ttbr0_el1(Pgdir pgdir) {
    __asm ("msr ttbr0_el1, %[input]"
		: 
		: [input] "r" (pgdir)
	);
    __asm ("isb");
}

// 设置高地址页表寄存器
void set_ttbr1_el1(Pgdir pgdir) {
    __asm ("msr ttbr1_el1, %[input]"
		: 
		: [input] "r" (pgdir)
	);
    __asm ("isb");
}

// 间接内存属性寄存器
void set_mair_el1() {
    __asm ("msr mair_el1, %[input]"
		: 
		: [input] "r" (MAIR_EL1)
	);
    __asm ("isb");
}

// 设置翻译控制寄存器
void set_tcr_el1() {
    __asm ("msr tcr_el1, %[input]"
		: 
		: [input] "r" (TCR_EL1)
	);
    __asm ("isb");
}

// 设置系统控制寄存器，此函数开启mmu
void set_sctlr_el1() {
    __asm ("msr sctlr_el1, %[input]"
		: 
		: [input] "r" (SCTLR_EL1)
	);
    __asm ("isb");
}

void vm_init() {
    Pgdir pgdir;
    struct Page *page;
    page_alloc(&page);
    pgdir = (Pgdir)page2pa(page);
	kvm_init(pgdir);
	// vm_print(pgdir);
    set_ttbr0_el1(pgdir);
    set_mair_el1();
    set_tcr_el1();
	printf("vm init almost success.\n");
    set_sctlr_el1();
	printf("vm init success.\n");
}

void kvm_init(Pgdir pgdir) {
	int ret;
	if ((ret=pages_map(pgdir,0,0,0x4000000,PTE_RW|PTE_USER))<0)
		return;
	printf("kvm_init success\n");
}


// 建立范围映射
int pages_map(Pgdir pgdir, uint64 va, uint64 pa, 
           uint64 length,u_int perm) {
	uint64 a = ROUNDDOWN(va,PGSIZE);
	uint64 last = ROUNDDOWN(va+length-1, PGSIZE);
	Pte *pte;
	int ret;
	for(;;) {
		if ((ret=pgdir_walk(pgdir,a,1,&pte))<0) {
			printf("pages_map:failed\n");
			return ret;
		}
		if (*pte & PTE_VALID)
			panic("remap");
		*pte = PA2PTE(pa) | perm | PTE_VALID;
		if (a==last)
			break;
		a += PGSIZE;
		pa += PGSIZE;
	}
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
            if (level<2) printf(".. ");
            if (level<1) printf(".. ");
            printf("%d: 0x%08x\n",i,pgdir[i]);
            if (pgdir[i] & PTE_TABLE) {
                vm_print_recursive((Pgdir)PTE2PA(pgdir[i]),level-1);
            }
        }
    }
}

void vm_print(Pgdir pgdir) {
    printf("vm_print 0x%08x...\n", pgdir);
    vm_print_recursive(pgdir, 2);
}



