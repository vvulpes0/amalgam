#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
void
bx_free(struct bmatrix * m)
{
	size_t i;
	if (!m || !m->vecs) { return; }
	for (i = 0; i < m->size; ++i)
	{
		ui_free(m->vecs[i]);
		m->vecs[i] = NULL;
	}
	free(m->vecs);
	m->vecs = NULL;
	free(m);
}
