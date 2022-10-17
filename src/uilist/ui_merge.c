#include "uilist.h"
#include <stdlib.h>

struct uilist *
ui_merge(struct uilist * p, struct uilist * q)
{
	struct uilist * r;
	struct uilist * t;
	if (!q) { return p; }
	if (!p) { return ui_copy(q); }
	r = p;
	while (q)
	{
		if (q->value == r->value)
		{
			q = q->next;
			continue;
		}
		if (q->value < r->value)
		{
			t = malloc(sizeof(*t));
			t->value = r->value;
			t->next = r->next;
			r->value = q->value;
			r->next = t;
			r = t;
			q = q->next;
			continue;
		}
		if (!r->next)
		{
			r->next = ui_copy(q);
			break;
		}
		r = r->next;
	}
	return p;
}
