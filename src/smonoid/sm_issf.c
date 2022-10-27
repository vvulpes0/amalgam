#include "smonoid.h"
#include "uilist.h"
int
sm_issf(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->polyeggs) { return 0; }
	}
	return 1;
}
