#include "uilist.h"
#include <stdlib.h>
void
ui_free(struct uilist * p)
{
	struct uilist * t;
	while (p)
	{
		t = p->next;
		free(p);
		p = t;
	}
}
