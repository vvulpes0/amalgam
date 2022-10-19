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

void
fi_restrict(struct finsa * c, struct uilist * v)
{
	struct uilist * p;
	void * t;
	size_t i;
	size_t j;
	size_t k;
	size_t x;
	if (!c || !c->graphs || c->graphs[0]->size < 1) { return; }
	c->finals = ui_intersect(c->finals, v);
	_rename(c->finals, v);
	for (p = v, x = 0; p; p = p->next, ++x);
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
		for (; k < c->graphs[i]->size; ++k)
		{
			ui_free(c->graphs[i]->vecs[k]);
		}
		c->graphs[i]->size = x;
	}

	if (c->graphs[0]->size) { return; }
	/* oops we took away every element! */
	/* make it a one-state sink instead */
	for (i = 0; i < c->count; ++i)
	{
		c->graphs[i]->size = 1;
		c->graphs[i]->vecs[0]
			= ui_insert(c->graphs[i]->vecs[0], 0);
	}
}
