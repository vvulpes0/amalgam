#include <limits.h>
#include "bmatrix.h"
size_t
bx_data_size(struct bmatrix * m)
{
	if (!m || !m->data) { return 0; }
	return ((m->rows * m->cols) + CHAR_BIT - 1) / CHAR_BIT;
}
