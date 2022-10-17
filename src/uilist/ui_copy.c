#include "uilist.h"
#include <stdlib.h>
struct uilist *
ui_copy(struct uilist * p)
{
	struct uilist * q;
	struct uilist ** r;
	if (!p) { return NULL; }
	q = malloc(sizeof(*q));
	if (!q) { return NULL; }
	q->value = p->value;
	q->next = NULL;
	r = &(q->next);
	p = p->next;
	while (p)
	{
		*r = malloc(sizeof(**r));
		if (!*r) { free(q); return NULL; }
		(*r)->value = p->value;
		(*r)->next = NULL;
		r = &((*r)->next);
		p = p->next;
	}
	return q;
}
