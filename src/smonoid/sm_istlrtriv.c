#include "smonoid.h"
int
sm_istlrtriv(struct eggboxes * s)
{
	return sm_ese(s, sm_isrtriv, 1);
}
