#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
static void
_rename(struct uilist * a, struct uilist * b)
{
	size_t i = 0;
	while (a && b)
	{
		if (a->value == b->value)
		{
			a->value = i;
			++i;
			b = b->next;
			a = a->next;
			continue;
		}
		if (a->value > b->value)
		{
			b = b->next;
			++i;
			continue;
		}
		a = a->next;
	}
}

struct finsa *
sm_generate(struct uilist * v, struct finsa * m)
{
	struct finsa * c = fi_copy(m);
	struct finsa * s;
	struct uilist * p;
	void * t;
	size_t i = 0;
	size_t j;
	size_t k;
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
	c->count = i;

	/* for each graph, move the desired rows to the front
	 * and remove unwanted elements.
	 * Then rename elements to indices in the smaller set.
	 * */
	for (i = 0; i < c->count; ++i)
	{
		p = v;
		k = 0;
		while (p && k < c->graphs[i]->size)
		{
			for (j = k; j < p->value; ++j)
			{
				ui_free(c->graphs[i]->vecs[j]);
				c->graphs[i]->vecs[j] = NULL;
			}
			t = c->graphs[i]->vecs[p->value];
			c->graphs[i]->vecs[p->value] = NULL;
			c->graphs[i]->vecs[k] = t;
			c->graphs[i]->vecs[k]
				= ui_intersect(c->graphs[i]->vecs[k], v);
			_rename(c->graphs[i]->vecs[k], v);
			++k;
			p = p->next;
		}
		c->graphs[i]->size = c->count;
	}

	/* make the monoid, thereby generating any missing elements */
	s = fi_smonoid(c);
	fi_free(c);
	return s;
}
