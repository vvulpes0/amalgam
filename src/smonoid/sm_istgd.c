#include "smonoid.h"
#include "uilist.h"
int
sm_istgd(struct classifier * b)
{
	return sm_isgd(b, 1);
}
