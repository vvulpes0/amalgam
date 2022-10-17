#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
void
fi_free(struct finsa * m)
{
	size_t i;
	for (i = 0; i < m->count; ++i)
	{
		bx_free(m->graphs[i]);
		m->graphs[i] = NULL;
	}
	free(m->graphs);
	m->graphs = NULL;
	ui_free(m->finals);
	m->finals = NULL;
	free(m);
}
