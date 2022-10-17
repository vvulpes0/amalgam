#include "smonoid.h"
int
sm_istlda(struct eggboxes * s)
{
	return sm_ese(s, sm_isda, 1);
}
