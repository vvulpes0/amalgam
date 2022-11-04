#include <amalgam/amalgam.h>
#include <amalgam/io/att.h>
#include <stdlib.h>
#include <stdio.h>

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
	struct uilist u;
	int comm;
	int subcomm;
	int lcom;
	int issl = 0;
	int istsl = 0;
	/* read the rest of stdin so writers don't die */
	while (!feof(stdin)) { fgetc(stdin); }
	if (!m) { return 1; }
	u.next = NULL; u.value = 0;
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
	boxes = sm_localsm(m, &subcomm);
	if (!boxes) { return EXIT_FAILURE; }
	e = sm_eggbox(m);
	if (!e) { sm_freelist(boxes); return EXIT_FAILURE; }
	comm = sm_iscom(m);
	lcom = subcomm && (!m->finals || comm);
	fi_free(m);
	m = NULL;
	printf("%12s%4s%4s %4s\n","","V","LV","TLV");
	static char const * const format = "%12s%4c%4c%4c\n";
	printf(format, "Commutative", b(comm), b(lcom), b(subcomm));
	printf(format, "SF", b(sm_issf(e)), ' ', ' ');
	printf(format, "DA", b(sm_isda(e)),
	       b(sm_islda(boxes, 0)), b(sm_istlda(boxes)));
	printf(format, "L-trivial", b(sm_isltriv(e)),
	       b(sm_islltriv(boxes, 0)), b(sm_istlltriv(boxes)));
	printf(format, "R-trivial", b(sm_isrtriv(e)),
	       b(sm_islrtriv(boxes, 0)), b(sm_istlrtriv(boxes)));
	printf(format, "Band", b(sm_isband(e)),
	       b(sm_ese(boxes, sm_isband, 0)),
	       b(sm_ese(boxes, sm_isband, 1)));
	printf(format, "PT", b(sm_ispt(e)),
	       b(sm_islj(boxes, 0)), b(sm_istlj(boxes)));
	printf(format, "Acom", b(comm && sm_issf(e)),
	       b(lcom && sm_issf(e)),
	       b(subcomm && sm_issf(e)));
	printf(format, "Semilattice", b(sm_issemilat(e)),
	       b(sm_islt(boxes, 0)), b(sm_istlt(boxes)));
	printf(format, "1", b(sm_istriv(e)),
	       b(sm_isgd(e, 0)), b(sm_istgd(e)));
	printf(format, "D", ' ', b(sm_isd(e, 0)), b(sm_istd(e)));
	printf(format, "K", ' ', b(sm_isk(e, 0)), b(sm_istk(e)));
	printf(format, "F", ' ', b(sm_isn(e, 0)), b(sm_istn(e)));
	printf("\n");
	printf(format, "SL", ' ', b(issl), b(istsl));
	sm_free(e);
	sm_freelist(boxes);
	boxes = NULL;
	return 0;
}
