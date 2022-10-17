#include "smonoid.h"
#include "uilist.h"
int
sm_isf(struct eggbox * b, int x)
{
	size_t r;
	size_t c;
	_Bool i;
	_Bool ever_i = 0;
	for (; b; b = b->next)
	{
		if (b->rows != 1 || b->cols != 1) { return 0; }
		if (x && b->rows == 1 && b->cols == 1
		    && b->eggs[0]->value == 0
		    && !b->eggs[0]->next)
		{
			continue;
		}
		i = b->groups[0];
		if (ever_i && i) { return 0; }
		ever_i |= i;
		for (r = 0; r < b->rows; ++r)
		{
			for (c = 0; c < b->cols; ++c)
			{
				if (b->eggs[r * b->cols + c]->next)
				{
					/* not even SF */
					return 0;
				}
				if (b->groups[r * b->cols + c] != i)
				{
					/* unmatched idempotency */
					return 0;
				}
			}
		}
	}
	return 1;
}
