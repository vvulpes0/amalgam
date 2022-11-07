#include "smonoid.h"
#include "uilist.h"
int
sm_istriv(struct classifier * b)
{
	struct eggbox * p;
	if (!b || !b->localsm) { return 1; }
	/* account for languages like "nonempty"
	 * whose semigroups have but a single element
	 * but whose monoids are doubleton.
	 * "Trivial" in this case means "trivial monoid"
	 */
	if (b->semigroup != b->localsm->box) { return 0; }
	p = b->semigroup;
	return !(p->polyegg || p->polyrow || p->polycol || p->next);
}
