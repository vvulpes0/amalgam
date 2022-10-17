#include "bmatrix.h"
#include "uilist.h"

int
bx_add(struct bmatrix * a, struct bmatrix * b)
{
	size_t i;
	if (!a || !a->vecs || !b || !b->vecs) { return 0; }
	if (a->size != b->size) { return 0; }
	for (i = 0; i < a->size; ++i)
	{
		a->vecs[i] = ui_merge(a->vecs[i], b->vecs[i]);
	}
	return 1;
}
