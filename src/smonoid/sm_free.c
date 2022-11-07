#include "smonoid.h"
#include <stdlib.h>
void
sm_free(struct classifier * p)
{
	struct eggboxes * t;
	if (p->localsm && p->localsm->box == p->semigroup)
	{
		t = p->localsm;
		p->localsm = t->next;
		t->box = NULL;
		t->next = NULL;
		sm_freelist(t);
	}
	sm_freebox(p->semigroup);
	sm_freelist(p->localsm);
	free(p);
}
