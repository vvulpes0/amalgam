#include "smonoid.h"
#include "uilist.h"
int
sm_issf(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->eggs[0]->next) { return 0; }
	}
	return 1;
}
