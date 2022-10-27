#include "smonoid.h"
#include "uilist.h"
int
sm_istriv(struct eggbox * b)
{
	if (!b) { return 1; }
	return (b->rows == 1 && b->cols == 1 && !b->polyeggs && !b->next);
}
