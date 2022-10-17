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
		i = b->groups[0];
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
