#include "smonoid.h"
int
sm_islda(struct classifier * s, int x)
{
	return sm_ese(s, sm_isda, x);
}
