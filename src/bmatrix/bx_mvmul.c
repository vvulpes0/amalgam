#include "bmatrix.h"
#include "uilist.h"
struct uilist *
bx_vmmul(struct uilist * v, struct bmatrix * n)
{
	struct uilist * r = NULL;
	struct bmatrix * m = bx_copy(n);
	if (!m || !m->vecs) { return NULL; }
	if (!bx_transpose(m)) { bx_free(m); return NULL; }
	while (v)
	{
		if (v->value >= m->size) { continue; } /* invalid */
		ui_merge(v, m->vecs[v->value]);
	}
	bx_free(m);
	return r;
}
