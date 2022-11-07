#include "smonoid.h"
#include "uilist.h"
int
sm_isltriv(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->polyegg || b->polyrow) { return 0; }
	}
	return 1;
}
