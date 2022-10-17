#include "bmatrix.h"
#include "uilist.h"
struct uilist *
bx_vmmul(struct uilist * v, struct bmatrix * m)
{
	struct uilist * r = NULL;
	if (!m || !m->vecs) { return NULL; }
	while (v)
	{
		if (v->value >= m->size) { continue; } /* invalid */
		ui_merge(v, m->vecs[v->value]);
	}
	return r;
}
