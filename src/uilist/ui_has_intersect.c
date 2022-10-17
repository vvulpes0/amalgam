#include "uilist.h"
#include <stdlib.h>
int
ui_has_intersect(struct uilist * p, struct uilist * q)
{
	while (p && q)
	{
		if (p->value < q->value)
		{
			p = p->next;
			continue;
		}
		if (p->value > q->value)
		{
			q = q->next;
			continue;
		}
		return 1;
	}
	return 0;
}
