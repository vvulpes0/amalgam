#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
int
fi_issl(struct finsa * m)
{
	struct finsa * psg;
	struct uilist * p = NULL;
	struct uilist * q = NULL;
	size_t i;
	if (!m || !m->graphs || !m->count) { return 1; }
	for (i = 0; i < m->graphs[0]->size; ++i)
	{
		p = ui_insert(p, (unsigned int)i);
	}
	psg = fi_powerset(m, p, POLY);
	ui_free(p);
	p = NULL;
	if (!psg || !psg->graphs || !psg->count) { return 1; }
	for (i = 1; i < psg->count; ++i)
	{
		bx_add(psg->graphs[0], psg->graphs[i]);
	}
	/* check for self-loops */
	for (p = psg->finals; p; p = p->next)
	{
		if (ui_find(psg->graphs[0]->vecs[p->value], p->value))
		{
			fi_free(psg);
			return 0;
		}
	}
	/* check for bigger cycles */
	bx_reachability(psg->graphs[0]);
	for (p = psg->finals; p; p = p->next)
	{
		for (q = p->next; q; q = q->next)
		{
			if (!ui_find(psg->graphs[0]->vecs[p->value],
			             q->value))
			{
				continue;
			}
			if (!ui_find(psg->graphs[0]->vecs[q->value],
			             p->value))
			{
				continue;
			}
			fi_free(psg);
			return 0;
		}
	}
	fi_free(psg);
	return 1;
}
