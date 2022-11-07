#include "smonoid.h"
#include "uilist.h"
int
sm_isband(struct eggbox * b)
{
	size_t i;
	for (; b; b = b->next)
	{
		if (b->polyegg || b->irregular || b->nonfull) { return 0; }
	}
	return 1;
}
