#include "smonoid.h"
#include "uilist.h"
int
sm_isrtriv(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->polyegg || b->polycol) { return 0; }
	}
	return 1;
}
