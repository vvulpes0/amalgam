#include "finsa.h"

#include "bmatrix.h"
#include "uilist.h"

#include <stdlib.h>

/** @private */
struct bxlist
{
	struct bxlist * next;
	struct bmatrix * matrix;
};

static void
_free_bxlist(struct bxlist * p)
{
	struct bxlist * t;
	while (p)
	{
		t = p->next;
		bx_free(p->matrix);
		free(p);
		p = t;
	}
}

static struct bxlist *
_findmats(struct finsa * m, int *id_reachable)
{
	struct bxlist * o = NULL;
	struct bxlist ** oe = &o;
	struct bxlist * n = NULL;
	struct bxlist * p;
	struct bmatrix * t;
	size_t i;
	int x;
	if (!id_reachable) { id_reachable = &x; }
	if (!m) { return NULL; }
	if (m->count == 0) { return NULL; }
	/* first step: insert the identity */
	o = malloc(sizeof(*o));
	if (!o) { return NULL; }
	o->next = NULL;
	o->matrix = bx_identity(m->graphs[0]->size);
	if (!o->matrix) { _free_bxlist(o); return NULL; }
	oe = &(o->next);
	/* then append new things until we stop generating new things */
	n = o;
	while (n)
	{
		for (i = 0; i < m->count; ++i)
		{
			t = bx_copy(n->matrix);
			if (!t) { _free_bxlist(o); return NULL; }
			if (!bx_mul(t, m->graphs[i]))
			{
				bx_free(t);
				_free_bxlist(o);
				return NULL;
			}
			p = o;
			while (p)
			{
				if (bx_eq(t, p->matrix))
				{
					(*id_reachable) |= (p==o);
					bx_free(t);
					break;
				}
				p = p->next;
			}
			if (!p)
			{
				*oe = malloc(sizeof(**oe));
				if (!*oe)
				{
					bx_free(t);
					_free_bxlist(o);
					return NULL;
				}
				(*oe)->next = NULL;
				(*oe)->matrix = t;
				oe = &((*oe)->next);
			}
		}
		n = n->next;
	}
	return o;
}

static size_t
_locate(struct bmatrix * m, struct bxlist * s)
{
	size_t i;
	for (i = 0; s && !bx_eq(m, s->matrix); ++i, s = s->next) { ; }
	return i;
}

struct finsa *
fi_smonoid(struct finsa * m)
{
	struct bxlist * b;
	struct bxlist * p;
	struct bxlist * q;
	struct finsa * o;
	struct bmatrix * g;
	struct bmatrix * t;
	size_t i;
	size_t j;
	int is_monoid = 0;
	b = _findmats(m, &is_monoid);
	if (!b) { return NULL; }
	o = malloc(sizeof(*o));
	if (!o) { _free_bxlist(b); return NULL; }
	o->finals = NULL;
	o->count = 0;
	for (p = b; p; p = p->next) { ++(o->count); }
	o->graphs = malloc(o->count * sizeof(*(o->graphs)));
	if (!o->graphs)
	{
		_free_bxlist(b);
		free(o);
		return NULL;
	}
	for (i = 0; i < o->count; ++i) { o->graphs[i] = NULL; }
	for (i = 0, p = b; p; ++i, p = p->next)
	{
		o->graphs[i] = malloc(sizeof(*(o->graphs[i])));
		if (!o->graphs[i])
		{
			_free_bxlist(b);
			fi_free(o);
			return NULL;
		}
		g = o->graphs[i];
		g->size = o->count;
		g->vecs = malloc(o->count * sizeof(*(g->vecs)));
		if (!g->vecs)
		{
			free(o->graphs[i]);
			o->graphs[i] = NULL;
			_free_bxlist(b);
			fi_free(o);
			return NULL;
		}
		for (j = 0; j < g->size; ++j) { g->vecs[j] = NULL; }
		for (j = 0, q = b; q; ++j, q = q->next)
		{
			t = bx_copy(q->matrix);
			if (!t)
			{
				bx_free(g);
				o->graphs[i] = NULL;
				_free_bxlist(b);
				fi_free(o);
				return NULL;
			}
			g->vecs[j] = malloc(sizeof(*(g->vecs[j])));
			if (!g->vecs[j] || !bx_mul(t, p->matrix))
			{
				bx_free(t);
				bx_free(g);
				o->graphs[i] = NULL;
				_free_bxlist(b);
				fi_free(o);
				return NULL;
			}
			g->vecs[j]->next = NULL;
			g->vecs[j]->value = _locate(t, b);
			bx_free(t);
		}
	}
	_free_bxlist(b);
	b = NULL;
	if (is_monoid)
	{
		o->finals = malloc(sizeof(*(o->finals)));
		if (!o->finals) { fi_free(o); return NULL; }
		o->finals->next = NULL;
		o->finals->value = 0;
	}
	return o;
}
