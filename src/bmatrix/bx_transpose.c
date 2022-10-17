#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
int
bx_transpose(struct bmatrix * m)
{
	struct uilist ** x;
	struct uilist * t;
	size_t i;
	if (!m || !m->vecs || !m->size) { return 1; }
	x = malloc(m->size * sizeof(*x));
	if (!x) { return 0; } /* oh no, no memory */
	for (i = 0; i < m->size; ++i)
	{
		x[i] = m->vecs[i];
		m->vecs[i] = NULL;
	}
	for (i = m->size - 1; i + 1 > 0; --i)
	{
		while (x[i])
		{
			t = x[i];
			x[i] = t->next;
			if (t->value >= m->size)
			{
				/* ill-formed structure! */
				free(t);
				continue;
			}
			t->next = m->vecs[t->value];
			m->vecs[t->value] = t;
			t->value = i;
		}
	}
	free(x);
	return 1;
}
