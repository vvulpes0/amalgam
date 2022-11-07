#include "smonoid.h"
int
sm_istlrtriv(struct classifier * s)
{
	return sm_ese(s, sm_isrtriv, 1);
}
