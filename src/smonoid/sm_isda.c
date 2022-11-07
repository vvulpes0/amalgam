#include "smonoid.h"
#include "uilist.h"
int
sm_isda(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->polyegg || b->nonfull) { return 0; }
	}
	return 1;
}
