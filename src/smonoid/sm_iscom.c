#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
int
sm_iscom(struct finsa * m)
{
	size_t i;
	size_t j;
	unsigned int x;
	unsigned int y;
	if (!m) { return 1; }
	if (!m->graphs) { return 0; }
	for (i = 0; i < m->count; ++i)
	{
		if (!m->graphs[i]) { return 0; }
		if (!m->graphs[i]->vecs) { return 0; }
		for (j = i + 1; j < m->count; ++j)
		{
			if (!m->graphs[i]->vecs[j]) { return 0; }
			x = m->graphs[i]->vecs[j]->value;
			if (!m->graphs[j]) { return 0; }
			if (!m->graphs[j]->vecs) { return 0; }
			if (!m->graphs[j]->vecs[i]) { return 0; }
			y = m->graphs[j]->vecs[i]->value;
			if (x != y) { return 0; }
		}
	}
	return 1;
}
