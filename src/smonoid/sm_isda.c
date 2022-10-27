#include "smonoid.h"
#include "uilist.h"
int
sm_isda(struct eggbox * b)
{
	size_t r;
	size_t c;
	_Bool i;
	for (; b; b = b->next)
	{
		if (b->polyeggs) { return 0; }
		i = b->groups[0];
		for (r = 0; r < b->rows * b->cols; ++r)
		{
			/* unmatched idempotency */
			if (b->groups[r] != i) { return 0; }
		}
	}
	return 1;
}
