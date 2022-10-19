#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdio.h>
int
fi_todot(FILE * f, struct finsa * m)
{
	struct uilist * p;
	size_t c;
	size_t i;
	size_t j;
	size_t has_touched;
	int nc = 0;
	if (!m || !m->graphs) { return fprintf(f, "digraph {}\n"); }
	if (!m->graphs[0]) { return fprintf(f, "digraph {}\n"); }
	if (!m->graphs[0]->size) { return fprintf(f, "digraph {}\n"); }
	nc += fprintf(f, "digraph {\n");
	nc += fprintf(f, "_ [style=invis,width=0,height=0,label=\"\"];\n");
	/* note the existence of final states */
	for (p = m->finals; p; p = p->next)
	{
		nc += fprintf(f, "\t%u[peripheries=\"2\"];\n", p->value);
	}
	/* mark initial state */
	fprintf(f, "_ -> 0;\n");
	/* for each state pair */
	for (i = 0; i < m->graphs[0]->size; ++i)
	{
		for (j = 0; j < m->graphs[0]->size; ++j)
		{
			has_touched = 0;
			for (c = 0; c < m->count; ++c)
			{
				if (!m->graphs[c]) { continue; }
				if (!m->graphs[c]->vecs) { continue; }
				if (ui_find(m->graphs[c]->vecs[i], j))
				{
					++has_touched;
				}
			}
			if (!has_touched) { continue; }
			nc += fprintf(f, "\t%zu -> %zu [label=\"", i, j);
			for (c = 0; c < m->count; ++c)
			{
				if (!m->graphs[c]) { continue; }
				if (!m->graphs[c]->vecs) { continue; }
				if (ui_find(m->graphs[c]->vecs[i], j))
				{
					nc += fprintf(f, "%zu", c);
					--has_touched;
					if (has_touched)
					{
						nc += fprintf(f, ",");
					}
				}
			}
			nc += fprintf(f, "\"];");
		}
	}
	return nc + fprintf(f, "}\n");
}
