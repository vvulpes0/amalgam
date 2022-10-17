#include "smonoid.h"
int
sm_istlltriv(struct eggboxes * s)
{
	return sm_ese(s, sm_isltriv, 1);
}
