#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
struct eggboxes *
sm_localsm(struct finsa * m, int * all_comm)
{
	struct eggboxes * o = NULL;
	struct eggboxes ** oe = &o;
	struct finsa * f;
	struct uilist * v;
	struct uilist * x;
	size_t i;
	size_t j;
	if (all_comm) { *all_comm = 1; }
	if (!m || !m->graphs) { return NULL; }
	if (!m->graphs[0]) { return NULL; }
	if (m->count != m->graphs[0]->size) { return NULL; }
	/* now we're safe to believe we have an actual semigroup */
	for (i = 0; i < m->count; ++i)
	{
		if (!m->graphs[i] || !m->graphs[i]->vecs)
		{
			sm_freelist(o);
			return NULL;
		}
		if (!m->graphs[i]->vecs[i]) { continue; }
		if (i != m->graphs[i]->vecs[i]->value) { continue; }
		/* here we are idempotent! */
		/* compute eS by unioning matrix rows */
		v = NULL;
		for (j = 0; j < m->count; ++j)
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
		*oe = malloc(sizeof(**oe));
		if (!*oe)
		{
			sm_freelist(o);
			fi_free(f);
			ui_free(v);
			return NULL;
		}
		(*oe)->is_proper = 1;
		(*oe)->next = NULL;
		(*oe)->box = sm_eggbox(f);
		if (!(*oe)->box)
		{
			sm_freelist(o);
			fi_free(f);
			ui_free(v);
			return NULL;
		}
		if (!i && m->finals) { (*oe)->is_proper = 0; }
		oe = &((*oe)->next);
		/* if you care about commutativity of the semigroup,
		 * calculate that yourself. */
		if (all_comm && (i || !m->finals))
		{
			*all_comm &= !!sm_iscom(f);
		}
		fi_free(f);
		ui_free(v);
	}
	return o;
}
