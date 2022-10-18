#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
void
fi_rmeps(struct finsa * m, int e)
{
	struct uilist * v;
	struct bmatrix * t;
	size_t i;
	size_t j;
	if (!m || !m->graphs) { return; }
	if (e >= m->count) { return; }
	if (!bx_reachability(m->graphs[e])) { return; }
	for (i = 0; i < m->count; ++i)
	{
		/* unnecessary test saves plenty of multiplication */
		if (i == e) { continue; }
		for (j = 0; j < m->graphs[i]->size; ++j)
		{
			v = bx_vmmul(m->graphs[i]->vecs[j],
			             m->graphs[e]);
			ui_free(m->graphs[i]->vecs[j]);
			m->graphs[i]->vecs[j] = v;
		}
	}
	t = m->graphs[e];
	m->graphs[e] = m->graphs[m->count - 1];
	m->graphs[m->count - 1] = t;
	bx_free(m->graphs[m->count - 1]);
	--(m->count);
}
