#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
struct finsa *
sm_generate(struct uilist * v, struct finsa * m)
{
	struct finsa * c = fi_copy(m);
	struct finsa * s;
	struct uilist * p;
	void * t;
	size_t i = 0;
	size_t j;
	if (!c) { return NULL; }

	/* we don't care about the final states
	 * when generating syntactic monoids
	 */
	ui_free(c->finals);
	c->finals = NULL;

	/* move desired elements to the front of the system */
	p = v;
	while (p && i < c->count)
	{
		for (j = i; j < p->value; ++j)
		{
			bx_free(c->graphs[j]);
			c->graphs[j] = NULL;
		}
		t = c->graphs[p->value];
		c->graphs[p->value] = NULL;
		c->graphs[i] = t;
		++i;
		p = p->next;
	}
	for (j = i; j < c->count; ++j)
	{
		bx_free(c->graphs[j]);
	}
	c->count = i;

	fi_restrict(c, v);
	/* make the monoid, thereby generating any missing elements */
	s = fi_smonoid(c);
	fi_free(c);
	return s;
}
