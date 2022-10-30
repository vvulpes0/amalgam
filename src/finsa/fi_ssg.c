#include <finsa.h>
#include <bmatrix.h>
#include <uilist.h>

struct finsa *
fi_ssg(struct finsa * m)
{
	struct finsa * o = fi_smonoid(m);
	struct uilist * t;
	size_t i;
	size_t j;
	if (!o) { return NULL; }
	if (o->finals) { return o; }
	if (o->count < 2) { return o; }
	bx_free(o->graphs[0]);
	for (i = 1; i < o->count; ++i)
	{
		ui_free(o->graphs[i]->vecs[0]);
		for (j = 1; j < o->graphs[i]->size; ++j)
		{
			if (!o->graphs[i]->vecs[j]->value)
			{
				t = o->graphs[i]->vecs[j];
				o->graphs[i]->vecs[j]
					= o->graphs[i]->vecs[j]->next;
				t->next = NULL;
				ui_free(t);
			}
			for (t = o->graphs[i]->vecs[j]; t; t = t->next)
			{
				--(t->value);
			}
			o->graphs[i]->vecs[j - 1]
				= o->graphs[i]->vecs[j];
		}
		--(o->graphs[i]->size);
		o->graphs[i - 1] = o->graphs[i];
	}
	--(o->count);
	return o;
}
