#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
static struct eggbox *
fill_box(struct finsa * m, unsigned int i, int *all_comm)
{
	struct finsa * f = NULL;
	struct eggbox * o = NULL;
	struct uilist * v = NULL;
	struct uilist * x = NULL;
	/* compute eS by unioning matrix rows */
	for (size_t j = 0; j < m->count; ++j)
	{
		v = ui_merge(v, m->graphs[j]->vecs[i]);
	}
	x = v;
	/* now eS*e is a simple matrix multiplication */
	v = bx_vmmul(x, m->graphs[i]);
	ui_free(x);
	x = NULL;
	/* restrict to v=eSe */
	f = sm_generate(v, m);
	/* we should never be passing in i==0, this is fine */
	if (all_comm)
	{
		*all_comm = *all_comm && !!sm_iscom(f);
	}
	o = sm_eggbox(f);
	fi_free(f);
	ui_free(v);
	return o;
}

struct eggboxes *
sm_localsm(struct finsa * m, int * all_comm)
{
	struct eggboxes * o = NULL;
	struct eggboxes ** oe = &o;
	unsigned int * idempotents = NULL;
	struct eggbox *** boxes = NULL;
	size_t i;
	size_t num_idempotents = 0;
	if (all_comm) { *all_comm = 1; }
	if (!m || !m->graphs) { return NULL; }
	if (!m->graphs[0]) { return NULL; }
	if (m->count != m->graphs[0]->size) { return NULL; }
	/* now we're safe to believe we have an actual semigroup
	 * start by generating the skeleton eggbox list:
	 */
	idempotents = calloc(m->count, sizeof(*idempotents));
	if (!idempotents) { return NULL; }
	boxes = malloc(m->count * sizeof(*boxes));
	if (!boxes) { free(idempotents); return NULL; }
	for (i = 1; i < m->count; ++i)
	{
		if (!m->graphs[i] || !m->graphs[i]->vecs)
		{
			sm_freelist(o);
			return NULL;
		}
		if (!m->graphs[i]->vecs[i]) { continue; }
		if (i != m->graphs[i]->vecs[i]->value) { continue; }
		/* here we are idempotent!
		 * allocate a box */
		*oe = malloc(sizeof(**oe));
		if (!*oe)
		{
			sm_freelist(o);
			return NULL;
		}
		(*oe)->next = NULL;
		(*oe)->box = NULL;
		boxes[num_idempotents] = &((*oe)->box);
		idempotents[num_idempotents++] = i;
		oe = &((*oe)->next);
	}
	/* fill the structure for each nonidentity idempotent */
	int ac = all_comm ? *all_comm : 1;
	#pragma omp parallel for reduction (&&:ac)
	for (i = 0; i < num_idempotents; ++i)
	{
		int tlallcomm = 1;
		*(boxes[i]) = fill_box(m, idempotents[i], &tlallcomm);
		ac = ac && tlallcomm;
	}
	if (all_comm) { *all_comm = ac; }
	free(idempotents);
	free(boxes);
	return o;
}
