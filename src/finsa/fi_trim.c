#include "finsa.h"
#include "bmatrix.h"
int
fi_trim(struct finsa * f)
{
	struct bmatrix * m;
	size_t i;
	if (!f || !f->graphs || !f->count) { return 0; }
	m = bx_copy(f->graphs[0]);
	if (!m) { return 0; }
	for (i = 1; i < f->count; ++i)
	{
		if (!bx_add(m, f->graphs[i])) { bx_free(m); return 0; }
	}
	if (!bx_reachability(m)) { bx_free(m); return 0; }
	fi_restrict(f, m->vecs[0]);
	bx_free(m);
	return 1;
}
