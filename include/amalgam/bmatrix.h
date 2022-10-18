/** @file amalgam/bmatrix.h
 * @brief Operations on sparse square matrices.
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 *
 * Sparse square matrices over the Boolean semigroup of two elements
 * are particularly useful for storing directed graphs.
 * An edge-labeled directed graph is merely a collection of graphs
 * that overlay upon each other.
 *
 * Prioritizing low memory usage, most operations are in-place.
 * The restriction to square matrices prevents unnecessary reallocation.
 *
 * When big-O notations are provided,
 * \f$V\f$ is the number of rows or columns in the matrix, while
 * \f$E\f$ is the number of set entries, and
 * \f$n\f$ is \f$V+E\f$
 */
#ifndef AMALGAM_BMATRIX_H
#define AMALGAM_BMATRIX_H
#ifdef __cplusplus
extern "C" {
#if 0
} /* make the formatter happy */
#endif
#endif
#include <stddef.h>

struct uilist;

/** @brief A sparse bit-matrix.
 *
 * Matrices over the Boolean semiring of two elements.
 * In this semiring, addition is a logical OR operation
 * and multiplication a logical AND.
 */
struct bmatrix
{
	/** a list of rows,
	 * each of which an ascending list
	 * of column-indices where a 1 occurs */
	struct uilist ** vecs;
	/** the number of rows and columns */
	size_t size;
};

/** @brief Construct an identity matrix. \f$\mathcal{O}(V)\f$
 *
 * Allocate a new matrix whose entries are all zero save for
 * the main diagonal upon which entries are one.
 * @param V the number of rows / columns
 * @return an identity matrix, or NULL if insufficient memory
 */
struct bmatrix * bx_identity(size_t V);

/** @brief Construct a cyclic adjacency matrix. \f$\mathcal{O}(V)\f$
 *
 * Allocate a new matrix whose entries are all zero save for
 * the elements to the right of the main diagonal (with wrapping)
 * where entries are one.
 * @param V the number of rows / columns
 * @return a cyclic adjacency matrix, or NULL if insufficient memory
 */
struct bmatrix * bx_cycle(size_t V);

/** @brief Deep copy a matrix. \f$\mathcal{O}(n)\f$
 *
 * Allocate sufficient memory for a new matrix and return the result.
 * The result is NULL upon failure or if \p M were of zero size.
 * @param[in] M the matrix to copy
 * @return a deep copy of \p M, or NULL upon failure
 */
struct bmatrix * bx_copy(struct bmatrix * M);

/** @brief Free a matrix and all its internal data. \f$\mathcal{O}(n)\f$
 *
 * Every row of the matrix is freed,
 * then finally the matrix itself is freed.
 * @param M the matrix to free
 */
void bx_free(struct bmatrix * M);

/** @brief Transpose a matrix. \f$\mathcal{O}(n)\f$
 *
 * Swap the rows and columns of the matrix. For example:
 * \f[
 * \left[\begin{array}{ccc}0&0&0\\1&0&1\\1&1&0\end{array}\right]
 * \rightarrow
 * \left[\begin{array}{ccc}0&1&1\\0&0&1\\0&1&0\end{array}\right]
 * \f]
 * @param[in,out] M the matrix to transpose
 * @return nonzero iff successful
 */
int bx_transpose(struct bmatrix * M);

/** @brief Compare matrices. \f$\mathcal{O}(n)\f$
 *
 * Two matrices are equal iff they have the same elements set and unset.
 * @return nonzero iff matrices were equal
 */
int bx_eq(struct bmatrix * A, struct bmatrix * M);

/** @brief Add two matrices. \f$\mathcal{O}(n)\f$
 *
 * Addition takes place in the Boolean semiring over two elements:
 * if an element is True in either addend, it is True in the result.
 * This is essentially an elementwise OR operation.
 * @param[in,out] A the accumulating destination
 * @param[in]     M the matrix to add into \p A
 * @return nonzero iff matrices were compatible and operation succeeded
 */
int bx_add(struct bmatrix * A, struct bmatrix * M);

/** @brief Elementwise multiply two matrices. \f$\mathcal{O}(n)\f$
 *
 * Multiplication takes place in the Boolean semiring over two elements:
 * if an element is False in either addend, it is False in the result.
 * This is essentially an elementwise AND operation.
 * @param[in,out] A the accumulating destination
 * @param[in]     M the matrix to multiply into \p A
 * @return nonzero iff matrices were compatible and operation succeeded
 */
int bx_emul(struct bmatrix * A, struct bmatrix * M);

/** @brief Multiply two matrices: \f$A\cdot M\f$. \f$\mathcal{O}(V^3)\f$
 *
 * Multiplication takes place in the Boolean semiring over two elements:
 * addition saturates at one and shall go no higher.
 * If each matrix is an adjacency matrix over the same vertices
 * (ordered the same way), the result is the adjacency matrix
 * obtained by first following an edge represented in \p A
 * then one represented in \p M.
 *
 * This function may allocate sufficient memory
 * to hold a copy of each operand.
 * @param[in,out] A the accumulating destination
 * @param[in]     M the matrix to multiply into \p A
 * @return nonzero iff matrices were compatible and operation succeeded
 */
int bx_mul(struct bmatrix * A, struct bmatrix * M);

/** @brief Compute reachability over an adjacency matrix.
 * \f$\mathcal{O}(V^3\log V)\f$
 *
 * If \p A is an adjacency matrix, the result is the transitive closure
 * of the associated operation unioned with the identity.
 * More generically, computes the fixed point of multiplying
 * the union of A with the identity by itself ad infinitum.
 *
 * At the cost of some excess memory, \p A is modified
 * only if the result is successful.
 * @param[in,out] A the accumulating destination
 * @return nonzero iff the operation succeeded
 */
int bx_reachability(struct bmatrix * A);

/** @brief Compute vector-matrix multiplication \f$vM\f$.
 * \f$\mathcal{O}(n+vV)\f$ (for \f$v\f$ the set entries in the vector)
 *
 * Multiply a row-vector by a matrix.  The matrix is on the right,
 * so set elements of the vector select rows, which are then combined
 * with the sum of the semigroup (logical OR).
 *
 * This is not an in-place operation.
 * @param[in] v the vector
 * @param[in] M the matrix
 * @return the vector resulting from the multiplication
 */
struct uilist * bx_vmmul(struct uilist * v, struct bmatrix * M);

/** @brief Compute matrix-vector multiplication \f$Mv\f$.
 * \f$\mathcal{O}(vV)\f$ (for \f$v\f$ the set entries in the vector)
 *
 * Multiply a matrix by a column-vector.  The matrix is on the left,
 * so set elements of the vector select columns, which are then combined
 * with the sum of the semigroup (logical OR).
 *
 * This is not an in-place operation.
 * @param[in] M the matrix
 * @param[in] v the vector
 * @return the vector resulting from the multiplication
 */
struct uilist * bx_mvmul(struct bmatrix * M, struct uilist * v);
#ifdef __cplusplus
}
#endif
#endif
/* see LICENSE file for full text of license */
