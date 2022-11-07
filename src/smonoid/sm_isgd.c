#include "smonoid.h"
#include "uilist.h"
int
sm_isgd(struct classifier * b, int x)
{
	struct eggbox * p;
	int max_allowed;
	int num_reg = 0;
	if (!b || (x && !b->localsm)) { return 1; }
	max_allowed = !!(x && b->semigroup == b->localsm->box) + 1;
	for (p = b->semigroup; p; p = p->next)
	{
		if (p->polyegg || p->nonfull) { return 0; }
		if (!p->irregular) { ++num_reg; }
		if (num_reg > max_allowed) { return 0; }
	}
	return 1;
}
