#include "smonoid.h"
#include "finsa.h"
#include <stdlib.h>
struct classifier *
sm_classifier(struct finsa * m)
{
	struct classifier * c = NULL;
	struct eggboxes ** pl = NULL;
	int plcomm = 1;
	if (!m) { return NULL; }
	c = calloc(1,sizeof(*c));
	if (!c) { return NULL; }
	c->semigroup = sm_eggbox(m);
	pl = &(c->localsm);
	if (m->finals)
	{
		c->localsm = malloc(sizeof(*(c->localsm)));
		if (!c->localsm) { sm_freebox(c->semigroup); return NULL; }
		c->localsm->next = NULL;
		c->localsm->box = c->semigroup;
		pl = &(c->localsm->next);
	}
	c->comm = sm_iscom(m);
	*pl = sm_localsm(m, &plcomm);
	c->plcomm = !!plcomm;
	return c;
}
