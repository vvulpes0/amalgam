#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
struct bmatrix *
fi_dblgraph(struct finsa * f)
{
	struct bmatrix * o;
	size_t i;
	size_t n;
	unsigned int a;
	unsigned int b;
	unsigned int x;
	unsigned int y;
	if (!f || !f->count || !f->graphs) { return NULL; }
	if (!f->graphs[0] || !f->graphs[0]->size) { return NULL; }
	n = f->graphs[0]->size;
	if (n < 2) { return NULL; }
	o = malloc(sizeof(*o));
	o->vecs = malloc(sizeof(*(o->vecs)) * n * (n - 1));
	a = 0;
	b = 0;
	for (o->size = 0; o->size < n * (n-1); ++(o->size))
	{
		b += 1;
		b += (b == a);
		if (b == n) { ++a; b = 0; }
		o->vecs[o->size] = NULL;
		for (i = 0; i < f->count; ++i)
		{
			if (!f->graphs[i]->vecs[a]) { continue; }
			if (!f->graphs[i]->vecs[b]) { continue; }
			x = f->graphs[i]->vecs[a]->value;
			y = f->graphs[i]->vecs[b]->value;
			if (x == y) { continue; }
			o->vecs[o->size]
				= ui_insert(o->vecs[o->size],
				            x * (n-1) + y - (x<=y));
		}
	}
	return o;
}
