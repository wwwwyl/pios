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
void aarch64_pm_init();
void page_init();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);

#endif /* _PMAP_H_ */