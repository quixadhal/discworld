#include <stdio.h>
#include "config.h"
#include "lint.h"
#include "interpret.h"
#include "object.h"
#include "md.h"

/*
   note: do not use MALLOC() etc. in this module.  Unbridled recursion
   will occur.  (use malloc() etc. instead)

   This module introduces quite a lot of overhead but it can be useful
   for tracking down memory leaks or for catching the freeing on non-malloc'd
   data.  This module could easily be extended to allow the malloced memory
   chunks to be tagged with a string label.
*/

#ifdef DEBUGMALLOC

int malloc_mask = 0;

static node_t **table;
unsigned long total_malloced = 0L;
unsigned long hiwater = 0L;

void
MDinit()
{
	table = (node_t **)calloc(TABLESIZE, sizeof(node_t *));
}

void
MDmalloc(node, size, tag, desc)
node_t *node;
int size;
unsigned short tag;
char *desc;
{
	unsigned long int h;

	total_malloced += size;
	if (total_malloced > hiwater) {
		hiwater = total_malloced;
	}
	h = (unsigned long)node % TABLESIZE;
	node->size = size;
	node->next = table[h];
#ifdef DEBUGMALLOC_EXTENSIONS
	node->tag = tag ? tag : 1;
	node->desc = desc ? desc : "default";
	if (malloc_mask & node->tag) {
		fprintf(stderr,"MDmalloc: %5d, [%-25s], %8x:(%d)\n",
			node->tag, node->desc, (unsigned int)PTR(node), node->size);
		fflush(stderr);
	}
#endif
	table[h] = node;
}

int
MDfree(ptr)
void *ptr;
{
	unsigned long int h;
	node_t *entry, **oentry;

	h = (unsigned long)ptr % TABLESIZE;
	oentry = &table[h];
	for (entry = *oentry; entry; oentry = &entry->next, entry = *oentry) {
		if (entry == ptr) {
			*oentry = entry->next;
			total_malloced -= entry->size;
			break;
		}
	}
	if (entry) {
#ifdef DEBUGMALLOC_EXTENSIONS
		if (malloc_mask & entry->tag) {
			fprintf(stderr,"MDfree: %5d, [%-25s], %8x:(%d)\n",
				entry->tag, entry->desc, (unsigned int)PTR(entry), entry->size);
			fflush(stderr);
		}
#endif
	} else {
		fprintf(stderr,
	"md: debugmalloc: attempted to free non-malloc'd pointer %x\n",
		(unsigned int)ptr);
#ifdef DEBUG
		abort();
#endif
		return 0;
	}
	return 1;
}

#ifdef DEBUGMALLOC_EXTENSIONS
void dump_debugmalloc(tfn, mask)
char *tfn;
int mask;
{
	int j;
	char *fn;
	node_t *entry;
	FILE *fp;
	int count = 0;

	fn = check_valid_path(tfn, current_object, "debugmalloc", 1);
	if (!fn) {
		add_message("Invalid path '%s' for writing.\n", tfn);
		return;
	}
	fp = fopen(fn, "w");
	if (!fp) {
		add_message("Unable to open %s for writing.\n", fn);
		return;
	}
	add_message("Dumping to %s ...",fn);
	for (j = 0; j < TABLESIZE; j++) {
		for (entry = table[j]; entry; entry = entry->next) {
			if (entry->tag & mask) {
				fprintf(fp,"%6d: tag = %5d, [%-20s], addr = %8x, size = %7d\n",
					count++, entry->tag, entry->desc,
					(unsigned int)PTR(entry), entry->size);
			}
		}
	}
	fclose(fp);
	add_message(" done.\n");
}
#endif /* DEBUGMALLOC_EXTENSIONS */

void set_malloc_mask(mask)
int mask;
{
	malloc_mask = mask;
}

#endif /* DEBUGMALLOC */
