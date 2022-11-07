#include <amalgam/amalgam.h>
#include <amalgam/io/att.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

/** @private */
struct all_classes
{
	/* varieties */
	_Bool SF : 1;
	_Bool DA : 1;
	_Bool L : 1;
	_Bool R : 1;
	_Bool B : 1;
	_Bool J : 1;
	_Bool Acom : 1;
	_Bool J1 : 1;
	_Bool Triv : 1;
	/* local */
	_Bool LSF : 1;
	_Bool LDA : 1;
	_Bool LL : 1;
	_Bool LR : 1;
	_Bool LB : 1;
	_Bool LJ : 1;
	_Bool LTT : 1;
	_Bool LT : 1;
	_Bool GD : 1;
	_Bool D : 1;
	_Bool K : 1;
	_Bool N : 1;
	/* tier-local */
	_Bool TLSF : 1;
	_Bool TLDA : 1;
	_Bool TLL : 1;
	_Bool TLR : 1;
	_Bool TLB : 1;
	_Bool TLJ : 1;
	_Bool TLTT : 1;
	_Bool TLT : 1;
	_Bool TGD : 1;
	_Bool TD : 1;
	_Bool TK : 1;
	_Bool TN : 1;
};

static void
invalidate(struct eggbox * b, struct all_classes * r,
           _Bool touch_v, _Bool touch_lv, _Bool touch_tlv)
{
	if (!b || !r) { return; }
	if (b->polyegg)
	{
		/* invalidate EVERYTHING everywhere if not star-free */
		memset(r, 0, sizeof(*r));
		return;
	}
	/* now for more reasonable things: */
	if (b->polyrow || b->nonfull)
	{
		/* invalidate L-trivial and subclasses thereof */
		r->L = r->J = r->Acom = r->J1 = r->Triv = 0;
		r->D = r->TD = r->N = r->TN = 0;
		if (touch_lv)
		{
			r->LL = r->LJ = r->LTT = r->LT = 0;
		}
		if (touch_tlv)
		{
			r->TLL = r->TLJ = r->TLTT = r->TLT = r->TGD = 0;
		}
	}
	if (b->polycol || b->nonfull)
	{
		/* invalidate R-trivial and subclasses thereof */
		r->R = r->J = r->Acom = r->J1 = r->Triv = 0;
		r->K = r->TK = r->N = r->TN = 0;
		if (touch_lv)
		{
			r->LR = r->LJ = r->LTT = r->LT = 0;
		}
		if (touch_tlv)
		{
			r->TLR = r->TLJ = r->TLTT = r->TLT = r->TGD = 0;
		}
	}
	if (b->irregular || b->nonfull)
	{
		/* invalidate Band and subclasses thereof */
		r->B = r->J1 = r->Triv = 0;
		if (touch_lv)
		{
			r->LB = r->LT = 0;
		}
		if (touch_tlv)
		{
			r->TLB = r->TLT = r->TGD = 0;
		}
	}
	if (b->nonfull)
	{
		/* invalidate DA and subclasses thereof */
		/* but subclasses have been handled above */
		r->DA = 0;
		if (touch_lv) { r->LDA = 0; }
		if (touch_tlv) { r->TLDA = 0; }
	}
	if (b->next && touch_lv)
	{
		/* invalidate GD and subclasses */
		r->GD = r->D = r->K = r->N = r->Triv = 0;
	}
	if (b->next && touch_tlv)
	{
		/* invalidate GD and subclasses */
		r->TGD = r->TD = r->TK = r->TN = r->Triv = 0;
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
	struct classifier * c;
	struct eggbox * e;
	struct eggboxes * es;
	struct all_classes r;
	struct uilist u;
	int issl = 0;
	int istsl = 0;
	int lcomm = 0;
	u.next = NULL;
	u.value = 0;
	/* read the rest of stdin so writers don't die */
	while (!feof(stdin)) { fgetc(stdin); }
	if (!m) { return 1; }
	/* keep the connected component containing the initial state */
	fi_trim(m);
	/* determinize */
	s = fi_powerset(m, &u, HAS_FINAL);
	fi_free(m);
	m = s;
	s = NULL;
	/* minimize and trim */
	fi_nerode(m);
	issl = fi_issl(m);
	s = fi_ssg(m);
	fi_project(m);
	fi_nerode(m);
	istsl = fi_issl(m);
	fi_free(m);
	m = s;
	s = NULL;
	/* the main structure: local eggboxes */
	c = sm_classifier(m);
	if (!c) { return EXIT_FAILURE; }
	fi_free(m);
	m = NULL;

	/* initially assume every membership holds */
	memset(&r, (int)((unsigned char)UCHAR_MAX), sizeof(r));
	/* classify based on the main semigroup */
	for (e = c->semigroup; e; e = e->next)
	{
		invalidate(e, &r, 1, 0, 0);
	}
	for (es = c->localsm; es && r.SF; es = es->next)
	{
		for (e = es->box; e; e = e->next)
		{
			invalidate(e, &r, 0, 1, es->box != c->semigroup);
		}
	}
	r.Acom = r.Acom && c->comm;
	lcomm = c->plcomm&&((c->semigroup != c->localsm->box)||c->comm);
	r.LTT  = r.LTT && lcomm;
	r.TLTT = r.TLTT && c->plcomm;

	printf("%12s%4s%4s %4s\n","","V","LV","TLV");
	static char const * const format = "%12s%4c%4c%4c\n";
	printf(format, "Commutative", b(c->comm), b(lcomm), b(c->plcomm));
	printf(format, "SF", b(r.SF), ' ', ' ');
	printf(format, "DA", b(r.DA), b(r.LDA), b(r.TLDA));
	printf(format, "L-trivial", b(r.L), b(r.LL), b(r.TLL));
	printf(format, "R-trivial", b(r.R), b(r.LR), b(r.TLR));
	printf(format, "Band", b(r.B), b(r.LB), b(r.TLB));
	printf(format, "PT", b(r.J), b(r.LJ), b(r.TLJ));
	printf(format, "Acom", b(r.Acom), b(r.LTT), b(r.TLTT));
	printf(format, "Semilattice", b(r.J1), b(r.LT), b(r.TLT));
	printf(format, "1", b(r.Triv), b(r.GD), b(r.TGD));
	printf(format, "D", ' ', b(r.D), b(r.TD));
	printf(format, "K", ' ', b(r.K), b(r.TK));
	printf(format, "F", ' ', b(r.N), b(r.TN));
	printf("\n");
	printf(format, "SL", ' ', b(issl), b(istsl));
	sm_free(c);
	c = NULL;
	return 0;
}
