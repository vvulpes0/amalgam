#include "smonoid.h"
#include "uilist.h"
int
sm_isgd(struct eggbox * b, int x)
{
	size_t r;
	_Bool i;
	_Bool ever_i = 0;
	for (; b; b = b->next)
	{
		if (b->polyeggs) { return 0; }
		if (x && b->rows == 1 && b->cols == 1 && b->has_id)
		{
			continue;
		}
		i = b->groups[0];
		if (ever_i && i) { return 0; }
		ever_i |= i;
		for (r = 0; r < b->rows * b->cols; ++r)
		{
			/* mismatched idempotency */
			if (b->groups[r] != i) { return 0; }
		}
	}
	return 1;
}
