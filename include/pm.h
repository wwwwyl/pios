#ifndef _PMAP_H_
#define _PMAP_H_

#include "queue.h"
#include "types.h"
#include "printf.h"

LIST_HEAD(Page_list, Page);
typedef LIST_ENTRY(Page) Page_LIST_entry_t;

struct Page {
	Page_LIST_entry_t pp_link;	/* free list link */
	u_short pp_ref;
};


// void aarch64_init();
void aarch64_detect_memory();
void pm_init();
// void page_init();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);

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

#endif /* _PMAP_H_ */