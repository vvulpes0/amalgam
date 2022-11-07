#include "smonoid.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <stdlib.h>

/** @private */
struct ebvec
{
	struct ebvec * next;
	struct uilist * elements;
};

static void
free_ebvec(struct ebvec * p)
{
	struct ebvec * t;
	while (p)
	{
		t = p->next;
		ui_free(p->elements);
		p->elements = NULL;
		free(p);
		p = t;
	}
}

static struct ebvec *
cluster(struct bmatrix * r)
{
	struct ebvec * o = NULL;
	struct ebvec * ep;
	struct uilist * t;
	size_t i;
	if (!r || !r->vecs) { return NULL; }
	for (i = r->size; i--;)
	{
		ep = o;
		t = malloc(sizeof(*t));
		if (!t) { free_ebvec(o); return NULL; }
		t->next = NULL;
		t->value = i;
		while (ep)
		{
			if (ui_eq(r->vecs[i],
			          r->vecs[ep->elements->value]))
			{
				t->next = ep->elements;
				ep->elements = t;
				break;
			}
			ep = ep->next;
		}
		if (!ep)
		{
			ep = malloc(sizeof(*ep));
			if (!ep) { free_ebvec(o); return NULL; }
			ep->next = o;
			ep->elements = t;
			o = ep;
		}
	}
	return o;
}

static size_t
partition_overlap(struct uilist * p, struct ebvec ** v, _Bool * polyegg)
{
	struct ebvec * a = NULL;
	struct ebvec ** ae = &a;
	struct ebvec * b = NULL;
	struct ebvec ** be = &b;
	struct ebvec * t = NULL;
	struct uilist * x = NULL;
	struct uilist * y = NULL;
	size_t i = 0;
	size_t num_elts = 0;
	_Bool saw_polyegg = 0;
	while (v && (*v))
	{
		t = (*v)->next;
		(*v)->next = NULL;
		x = p;
		y = (*v)->elements;
		num_elts = 0;
		while ((!num_elts || (num_elts < 2 && !saw_polyegg))
		       && x && y)
		{
			if (x->value < y->value)
			{
				x = x->next;
				continue;
			}
			if (x->value > y->value)
			{
				y = y->next;
				continue;
			}
			++num_elts;
			x = x->next;
			y = y->next;
		}
		if (num_elts > 1) { saw_polyegg = 1; }
		if (num_elts)
		{
			++i;
			(*ae) = (*v);
			ae = &((*v)->next);
		}
		else
		{
			(*be) = (*v);
			be = &((*v)->next);
		}
		(*v) = t;
	}
	(*ae) = b;
	(*v) = a;
	if (polyegg) { *polyegg = saw_polyegg; }
	return i;
}

static void
check_regularity(struct finsa * m, struct uilist * p, struct eggbox * b)
{
	unsigned int i;
	if (!m || !m->graphs || !p || !b) { return; }
	while (p && (b->irregular || !b->nonfull))
	{
		i = p->value;
		if (!m->graphs[i] || !m->graphs[i]->vecs
		    || !m->graphs[i]->vecs[i])
		{
			p = p->next;
			continue;
		}
		/* are we idempotent? */
		if (m->graphs[i]->vecs[i]->value == i)
		{
			b->irregular = 0;
		}
		else
		{
			b->nonfull = 1;
		}
		p = p->next;
	}
}

struct eggbox *
sm_eggbox(struct finsa * m)
{
	struct bmatrix * mt;
	struct ebvec * rows;
	struct ebvec * cols;
	struct ebvec * t;
	struct eggbox * o = NULL;
	struct eggbox * b;
	size_t i;
	size_t nrows;
	size_t ncols;
	_Bool flag;
	if (!m || !m->graphs || !m->count) { return NULL; }
	mt = sm_rrel(m);
	if (!mt) { return NULL; }
	rows = cluster(mt);
	bx_free(mt);
	if (!rows) { return NULL; }
	mt = sm_lrel(m);
	cols = cluster(mt);
	bx_free(mt);
	if (!cols) { return NULL; }
	mt = NULL;
	while (rows)
	{
		b = calloc(1,sizeof(*b));
		if (!b)
		{
			free_ebvec(rows);
			free_ebvec(cols);
			sm_freebox(o);
			return NULL;
		}
		b->next = o;
		flag = 0;
		ncols = partition_overlap(rows->elements, &cols, &flag);
		nrows = partition_overlap(cols->elements, &rows, NULL);
		if (flag) { b->polyegg = 1; }
		if (nrows > 1) { b->polyrow = 1; }
		if (ncols > 1) { b->polycol = 1; }
		b->irregular = 1;
		for (i = 0; i < nrows; ++i)
		{
			t = rows;
			check_regularity(m, rows->elements, b);
			ui_free(rows->elements);
			rows->elements = NULL;
			rows = rows->next;
			free(t);
		}
		if (b->irregular && b->nonfull) { b->nonfull = 0; }
		for (i = 0; i < ncols; ++i)
		{
			t = cols;
			ui_free(cols->elements);
			cols->elements = NULL;
			cols = cols->next;
			free(t);
		}
		o = b;
	}
	free_ebvec(cols);
	return o;
}
