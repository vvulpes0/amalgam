#include "smonoid.h"
int
sm_ese(struct classifier * c, int (*f)(struct eggbox *), int x)
{
	struct eggboxes * b;
	if (!c || !c->localsm) { return 1; }
	b = c->localsm;
	if (x && c->semigroup == b->box)
	{
		b = b->next;
	}
	for (; b; b = b->next) { if (!f(b->box)) { return 0; } }
	return 1;
}
