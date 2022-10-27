#include "smonoid.h"
#include "uilist.h"
int
sm_isrtriv(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->polyeggs || b->cols != 1) { return 0; }
	}
	return 1;
}
