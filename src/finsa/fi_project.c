#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
static int
_isid(struct bmatrix * g)
{
	size_t i;
	if (!g || !g->vecs) { return 0; }
	for (i = 0; i < g->size; ++i)
	{
		if (!g->vecs[i]) { return 0; }
		if (g->vecs[i]->next) { return 0; }
		if (g->vecs[i]->value != i) { return 0; }
	}
	return 1;
}

void
fi_project(struct finsa * m)
{
	struct bmatrix * t;
	size_t i;
	size_t n = 0;
	if (!m || !m->graphs || !m->count) { return; }
	for (i = 0; i < m->count - n; ++i)
	{
		if (!_isid(m->graphs[i])) { continue; }
		++n;
		t = m->graphs[i];
		m->graphs[i] = m->graphs[m->count - n];
		m->graphs[m->count - n] = t;
		--i;
	}
	for (i = m->count - n; i < m->count; ++i)
	{
		bx_free(m->graphs[i]);
		m->graphs[i] = NULL;
	}
	m->count -= n;
}
