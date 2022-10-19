#include <amalgam/amalgam.h>
#include <amalgam/io/att.h>
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
		if (p) { printf(":: %p", p); }
		printf("\n");
	}
}

static char
b(int x)
{
	return x ? 'Y' : 'N';
}

int
main(void)
{
	/* struct finsa * m = _mkaut(); */
	struct finsa * m = fi_fromatt(stdin);
	struct finsa * s;
	struct eggbox * e;
	struct eggboxes * boxes;
	struct eggbox * p;
	struct uilist * x;
	struct uilist u;
	size_t i;
	int noid;
	int comm;
	int subcomm;
	int lcom;
	int issl = 0;
	int istsl = 0;
	while (!feof(stdin)) { fgetc(stdin); }
	if (!m) { return 1; }
	u.next = NULL; u.value = 0;
	fi_trim(m);
	for (i = 0; i < m->count; ++i)
	{
		print_matrix(m->graphs[i]);
		printf("\n");
	}
	for (x = m->finals; x; x = x->next ) { printf("%u ", x->value); }
	printf("\n\n");
	s = fi_powerset(m, &u, HAS_FINAL);
	fi_free(m);
	m = s;
	s = NULL;
	for (i = 0; i < m->count; ++i)
	{
		print_matrix(m->graphs[i]);
		printf("\n");
	}
	for (x = m->finals; x; x = x->next ) { printf("%u ", x->value); }
	printf("\n\n");
	fi_nerode(m);
	for (i = 0; i < m->count; ++i)
	{
		print_matrix(m->graphs[i]);
		printf("\n");
	}
	issl = fi_issl(m);
	s = fi_smonoid(m);
	fi_project(m);
	fi_nerode(m);
	istsl = fi_issl(m);
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
	printf("%12s%4s%4s %4s\n","","V","LV","TLV");
	static char const * const format = "%12s%4c%4c%4c\n";
	printf(format, "Commutative", b(comm), b(lcom), b(subcomm));
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
	printf(format, "F", ' ', b(sm_isn(e, noid)), b(sm_istn(e)));
	printf("\n");
	printf(format, "SL", ' ', b(issl), b(istsl));
	e = NULL;
	sm_freelist(boxes);
	boxes = NULL;
	return 0;
}
