#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
int
fi_issl(struct finsa * m)
{
	struct bmatrix * dbl;
	size_t i;
	size_t j;
	if (!m || !m->graphs || !m->count) { return 1; }
	dbl = fi_dblgraph(m);
	if (!dbl || !dbl->vecs || !dbl->size) { return 1; }
	/* check for self-loops */
	for (i = 0; i < dbl->size; ++i)
	{
		if (ui_find(dbl->vecs[i], i))
		{
			bx_free(dbl);
			return 0;
		}
	}
	/* check for bigger cycles */
	bx_reachability(dbl);
	for (i = 0; i < dbl->size - 1; ++i)
	{
		for (j = i + 1; j < dbl->size; ++j)
		{
			if (!ui_find(dbl->vecs[i], j)) { continue; }
			if (!ui_find(dbl->vecs[j], i)) { continue; }
			bx_free(dbl);
			return 0;
		}
	}
	bx_free(dbl);
	return 1;
}
