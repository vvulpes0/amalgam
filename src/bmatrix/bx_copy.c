#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
struct bmatrix *
bx_copy(struct bmatrix * m)
{
	struct bmatrix * p = malloc(sizeof(*p));
	if (!p) { return p; }
	p->vecs = malloc(m->size * sizeof(*(p->vecs)));
	for (p->size = 0; p->size < m->size; ++p->size)
	{
		p->vecs[p->size] = ui_copy(m->vecs[p->size]);
	}
	return p;
}
