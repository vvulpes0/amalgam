#include "bmatrix.h"
#include "uilist.h"
struct uilist *
bx_vmmul(struct uilist * v, struct bmatrix * m)
{
	struct uilist * r = NULL;
	if (!m || !m->vecs) { return NULL; }
	while (v)
	{
		if (v->value >= m->size) { break; } /* invalid */
		r = ui_merge(r, m->vecs[v->value]);
		v = v->next;
	}
	return r;
}
