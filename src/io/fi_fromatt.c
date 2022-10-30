#include "io/att.h"
#include "finsa.h"
#include "bmatrix.h"
#include "uilist.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/** @private */
struct translist
{
	struct translist * next;
	unsigned int src;
	unsigned int dst;
	unsigned int sym;
};
/** @private */
struct strlist
{
	struct strlist * next;
	char * str;
};

static void
_freestrlist(struct strlist * p)
{
	struct strlist * t;
	while (p)
	{
		t = p->next;
		free(p->str);
		free(p);
		p = t;
	}
}

static void
_freetranslist(struct translist * p)
{
	struct translist * t;
	while (p)
	{
		t = p->next;
		free(p);
		p = t;
	}
}

static size_t
_nf(char const * s)
{
	size_t i = 0;
	if (!s) { return 0; }
	while (*s)
	{
		while (*s &&  isspace(*s)) ++s;
		if (!*s) { break; }
		while (*s && !isspace(*s)) ++s;
		++i;
	}
	return i;
}

static char const *
_field(char const * restrict s, int i, int * restrict len)
{
	if (!s | !len) { return NULL; }
	*len = 0;
	while (i && *s)
	{
		while (*s &&  isspace(*s)) ++s;
		*len = 0;
		if (!*s) { return s; }
		while (*s && !isspace(*s)) { ++*len; ++s; }
		--i;
	}
	return s - *len;
}

static unsigned int
_find(struct strlist ** a, char const * field, int len)
{
	struct strlist * p;
	struct strlist * b = NULL;
	unsigned int i = 0U;
	if (!a) { return 0U; }
	p = *a;
	while (p)
	{
		if (!strncmp(field, p->str, len) && !p->str[len])
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
	p->str = malloc((len + 1) * sizeof(*(p->str)));
	if (!p->str) { free(p); }
	strncpy(p->str, field, len);
	p->str[len] = '\0';
	if (b) { b->next = p; return i; }
	*a = p;
	return i;
}

static struct finsa *
_mkaut(struct translist * trans, int syms, int states)
{
	struct finsa * o = malloc(sizeof(*o));
	struct bmatrix * g;
	struct uilist ** v;
	size_t i;
	size_t j;
	if (!o) { return NULL; }
	o->count = syms;
	o->graphs = malloc(syms * sizeof(*(o->graphs)));
	if (!o->graphs) { free(o); return NULL; }
	for (i = 0; i < syms; ++i) { o->graphs[i] = NULL; }
	for (i = 0; i < syms; ++i)
	{
		o->graphs[i] = malloc(sizeof(*(o->graphs[i])));
		if (!o->graphs[i]) { fi_free(o); return NULL; }
		o->graphs[i]->size = states;
		g = o->graphs[i];
		g->vecs = malloc(states * sizeof(*(g->vecs)));
		if (!g->vecs) { g->size = 0; fi_free(o); return NULL; }
		for (j = 0; j < states; ++j)
		{
			g->vecs[j] = NULL;
		}
	}
	while (trans)
	{
		v = o->graphs[trans->sym]->vecs;
		v[trans->src] = ui_insert(v[trans->src], trans->dst);
		trans = trans->next;
	}
	return o;
}

#define NUMCHARS 2048
struct finsa *
fi_fromatt(FILE * f)
{
	struct strlist * states = NULL;
	struct strlist * symbols = NULL;
	struct uilist * finals = NULL;
	struct translist * trans = NULL;
	struct translist ** transe = &trans;
	struct finsa * o;
	char * linebuf = malloc(NUMCHARS);
	char const * v;
	size_t nf;
	int nc = 0;
	int ns = 0;
	int s = 0;
	int x;
	if (!linebuf) { return NULL; }
	/* 0 is reserved for emptiness. let's ensure it is first */
	_find(&symbols, "0", 1);
	if (!symbols) { return NULL; }
	while (fgets(linebuf, NUMCHARS, f))
	{
		nf = _nf(linebuf);
		switch (nf)
		{
		case 0:
			/* blank line */
			continue;
		case 1:
		case 2:
			/* accepting state */
			v = _field(linebuf, 1, &s);
			if (!s) { continue; }
			x = _find(&states, v, s);
			finals = ui_insert(finals, x);
			ns = x > ns ? x : ns;
			break;
		case 3:
		case 4:
		case 5:
			/* arc for acceptor or transducer */
			*transe = malloc(sizeof(**transe));
			(*transe)->next = NULL;
			if (!*transe) { continue; }
			v = _field(linebuf, 1, &s);
			if (!s) { free(*transe); continue; }
			x = _find(&states, v, s);
			ns = x > ns ? x : ns;
			(*transe)->src = x;
			v = _field(v, 2, &s);
			if (!s) { free(*transe); continue; }
			x = _find(&states, v, s);
			ns = x > ns ? x : ns;
			(*transe)->dst = x;
			v = _field(v, 2, &s);
			if (!s) { free(*transe); continue; }
			x = _find(&symbols, v, s);
			(*transe)->sym = x;
			nc = x > nc ? x : nc;
			transe = &((*transe)->next);
			break;
		default:
			fprintf(stderr, "too many fields: %s\n", linebuf);
			continue;
		}
	}
	free(linebuf);
	_freestrlist(states);
	_freestrlist(symbols);
	o = _mkaut(trans, nc + 1, ns + 1);
	if (o)
	{
		o->finals = finals;
		finals = NULL;
		fi_rmeps(o, 0);
		if (!o->count && o->finals)
		{
			o->count = 1;
			o->graphs[0] = bx_identity(1);
		}
		if (!o->graphs[0] || (!o->count && !o->finals))
		{
			o->count = 0;
			fi_free(o);
			o = NULL;
		}
	}
	_freetranslist(trans);
	ui_free(finals);
	return o;
}
