#include "smonoid.h"
#include "uilist.h"
int
sm_isband(struct eggbox * b)
{
	size_t r;
	size_t c;
	for (; b; b = b->next)
	{
		for (r = 0; r < b->rows; ++r)
		{
			for (c = 0; c < b->cols; ++c)
			{
				if (b->eggs[r * b->cols + c]->next)
				{
					/* not even SF */
					return 0;
				}
				if (!b->groups[r * b->cols + c])
				{
					/* non-idempotent element */
					return 0;
				}
			}
		}
	}
	return 1;
}
