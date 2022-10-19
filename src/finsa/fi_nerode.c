#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
/* return index of the pair (p,q) in a triangular array */
static unsigned int
_index(unsigned int n, unsigned int p, unsigned int q)
{
	unsigned int i;
	if (p > q) { p ^= q; q ^= p; p ^= q; }
	q -= p + 1;
	for (i = 0; p; --p)
	{
		i += n;
		--n;
	}
	return i + q;
}

/* return nonzero iff new distinctions were made */
static int
_expand(struct finsa * f, char * d, unsigned int n)
{
	struct bmatrix * m;
	size_t c;
	size_t i;
	size_t j;
	unsigned int x;
	unsigned int a;
	unsigned int b;
	int changed = 0;
	if (!f || !d || !n) { return 0; }
	/* for each character */
	for (c = 0; c < f->count; ++c)
	{
		m = f->graphs[c];
		/* loop over state-pairs */
		for (i = 0, x = 0; i < n; ++i)
		{
			a = m->vecs[i] ? m->vecs[i]->value : n;
			b = n;
			for (j = i + 1; j <= n; ++j, ++x)
			{
				/* already distinct! */
				if (d[x]) { continue; }
				/* j==n is the augmented sink-state */
				b = ((j < n && m->vecs[j])
				     ? m->vecs[j]->value
				     : n);
				/* indistinct output won't help! */
				if (a == b) { continue; }
				if (!d[_index(n,a,b)]) { continue; }
				/* if they are distinct, flag it */
				d[x] = 1;
				changed = 1;
			}
		}
	}
	return changed;
}

int
fi_nerode(struct finsa * f)
{
	unsigned int * updates;
	struct bmatrix * m;
	struct uilist * states;
	struct uilist * p;
	struct uilist * q;
	char * d;
	size_t c;
	size_t i;
	size_t j;
	unsigned int num_states;
	unsigned int n;
	unsigned int x;
	if (!f || !f->graphs || f->count < 2) { return 1; }
	if (!f->graphs[0]) { return 0; }
	n = f->graphs[0]->size;
	d = malloc(n * (n+1) / 2 * sizeof(*d));
	if (!d) { return 0; }
	for (i = 0, x = 0; i < n; ++i)
	{
		for (j = i + 1; j <= n; ++j, ++x)
		{
			d[x] = (!ui_find(f->finals, i)
			        != !ui_find(f->finals, j));
		}
	}
	/* fixed point of expansion */
	while (_expand(f, d, n));
	updates = malloc(n * sizeof(*updates));
	if (!updates) { free(d); return 0; }
	for (i = 0, x = 0; i < n; ++i)
	{
		updates[i] = i;
		for (j = i + 1; j <= n; ++j, ++x)
		{
			if (!d[x]) { updates[i] = j; }
		}
	}
	free(d);
	/* keep only the desired states */
	states = NULL;
	for (i = 0; i < n; ++i)
	{
		if (updates[i] == n) { continue; }
		states = ui_insert(states, updates[i]);
	}
	num_states = 0;
	for (p = states; p; p = p->next) { ++num_states; }

	/* for each symbol (graph) */
	for (c = 0; c < f->count; ++c)
	{
		m = f->graphs[c];
		/* start by merging matrix rows */
		for (i = 0; i < m->size; ++i)
		{
			if (updates[i] == n) { continue; }
			m->vecs[updates[i]]
				= ui_merge(m->vecs[updates[i]],
				           m->vecs[i]);
		}
		/* then update the appropriate rows */
		for (p = states; p; p = p->next)
		{
			if (p->value == n) { continue; }
			for (q = m->vecs[p->value]; q; q = q->next)
			{
				if (q->value >= n) { continue; }
				if (updates[q->value] >= n) { continue; }
				m->vecs[p->value]
					= ui_insert(m->vecs[p->value],
					            updates[q->value]);
			}
		}
	}
	free(updates);
	fi_restrict(f, states);
	return 1;
}
