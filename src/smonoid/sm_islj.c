#include "smonoid.h"
int
sm_islj(struct eggboxes * s, int x)
{
	return sm_ese(s, sm_ispt, x);
}
