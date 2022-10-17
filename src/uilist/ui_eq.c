#include "uilist.h"
int
ui_eq(struct uilist * a, struct uilist * b)
{
	while (a && b)
	{
		if (a->value != b->value) { return 0; }
		a = a->next;
		b = b->next;
	}
	return !(a || b);
}
