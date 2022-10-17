#include "smonoid.h"
int
sm_isacom(struct eggbox * b, struct finsa * m)
{
	return sm_issf(b) && sm_iscom(m);
}
