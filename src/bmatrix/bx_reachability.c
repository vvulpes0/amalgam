#include "bmatrix.h"
#include <stdlib.h>
int
bx_reachability(struct bmatrix * m)
{
	struct bmatrix * a;
	struct bmatrix * i;
	struct uilist * t;
	size_t x = 1;
	if (!m || !m->vecs) { return 1; }
	a = bx_copy(m);
	if (!a) { return 0; }
	i = bx_identity(a->size);
	if (!i) { bx_free(a); return 0; }
	if (!bx_add(a, i)) { bx_free(a); bx_free(i); return 0; }
	bx_free(i);
	while (x < a->size)
	{
		if (!bx_mul(a, a)) { bx_free(a); return 0; }
		x += x;
	}
	for (x = 0; x < a->size; ++x)
	{
		t = m->vecs[x];
		m->vecs[x] = a->vecs[x];
		a->vecs[x] = t;
	}
	bx_free(a);
	return 1;
}
