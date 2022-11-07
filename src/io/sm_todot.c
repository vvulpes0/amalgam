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
	nc += fprintf(f, "digraph {\n");
	nc += fprintf(f, "\tnode[shape=plaintext,margin=0];\n");
	nc += fprintf(f, "0 [label=<");
	nc += fprintf(f, "<TABLE BORDER=\"0\" CELLBORDER=\"1\"");
	nc += fprintf(f, " CELLSPACING=\"0\">\n");
	for (x = 0; b; b = b->next, ++x)
	{
		nc += fprintf(f, "\t<TR>");
		nc += fprintf(f, "<TD SIDES=\"TBL\">%c</TD>",
		              b->polyegg   ? 'E' : ' ');
		nc += fprintf(f, "<TD SIDES=\"TB\">%c</TD>",
		              b->polyrow   ? 'R' : ' ');
		nc += fprintf(f, "<TD SIDES=\"TB\">%c</TD>",
		              b->polycol   ? 'C' : ' ');
		nc += fprintf(f, "<TD SIDES=\"TB\">%c</TD>",
		              b->irregular ? 'I' : ' ');
		nc += fprintf(f, "<TD SIDES=\"TBR\">%c</TD>",
		              b->nonfull   ? 'N' : ' ');
		nc += fprintf(f, "</TR>\n");
	}
	nc += fprintf(f, "</TABLE>");
	nc += fprintf(f, ">];\n");
	return nc + fprintf(f, "}\n");
}
