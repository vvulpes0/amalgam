#include "smonoid.h"
int
sm_issemilat(struct eggbox * b)
{
	return sm_isband(b) && sm_ispt(b);
}
