#include "smonoid.h"
#include "uilist.h"
int
sm_isband(struct eggbox * b)
{
	size_t i;
	for (; b; b = b->next)
	{
		if (b->polyeggs) { return 0; }
		for (i = 0; i < b->rows * b->cols; ++i)
		{
			/* non-idempotent element */
			if (!b->groups[i]) { return 0; }
		}
	}
	return 1;
}
