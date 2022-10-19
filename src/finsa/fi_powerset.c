#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>
/** @private */
struct llui
{
	struct llui * next;
	struct uilist * list;
};

static void
_freellui(struct llui * v)
{
	struct llui * t;
	while (v)
	{
		t = v->next;
		ui_free(v->list);
		free(v);
		v = t;
	}
}

static unsigned int
_find(struct llui ** a, struct uilist * field)
{
	struct llui * p;
	struct llui * b = NULL;
	unsigned int i = 0U;
	if (!a) { return 0U; }
	p = *a;
	while (p)
	{
		if (ui_eq(field, p->list))
		{
			return i;
		}
		b = p;
		p = p->next;
		++i;
	}
	/* if we haven't returned, it wasn't in there! */
	/* b still holds the last valid element, if any */
	p = malloc(sizeof(*p));
	if (!p) { return 0U; }
	p->next = NULL;
	p->list = ui_copy(field);
	if (b) { b->next = p; return i; }
	*a = p;
	return i;
}

static int
_finality(enum finality z, struct uilist * v, struct uilist * finals)
{
	switch (z)
	{
	case HAS_FINAL:
		return ui_has_intersect(v, finals);
	case POLY:
		return v && v->next;
	default:
		/* what is this */
		return 0;
	}
}

struct finsa *
fi_powerset(struct finsa * f, struct uilist * v, enum finality z)
{
	struct llui * states = malloc(sizeof(*states));
	struct llui * p = NULL;
	struct uilist * finals = NULL;
	struct uilist * r = NULL;
	struct finsa * o = NULL;
	size_t i;
	size_t ns;
	unsigned int n;

	/* start off in all states of v */
	if (!states) { return NULL; }
	states->next = NULL;
	states->list = ui_copy(v);
	if (!states->list && v) { free(states); return NULL; }
	if (_finality(z, v, f->finals))
	{
		finals = ui_insert(finals, 0);
	}

	/* figure out what other states exist, and how many */
	for (p = states, ns = 0; p; p = p->next, ++ns)
	{
		for (i = 0; i < f->count; ++i)
		{
			r = bx_vmmul(p->list, f->graphs[i]);
			n = _find(&states, r);
			if (_finality(z, r, f->finals))
			{
				finals = ui_insert(finals, n);
			}
			ui_free(r);
		}
	}

	o = malloc(sizeof(*o));
	if (!o) { _freellui(states); ui_free(finals); return NULL; }
	o->count = 0;
	o->finals = NULL;
	o->graphs = malloc(f->count * sizeof(*(o->graphs)));
	if (!o->graphs)
	{
		_freellui(states);
		ui_free(finals);
		free(o);
		return NULL;
	}
	o->finals = finals;
	for (o->count = 0; o->count < f->count; ++(o->count))
	{
		o->graphs[o->count]
			= malloc(sizeof(*(o->graphs[o->count])));
		if (!o->graphs[o->count])
		{
			_freellui(states);
			fi_free(o);
			return NULL;
		}
		o->graphs[o->count]->size = 0;
		o->graphs[o->count]->vecs
			= malloc(ns
			         * sizeof(*(o->graphs[o->count]->vecs)));
		for (o->graphs[o->count]->size = 0, p = states;
		     o->graphs[o->count]->size < ns;
		     ++(o->graphs[o->count]->size), p = p->next)
		{
			i = o->graphs[o->count]->size;
			r = bx_vmmul(p->list, f->graphs[o->count]);
			o->graphs[o->count]->vecs[i]
				= ui_insert(NULL, _find(&states, r));
			ui_free(r);
		}
	}
	_freellui(states);
	return o;
}
