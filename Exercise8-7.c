#include <unistd.h> // sbrk
#include <stdio.h>

#define NALLOC 1024        /* minimum #units to request */

typedef long Align;        /* for alignment to long boundary */
union header {             /* block header: */
	struct {
		union header *ptr; /* next block if on free list */
		unsigned size;     /* size of this block */
	} s;
	Align x;               /* force alignment of blocks */
};
typedef union header Header;
static Header base;        /* empty list to get started */
static Header *freep = NULL;      /* start of free list */


struct currhp {  // we can only free a pointer that was returned by a call to malloc or calloc
	Header *ptr;
	struct currhp *nxt;
	struct currhp *prev;
} my_mmcp = {NULL, &my_mmcp, &my_mmcp}, *mmcp = &my_mmcp;


/* free: put block ap in free list */
void my__myfree(Header *bp)
{
	Header *p;

	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;         /* freed block at start or end of arena */
	
	if (bp + bp->s.size == p->s.ptr) { /* join to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	}
	else
		bp->s.ptr = p->s.ptr;
	if (p + p->s.size == bp) {  /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}
	else
		p->s.ptr = bp;
	freep = p;
}


/* morecore: ask system for more memory */
static Header *morecore(unsigned nu)
{
	void *vp;
	Header *up;

	if (nu < NALLOC)
		nu = NALLOC;
	vp = sbrk(nu * sizeof(Header));
	if (vp == (void *) -1) /* no space at all */
		return NULL;
	up = vp;
	up->s.size = nu;
	my__myfree(up);
	return freep;
}


/* mymalloc: general-purpose storage allocator */
void *my__mymalloc(unsigned nbytes)
{
	Header *p, *prevp;
	unsigned nunits;

	if (nbytes < 1)
		return NULL;
	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
	if ((prevp = freep) == NULL) { /* no free list yet */
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}
	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {  /* big enough */
			if (p->s.size == nunits)   /* exactly */
				prevp->s.ptr = p->s.ptr;
			else {           /* allocate tail end */
				p->s.size -=nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			
			return p + 1;
		}
		if (p == freep) /* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL; /* none left */
	}
}


void myfree(void *ap)
{
	if (ap == NULL)
		return;

	Header *bp = (Header *)ap - 1; // point to block header
	struct currhp *p;
	for (p = mmcp->nxt; bp != p->ptr; p = p->nxt)
		if (p == mmcp)
			return;
	p->prev->nxt = p->nxt;
	p->nxt->prev = p->prev;
	
	my__myfree(bp);
	my__myfree((Header *)p - 1);
}


void *mymalloc(unsigned nbytes)
{
	Header *p = my__mymalloc(nbytes);
	struct currhp *hp = my__mymalloc(sizeof(struct currhp));
	hp->ptr = p - 1;
	hp->nxt = mmcp;
	hp->prev = mmcp->prev;
	hp->prev->nxt = hp;
	hp->nxt->prev = hp;
	return p;
}


void *mycalloc(size_t n, size_t size)
{
	char *mp; // sizeof(char) is always 1 
	if ((mp = mymalloc(n * size)) == NULL)
		return NULL;
	for (size_t i = 0; i < n * size; ++i)
		mp[i] = 0;
	return mp;
}


void printfreep(void)
{
	Header *p = freep;
	do {
		printf("%p->size = %u\n", (void *)p, p->s.size);
		p = p->s.ptr;
	} while (p != freep);

}


int main(void)
{
	printf("10 * sizeof(int) == %zu units\n", (10 * sizeof(int) + sizeof(Header) - 1) / sizeof(Header) + 1);
	printf("sizeof(struct currhp) == %zu units\n", (sizeof(struct currhp) + sizeof(Header) - 1) / sizeof(Header) + 1);
	printf("===========================================\n");

	int *a = mycalloc(10, sizeof(int));
	printfreep();
	printf("===========================================\n");

	int *b = mymalloc(10 * sizeof(int));
	printfreep();
	printf("===========================================\n");

	int *c = mycalloc(10, sizeof(int));
	printfreep();
	printf("===========================================\n");

	int *d = mymalloc(NALLOC * sizeof(Header));
	printfreep();
	printf("===========================================\n");

	myfree(a);
	printfreep();
	printf("===========================================\n");

	myfree(b);
	printfreep();
	printf("===========================================\n");

	myfree(c);
	printfreep();
	printf("===========================================\n");

	myfree(d);
	printfreep();

	return 0;
}
