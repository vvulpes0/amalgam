#include "smonoid.h"
int
sm_istlda(struct classifier * s)
{
	return sm_ese(s, sm_isda, 1);
}
