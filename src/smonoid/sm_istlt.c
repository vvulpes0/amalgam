#include "smonoid.h"
int
sm_istlt(struct eggboxes * b)
{
	return sm_ese(b, sm_issemilat, 1);
}
