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

/*
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
*/
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

int
main(void)
{
	struct finsa * m = _mkaut();
	struct finsa * s;
	struct eggbox * e;
	struct eggbox * p;
	struct uilist * x;
	size_t i;
	int noid;
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
	e = sm_eggbox(m);
	if (!e) { printf("OY\n"); }
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
	fi_free(m);
	m = NULL;
	printf("         SF? %c\n", sm_issf(e) ? 'Y' : 'N');
	printf("         DA? %c\n", sm_isda(e) ? 'Y' : 'N');
	printf("  L-trivial? %c\n", sm_isltriv(e) ? 'Y' : 'N');
	printf("  R-trivial? %c\n", sm_isrtriv(e) ? 'Y' : 'N');
	printf("         PT? %c\n", sm_ispt(e) ? 'Y' : 'N');
	printf("       Band? %c\n", sm_isband(e) ? 'Y' : 'N');
	printf("Semilattice? %c\n", sm_issemilat(e) ? 'Y' : 'N');
	printf("          1? %c\n", sm_istriv(e) ? 'Y' : 'N');
	printf("        TGD? %c\n", sm_istgd(e) ? 'Y' : 'N');
	printf("         TD? %c\n", sm_istd(e) ? 'Y' : 'N');
	printf("         TK? %c\n", sm_istk(e) ? 'Y' : 'N');
	printf("         TF? %c\n", sm_istf(e) ? 'Y' : 'N');
	printf("         GD? %c\n", sm_isgd(e,noid) ? 'Y' : 'N');
	printf("          D? %c\n", sm_isd(e,noid) ? 'Y' : 'N');
	printf("          K? %c\n", sm_isk(e,noid) ? 'Y' : 'N');
	printf("          F? %c\n", sm_isf(e,noid) ? 'Y' : 'N');
	sm_free(e);
	return 0;
}
