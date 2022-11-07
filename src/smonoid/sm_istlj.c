#include "smonoid.h"
int
sm_istlj(struct classifier * s)
{
	return sm_ese(s, sm_ispt, 1);
}
