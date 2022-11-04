#include "smonoid.h"
int
sm_ese(struct eggboxes * b, int (*f)(struct eggbox *), int x)
{
	if (!b) { return 1; }
	if (x && !b->is_proper)
	{
		struct eggbox * p = b->box;
		while (!p->has_id) { p = p->next; }
		if (p && p->rows == 1 && p->cols == 1 && !p->polyeggs)
		{
			b = b->next;
		}
	}
	for (; b; b = b->next) { if (!f(b->box)) { return 0; } }
	return 1;
}
