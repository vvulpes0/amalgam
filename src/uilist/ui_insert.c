#include "uilist.h"
#include <stdlib.h>
struct uilist *
ui_insert(struct uilist * v, unsigned int x)
{
	struct uilist * b = NULL;
	struct uilist * p;
	struct uilist * t;
	p = v;
	while (p && p->value < x)
	{
		b = p;
		p = p->next;
	}
	if (p && p->value == x) { return v; } /* already there */
	t = malloc(sizeof(*t));
	if (!t) { return v; }
	t->value = x;
	t->next = p;
	if (b) { b->next = t; return v; }
	return t;
}
