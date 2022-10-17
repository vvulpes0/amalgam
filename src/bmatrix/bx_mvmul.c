#include "bmatrix.h"
#include "uilist.h"
struct uilist *
bx_mvmul(struct bmatrix * n, struct uilist * v)
{
	struct uilist * r = NULL;
	struct bmatrix * m = bx_copy(n);
	if (!m || !m->vecs) { return NULL; }
	if (!bx_transpose(m)) { bx_free(m); return NULL; }
	while (v)
	{
		if (v->value >= m->size) { break; } /* invalid */
		r = ui_merge(r, m->vecs[v->value]);
		v = v->next;
	}
	bx_free(m);
	return r;
}
