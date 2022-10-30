#include <amalgam/amalgam.h>
#include <amalgam/io/att.h>
#include <amalgam/io/dot.h>
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
	char buf[1024];
	struct finsa * m = fi_fromatt(stdin);
	struct finsa * s;
	struct eggbox * box;
	FILE * f;
	struct uilist u;
	size_t i;
	u.next = NULL;
	u.value = 0;
	if (!m) { return EXIT_FAILURE; }
	fi_trim(m);
	s = fi_powerset(m, &u, HAS_FINAL);
	fi_free(m);
	m = s;
	s = NULL;
	fi_nerode(m);
	s = fi_ssg(m);
	fi_free(m);
	m = s;
	s = NULL;
	box = sm_eggbox(m);
	fi_free(m);
	m = NULL;
	sm_todot(stdout, box);
	sm_free(box);
	return EXIT_SUCCESS;
}
