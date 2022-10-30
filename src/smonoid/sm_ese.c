#include "smonoid.h"
int
sm_ese(struct eggboxes * b, int (*f)(struct eggbox *), int x)
{
	struct eggbox * p;
	if (!b) { return 1; }
	if (x && !b->is_proper) { b = b->next; }
	for (; b; b = b->next) { if (!f(b->box)) { return 0; } }
	return 1;
}
