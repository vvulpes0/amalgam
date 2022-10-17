#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
struct bmatrix *
sm_lrel(struct finsa * m)
{
	struct bmatrix * o;
	size_t i;
	if (!m || !m->graphs || !m->count) { return NULL; }
	if (m->count != m->graphs[0]->size) { return NULL; }
	o = malloc(sizeof(*o));
	if (!o) { return NULL; }
	o->vecs = malloc(m->count * sizeof(*(o->vecs)));
	if (!o->vecs) { free(o); return NULL; }
	for (o->size = 0; o->size < m->count; ++(o->size))
	{
		o->vecs[o->size] = NULL;
		/* Sx : union each rows of the x'th matrix */
		for (i = 0; i < m->graphs[o->size]->size; ++i)
		{
			o->vecs[o->size]
				= ui_merge(o->vecs[o->size],
				           m->graphs[o->size]->vecs[i]);
		}
	}
	return o;
}
