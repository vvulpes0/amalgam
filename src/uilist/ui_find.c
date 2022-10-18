#include "uilist.h"
#include <stddef.h>
struct uilist *
ui_find(struct uilist * a, unsigned int x)
{
	for (; a; a = a->next)
	{
		if (a->value == x) { return a; }
	}
	return NULL;
}
