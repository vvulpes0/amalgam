#include "smonoid.h"
int
sm_islda(struct eggboxes * s, int x)
{
	return sm_ese(s, sm_isda, x);
}
