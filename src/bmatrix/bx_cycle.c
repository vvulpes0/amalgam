#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
struct bmatrix *
bx_cycle(size_t v)
{
	struct bmatrix * p = malloc(sizeof(*p));
	if (!p) { return p; }
	p->vecs = malloc(v * sizeof(*(p->vecs)));
	for (p->size = 0; p->size < v; ++(p->size))
	{
		p->vecs[p->size] = malloc(sizeof(*(p->vecs[0])));
		if (!p->vecs[p->size]) { break; }
		p->vecs[p->size]->next = NULL;
		p->vecs[p->size]->value = p->size + 1;
	}
	if (p->size == v) { p->vecs[p->size - 1]->value = 0; return p; }
	/* everything already allocated is lower than the set size */
	bx_free(p);
	return NULL;
}
