#include <amalgam/amalgam.h>
#include <amalgam/io/att.h>
#include <amalgam/io/dot.h>
#include <stdlib.h>
#include <stdio.h>
int
main(void)
{
	struct finsa * m = fi_fromatt(stdin);
	struct finsa * s;
	struct eggbox * box;
	struct uilist u;
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
	sm_freebox(box);
	return EXIT_SUCCESS;
}
