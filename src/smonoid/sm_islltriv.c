#include "smonoid.h"
int
sm_islltriv(struct eggboxes * s, int x)
{
	return sm_ese(s, sm_isltriv, x);
}
