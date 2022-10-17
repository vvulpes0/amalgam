#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
struct finsa *
fi_copy(struct finsa * m)
{
	struct finsa * o;
	if (!m || !m->graphs) { return NULL; }
	o = malloc(sizeof(*o));
	if (!o) { return NULL; }
	o->graphs = malloc(m->count * sizeof(*(o->graphs)));
	if (!o->graphs) { free(o); return NULL; }
	o->finals = ui_copy(m->finals);
	if (m->finals && !o->finals) { free(o); return NULL; }
	for (o->count = 0; o->count < m->count; ++o->count)
	{
		o->graphs[o->count] = bx_copy(m->graphs[o->count]);
		if (m->graphs[o->count] && !o->graphs[o->count])
		{
			fi_free(o);
			return NULL;
		}
	}
	return o;
}
