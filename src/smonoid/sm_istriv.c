#include "smonoid.h"
#include "uilist.h"
int
sm_istriv(struct eggbox * b)
{
	if (!b) { return 1; }
	return (!b->next && !b->eggs[0]->next
	        && b->rows == 1 && b->cols == 1);
}
