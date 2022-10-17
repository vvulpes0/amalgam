#include "smonoid.h"
int
sm_ispt(struct eggbox * b)
{
	return sm_isltriv(b) && sm_isrtriv(b);
}
