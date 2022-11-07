#include "smonoid.h"
#include "uilist.h"
int
sm_istd(struct classifier * b)
{
	return sm_isd(b, 1);
}
