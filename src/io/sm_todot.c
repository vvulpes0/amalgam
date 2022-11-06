#include "smonoid.h"
#include "uilist.h"
#include <stdio.h>
int
_printlist(FILE * f, struct uilist * p)
{
	int nc = 0;
	for (; p; p = p->next)
	{
		nc += fprintf(f, "%u", p->value);
		if (!p->next) { continue; }
		nc += fprintf(f, ", ");
	}
	return nc;
}

int
sm_todot(FILE * f, struct eggbox * b)
{
	int nc = 0;
	size_t x;
	size_t i;
	size_t j;
	size_t k;
	nc += fprintf(f, "digraph {\n");
	nc += fprintf(f, "\tnode[shape=plaintext,margin=0];\n");
	for (x = 0; b; b = b->next, ++x)
	{
		nc += fprintf(f, "%zu [label=<", x);
		if (b->polyeggs)
		{
			nc += fprintf(f, "<TABLE ");
			nc += fprintf(f, "BORDER=\"1\">");
			nc += fprintf(f, "<TR><TD BORDER=\"0\">");
		}
		nc += fprintf(f, "<TABLE BORDER=\"0\" ");
		nc += fprintf(f, "CELLBORDER=\"1\" ");
		nc += fprintf(f, "CELLSPACING=\"0\">\n");
		for (i = 0; i < b->rows; ++i)
		{
			nc += fprintf(f, "\t<TR>\n");
			for (j = 0; j < b->cols; ++j)
			{
				k = i * b->cols + j;
				nc += fprintf(f, "\t\t<TD");
				if (b->groups[k])
				{
					nc += fprintf(f, " BGCOLOR=\"");
					nc += fprintf(f, "#D1D1D1\"");
				}
				nc += fprintf(f,">");
				if (b->has_id)
				{
					nc += fprintf(f, "\342\212\233");
				}
				else if (b->groups[k])
				{
					nc += fprintf(f, "\342\210\227");
				}
				nc += fprintf(f, "</TD>\n");
			}
			nc += fprintf(f, "\t</TR>\n");
		}
		nc += fprintf(f, "</TABLE>");
		if (b->polyeggs)
		{
			nc += fprintf(f, "</TD></TR>");
			nc += fprintf(f, "</TABLE>");
		}
		nc += fprintf(f, ">];\n");
	}
	return nc + fprintf(f, "}\n");
}
