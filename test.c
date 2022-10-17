#include "bmatrix.h"
#include "finsa.h"
#include "smonoid.h"
#include "uilist.h"
#include <stdlib.h>
#include <stdio.h>
void
print_matrix(struct bmatrix * m)
{
	struct uilist * p;
	size_t i;
	size_t j;
	if (!m || !m->vecs) { return; }
	for (i = 0; i < m->size; ++i)
	{
		p = m->vecs[i];
		for (j = 0; j < m->size; ++j)
		{
			if (!p) { printf("0 "); continue; }
			if (p->value > j) { printf("0 "); continue; }
			printf("1 ");
			p = p->next;
		}
		printf("\n");
	}
}


static _Bool testm[3][4][4] = {
	{{0,0,1,0},
	 {0,0,0,0},
	 {0,0,1,0},
	 {0,0,1,0},
	},
	{{0,1,0,0},
	 {0,1,0,0},
	 {0,0,0,1},
	 {0,0,0,1},
	},
	{{0,1,0,0},
	 {0,1,0,0},
	 {0,0,1,0},
	 {0,0,1,0},
	},
};
/*
static _Bool testm[3][5][5] = {
	{{0,1,0,0,0},
	 {0,0,0,0,1},
	 {0,0,1,0,0},
	 {0,0,1,0,0},
	 {0,0,0,0,1},
	},
	{{0,0,0,0,1},
	 {0,0,0,1,0},
	 {0,0,0,1,0},
	 {0,0,0,1,0},
	 {0,0,0,0,1},
	},
	{{1,0,0,0,0},
	 {0,1,0,0,0},
	 {0,0,1,0,0},
	 {0,0,0,1,0},
	 {0,0,0,0,1},
	},
};
*/

static struct finsa *
_mkaut(void)
{
	struct finsa * o = malloc(sizeof(*o));
	size_t i;
	size_t j;
	size_t k;
	size_t s;
	struct uilist * t;
	if (!o) { return NULL; }
	o->count = sizeof(testm) / sizeof(testm[0]);
	o->finals = NULL;
	o->graphs = malloc(o->count * sizeof(*(o->graphs)));
	if (!o->graphs) { free(o); return NULL; }
	for (i = 0; i < o->count; ++i) { o->graphs[i] = NULL; }
	for (i = 0; i < o->count; ++i)
	{
		s = sizeof(testm[0])/sizeof(testm[0][0]);
		o->graphs[i] = malloc(sizeof(*(o->graphs[i])));
		if (!o->graphs[i]) { fi_free(o); return NULL; }
		o->graphs[i]->size = s;
		o->graphs[i]->vecs
			= malloc(s * sizeof(*(o->graphs[i]->vecs)));
		for (j = 0; j < s; ++j) { o->graphs[i]->vecs[j] = NULL; }
		for (j = 0; j < s; ++j)
		{
			for (k = s - 1; k + 1 > 0; --k)
			{
				if (!testm[i][j][k]) { continue; }
				t = malloc(sizeof(*t));
				if (!t) { fi_free(o); return NULL; }
				t->next = o->graphs[i]->vecs[j];
				t->value = k;
				o->graphs[i]->vecs[j] = t;
			}
		}
	}
	return o;
}

static char
b(int x)
{
	return x ? 'Y' : 'N';
}

int
main(void)
{
	struct finsa * m = _mkaut();
	struct finsa * s;
	struct eggbox * e;
	struct eggboxes * boxes;
	struct eggbox * p;
	struct uilist * x;
	size_t i;
	int noid;
	int comm;
	int subcomm;
	int lcom;
	if (!m) { return 1; }
	for (i = 0; i < m->count; ++i)
	{
		print_matrix(m->graphs[i]);
		printf("\n");
	}
	s = fi_smonoid(m);
	fi_free(m);
	m = s;
	s = NULL;
	for (i = 0; i < m->count; ++i)
	{
		print_matrix(m->graphs[i]);
		printf("\n");
	}
	boxes = sm_localsm(m, &subcomm);
	if (!boxes) { printf("OY\n"); }
	e = boxes->box;
	p = e;
	while (p)
	{
		printf("<--\n");
		for (size_t r = 0; r < p->rows; ++r)
		{
			for (size_t c = 0; c < p->cols; ++c)
			{
				x = p->eggs[r * p->cols + c];
				while (x)
				{
					printf("%3u", x->value);
					x = x->next;
				}
				printf("|");
			}
			printf("\n");
		}
		printf("-->\n");
		p = p->next;
	}
	noid = !m->finals;
	comm = sm_iscom(m);
	lcom = subcomm && (noid || comm);
	fi_free(m);
	m = NULL;
	printf("              V  LV  TLV\n");
	static char const * const format
		= "%12s  %c   %c    %c\n";
	printf(format, "Commutative", b(comm), b(subcomm), b(lcom));
	printf(format, "SF", b(sm_issf(e)), ' ', ' ');
	printf(format, "DA", b(sm_isda(e)),
	       b(sm_islda(boxes, noid)), b(sm_istlda(boxes)));
	printf(format, "L-trivial", b(sm_isltriv(e)),
	       b(sm_islltriv(boxes, noid)), b(sm_istlltriv(boxes)));
	printf(format, "R-trivial", b(sm_isrtriv(e)),
	       b(sm_islrtriv(boxes, noid)), b(sm_istlrtriv(boxes)));
	printf(format, "Band", b(sm_isband(e)),
	       b(sm_ese(boxes, sm_isband, noid)),
	       b(sm_ese(boxes, sm_isband, 1)));
	printf(format, "PT", b(sm_ispt(e)),
	       b(sm_islj(boxes, noid)), b(sm_istlj(boxes)));
	printf(format, "Acom", b(comm && sm_issf(e)),
	       b(lcom && sm_issf(e)),
	       b(subcomm && sm_issf(e)));
	printf(format, "Semilattice", b(sm_issemilat(e)),
	       b(sm_islt(boxes, noid)), b(sm_istlt(boxes)));
	printf(format, "1", b(sm_istriv(e)),
	       b(sm_isgd(e, noid)), b(sm_istgd(e)));
	printf(format, "D", ' ', b(sm_isd(e, noid)), b(sm_istd(e)));
	printf(format, "K", ' ', b(sm_isk(e, noid)), b(sm_istk(e)));
	printf(format, "F", ' ', b(sm_isf(e, noid)), b(sm_istf(e)));
	e = NULL;
	sm_freelist(boxes);
	boxes = NULL;
	return 0;
}
