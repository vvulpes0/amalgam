#include "smonoid.h"
int
sm_islj(struct classifier * s, int x)
{
	return sm_ese(s, sm_ispt, x);
}
