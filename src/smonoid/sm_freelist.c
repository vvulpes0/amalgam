#include "smonoid.h"
#include <stdlib.h>
void
sm_freelist(struct eggboxes * p)
{
	struct eggboxes * t;
	while (p)
	{
		t = p->next;
		sm_freebox(p->box);
		free(p);
		p = t;
	}
}
