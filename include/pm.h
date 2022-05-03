#ifndef _PMAP_H_
#define _PMAP_H_

#include "queue.h"
#include "types.h"

LIST_HEAD(Page_list, Page);
typedef LIST_ENTRY(Page) Page_LIST_entry_t;

struct Page {
	Page_LIST_entry_t pp_link;	/* free list link */
	u_short pp_ref;
};


void aarch64_init();
void aarch64_detect_memory();
void pm_init();
void page_init();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);

extern struct Page *pages;

static inline u_long
page2ppn(struct Page *pp)
{
	return pp - pages;
}

static inline u_long
page2pa(struct Page *pp)
{
	return page2ppn(pp)<<12;
}

#endif /* _PMAP_H_ */