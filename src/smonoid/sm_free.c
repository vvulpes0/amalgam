#include "smonoid.h"
#include "uilist.h"
#include <stdlib.h>
void
sm_free(struct eggbox * p)
{
	struct eggbox * t;
	size_t i;
	while (p)
	{
		t = p->next;
		for (i = 0; i < p->rows * p->cols; ++i)
		{
			ui_free(p->eggs[i]);
			p->eggs[i] = NULL;
		}
		free(p->eggs);
		p->eggs = NULL;
		free(p->groups);
		p->groups = NULL;
		free(p);
		p = t;
	}
}
