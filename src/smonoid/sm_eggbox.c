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
	for (i = r->size - 1; i + 1 > 0; --i)
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
partition_overlap(struct uilist * p, struct ebvec ** v)
{
	size_t i = 0;
	struct ebvec * a = NULL;
	struct ebvec ** ae = &a;
	struct ebvec * b = NULL;
	struct ebvec ** be = &b;
	struct ebvec * t = NULL;
	while (v && (*v))
	{
		t = (*v)->next;
		(*v)->next = NULL;
		if (ui_has_intersect(p, (*v)->elements))
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
	return i;
}

static void
fill_box(struct eggbox * e, struct ebvec * rows, struct ebvec * cols,
         struct finsa * m)
{
	struct ebvec * ep;
	struct uilist * p;
	struct uilist * t;
	struct uilist * z;
	size_t c;
	size_t r;
	size_t s;
	if (!e || !rows || !cols || !m) { return; }
	for (r = 0; r < e->rows; ++r)
	{
		while (rows->elements)
		{
			t = rows->elements;
			rows->elements = t->next;
			t->next = NULL;
			ep = cols;
			c = 0;
			while (!ui_has_intersect(t, ep->elements)
			       && c < e->cols)
			{
				ep = ep->next;
				++c;
			}
			s = r * e->cols + c;
			z = m->graphs[t->value]->vecs[t->value];
			while (z && z->value < t->value)
			{
				z = z->next;
			}
			if (z && z->value == t->value)
			{
				e->groups[s] |= 1;
			}
			if (e->eggs[s])
			{
				p = e->eggs[s];
				while (p->next)
				{
					p = p->next;
				}
				p->next = t;
			}
			else
			{
				e->eggs[s] = t;
			}
		}
		rows = rows->next;
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
	struct eggbox * e;
	size_t i;
	size_t s;
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
		e = malloc(sizeof(*e));
		if (!e)
		{
			free_ebvec(rows);
			free_ebvec(cols);
			sm_free(o);
			return NULL;
		}
		e->next = o;
		e->cols = partition_overlap(rows->elements, &cols);
		e->rows = partition_overlap(cols->elements, &rows);
		s = e->cols * e->rows;
		e->eggs = malloc(s * sizeof(*(e->eggs)));
		if (!e->eggs)
		{
			free(e);
			free_ebvec(rows);
			free_ebvec(cols);
			sm_free(o);
			return NULL;
		}
		e->groups = malloc(s * sizeof(*(e->groups)));
		if (!e->groups)
		{
			free(e->eggs);
			free(e);
			free_ebvec(rows);
			free_ebvec(cols);
			sm_free(o);
			return NULL;
		}
		for (i = 0; i < s; ++i) { e->eggs[i] = NULL; }
		fill_box(e, rows, cols, m);
		for (i = 0; i < e->rows; ++i)
		{
			t = rows;
			ui_free(rows->elements);
			rows->elements = NULL;
			rows = rows->next;
			free(t);
		}
		for (i = 0; i < e->cols; ++i)
		{
			t = cols;
			ui_free(cols->elements);
			cols->elements = NULL;
			cols = cols->next;
			free(t);
		}
		o = e;
	}
	free_ebvec(cols);
	return o;
}
