#include "smonoid.h"
#include "uilist.h"
int
sm_isltriv(struct eggbox * b)
{
	for (; b; b = b->next)
	{
		if (b->eggs[0]->next || b->rows != 1) { return 0; }
	}
	return 1;
}
