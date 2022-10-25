#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
int
bx_mul(struct bmatrix * a, struct bmatrix * m)
{
	struct bmatrix * n;
	struct bmatrix * r;
	struct uilist * t;
	size_t i;
	size_t j;
	if (!a || !a->vecs || !m || !m->vecs) { return 0; }
	if (a->size != m->size) { return 0; }
	if (a->size == 0) { return 1; }
	r = malloc(sizeof(*r));
	if (!r) { return 0; }
	r->size = a->size;
	r->vecs = malloc(r->size * sizeof(*(r->vecs)));
	if (!r->vecs) { free(r); return 0; }
	for (i = 0; i < r->size; ++i)
	{
		r->vecs[i] = NULL;
	}
	n = bx_copy(m);
	if (!n) { bx_free(r); return 0; }
	if (!n->vecs) { free(n); bx_free(r); return 0; }
	if (!bx_transpose(n)) { bx_free(n); bx_free(r); return 0; }
	for (i = 0; i < a->size; ++i)
	{
		for (j = n->size; j--;)
		{
			if (!ui_has_intersect(a->vecs[i],n->vecs[j]))
			{
				continue;
			}
			t = malloc(sizeof(*t));
			if (!t)
			{
				bx_free(r);
				bx_free(n);
				return 0;
			}
			t->next = r->vecs[i];
			t->value = j;
			r->vecs[i] = t;
		}
	}
	for (i = 0; i < a->size; ++i)
	{
		ui_free(a->vecs[i]);
		a->vecs[i] = r->vecs[i];
	}
	free(r->vecs);
	free(r);
	bx_free(n);
	return 1;
}
