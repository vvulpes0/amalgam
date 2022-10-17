#include "bmatrix.h"
#include "uilist.h"
int
bx_eq(struct bmatrix * a, struct bmatrix * b)
{
	size_t i;
	if (!a) { return !b; }
	if (!b) { return 0; }
	if (a->size != b->size) { return 0; }
	for (i = 0; i < a->size; ++i)
	{
		if (!ui_eq(a->vecs[i], b->vecs[i])) { return 0; }
	}
	return 1;
}
