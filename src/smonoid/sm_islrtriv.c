#include "smonoid.h"
int
sm_islrtriv(struct eggboxes * s, int x)
{
	return sm_ese(s, sm_isrtriv, x);
}
