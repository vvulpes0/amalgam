#include "uilist.h"
#include <stdlib.h>
struct uilist *
ui_intersect(struct uilist * p, struct uilist * q)
{
	struct uilist * in = NULL;
	struct uilist * out = NULL;
	struct uilist ** inp = &(in);
	struct uilist ** outp = &(out);
	while (p && q)
	{
		if (p->value < q->value)
		{
			*outp = p;
			p = p->next;
			(*outp)->next = NULL;
			outp = &((*outp)->next);
			continue;
		}
		if (p->value > q->value)
		{
			q = q->next;
			continue;
		}
		*inp = p;
		p = p->next;
		(*inp)->next = NULL;
		inp = &((*inp)->next);
		continue;
	}
	*outp = p;
	if (out) { ui_free(out); }
	return in;
}
