#include "smonoid.h"
#include <stdlib.h>
void
sm_freebox(struct eggbox * p)
{
	struct eggbox * t;
	while (p) { t = p->next; free(p); p = t; }
}
