#include "smonoid.h"
int
sm_istlt(struct classifier * b)
{
	return sm_ese(b, sm_issemilat, 1);
}
