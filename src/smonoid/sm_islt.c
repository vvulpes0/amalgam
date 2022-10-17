#include "smonoid.h"
int
sm_islt(struct eggboxes * b, int x)
{
	return sm_ese(b, sm_issemilat, x);
}
