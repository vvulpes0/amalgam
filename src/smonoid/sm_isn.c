#include "smonoid.h"
#include "uilist.h"
int
sm_isn(struct eggbox * b, int x)
{
	_Bool i;
	_Bool ever_i = 0;
	for (; b; b = b->next)
	{
		if (b->polyeggs) { return 0; }
		if (b->rows != 1 || b->cols != 1) { return 0; }
		if (x && b->has_id) { continue; }
		i = b->groups[0];
		if (ever_i && i) { return 0; }
		ever_i |= i;
	}
	return 1;
}
