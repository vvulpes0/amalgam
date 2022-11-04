#include "smonoid.h"
#include "uilist.h"
#include <stdlib.h>
void
sm_free(struct eggbox * p)
{
	struct eggbox * t;
	while (p)
	{
		t = p->next;
		free(p->groups);
		p->groups = NULL;
		free(p);
		p = t;
	}
}
