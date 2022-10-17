/** @file smonoid.h
 * @brief Analysis of syntactic monoids.
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 *
 * Every regular pattern can be associated with a finite monoid.
 * This structure is fundamental in classifying pattern complexity.
 * An eggbox is a presentation of this structure
 * from which some decision problems are efficiently answered.
 * This header declares the structures and functions
 * needed for these tasks.
 */
#ifndef SMONOID_H
#define SMONOID_H
#ifdef __cplusplus
extern "C" {
#if 0
} /* make the formatter happy */
#endif
#endif
#include <stddef.h>

struct bmatrix;
struct finsa;
struct uilist;

/** @brief An eggbox diagram.
 *
 * An egg is a maximal set of H-related elements.
 * Within each box, eggs are arranged into a rectangular grid.
 * The groups are all and only those eggs that contain idempotents.
 * Eggs that are R-related are in the same row,
 * while those that are L-related are in the same column.
 * This structure is sufficient to decide
 * many algebraic classification problems.
 */
struct eggbox
{
	/** another box, if one exists */
	struct eggbox * next;
	/** an array of eggs (lists of H-related elements) */
	struct uilist ** eggs;
	/** an array sized like \c eggs indicating which are groups */
	_Bool * groups;
	/** number of rows (R-classes) of eggs */
	size_t rows;
	/** number of columns (L-classes) of eggs */
	size_t cols;
};

/** @brief Elements reachable through left-multiplication
 *
 * Compute an adjacency matrix for \p M where an element is set
 * iff the corresponding column \f$c\f$ can be reached by multiplying
 * an element \f$s\f$ to the left of the corresponding row \f$x\f$:
 * \f$c = sx\f$.
 * This is a left-coset matrix.
 * @param[in] M a syntactic monoid
 * @return the left-coset matrix
 */
struct bmatrix * sm_lrel(struct finsa * M);

/** @brief Elements reachable through right-multiplication
 *
 * Compute an adjacency matrix for \p M where an element is set
 * iff the corresponding column \f$c\f$ can be reached by multiplying
 * an element \f$s\f$ to the right of the corresponding row \f$x\f$:
 * \f$c = xs\f$.
 * This is a right-coset matrix.
 * @param[in] M a syntactic monoid
 * @return the right-coset matrix
 */
struct bmatrix * sm_rrel(struct finsa * M);

/** @brief Free an eggbox diagram and all its internal data.
 *
 * Each egg of each block is freed, then the block,
 * until no blocks remain.
 * @param B the eggbox diagram to free
 */
void sm_free(struct eggbox * B);

/** @brief Construct an eggbox diagram.
 *
 * Using the information of the reachability matrix,
 * an eggbox diagram is constructed.
 * Sufficient memory may be allocated to copy and transpose \p R
 * in order to determine the relatedness of elements.
 * @param[in] R a reachability matrix
 * @return an eggbox diagram for \p R, or NULL upon failure
 */
struct eggbox * sm_eggbox(struct finsa * R);

/** @brief Decide if an eggbox represents a trivial pattern.
 *
 * A trivial pattern is one describable with but a single state.
 * It accepts either all strings or no strings over its alphabet.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is trivial
 */
int sm_istriv(struct eggbox * B);

/** @brief Decide if an eggbox represents a (co-)finite pattern.
 *
 * A pattern is (co-)finite iff either it or its complement
 * consists of a finite number of words.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is (co-)finite
 */
int sm_isf(struct eggbox * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based definite pattern.
 *
 * A pattern is tier-based (co-)finite iff either it or its complement
 * consists of a finite number of words over some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based (co-)finite
 */
int sm_istf(struct eggbox * B);

/** @brief Decide if an eggbox represents a definite pattern.
 *
 * A pattern is definite iff it is described
 * by a Boolean combination of suffixes.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is definite
 */
int sm_isd(struct eggbox * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based definite pattern.
 *
 * A pattern is tier-based definite iff it is described
 * by a Boolean combination of suffixes on some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based definite
 */
int sm_istd(struct eggbox * B);

/** @brief Decide if an eggbox represents a reverse definite pattern.
 *
 * A pattern is definite iff it is described
 * by a Boolean combination of prefixes.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is reverse definite
 */
int sm_isk(struct eggbox * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based reverse definite pattern.
 *
 * A pattern is tier-based definite iff it is described
 * by a Boolean combination of prefixes on some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based reverse definite
 */
int sm_istk(struct eggbox * B);

/** @brief Decide if an eggbox represents a generalized definite pattern.
 *
 * A pattern is generalized definite iff it is described
 * by a Boolean combination of prefixes and suffixes.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is generalized definite
 */
int sm_isgd(struct eggbox * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based generalized definite pattern.
 *
 * A pattern is tier-based generalized definite iff it is described
 * by a Boolean combination of prefixes and suffixes on some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based generalized definite
 */
int sm_istgd(struct eggbox * B);

/** @brief Decide if an eggbox represents a semilattice.
 *
 * A semilattice is a commutative band,
 * a commutative structure where every element is idempotent.
 * Such a pattern is necessarily in PT,
 * and therefore necessarily able to be described
 * in quantifier-free, propositional logic
 * using the general-precedence operation.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is a semilattice
 */
int sm_issemilat(struct eggbox * B);

/** @brief Decide if an eggbox represents a piecewise testable pattern.
 *
 * A piecewise testable pattern is one describable in propositional
 * (quantifier-free) logic using only the general-precedence operation.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is piecewise testable
 */
int sm_ispt(struct eggbox * B);

/** @brief Decide if an eggbox represents an L-trivial pattern.
 *
 * An L-trivial pattern is one in which \f$M_e e=e\f$
 * for all idempotents \f$e\f$.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is L-trivial.
 */
int sm_isltriv(struct eggbox * B);

/** @brief Decide if an eggbox represents an R-trivial pattern.
 *
 * An L-trivial pattern is one in which \f$e M_e=e\f$
 * for all idempotents \f$e\f$.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is R-trivial.
 */
int sm_isrtriv(struct eggbox * B);

/** @brief Decide if an eggbox represents a band.
 *
 * A band is a structure where every element is idempotent.
 * Such a pattern is necessarily in DA,
 * and therefore necessarily able to be described
 * in first-order logic restricted to two variables
 * using the general-precedence operation.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is everywhere-idempotent
 */
int sm_isband(struct eggbox * B);

/** @brief Decide if an eggbox represents a pattern in DA.
 *
 * A pattern is in DA iff it is describable in first-order logic
 * restricted to two variables using the general-precedence operation.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is in DA
 */
int sm_isda(struct eggbox * B);

/** @brief Decide if an eggbox represents a star-free pattern.
 *
 * A star-free pattern is one describable in first-order logic
 * using the general-precedence operation.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is star-free
 */
int sm_issf(struct eggbox * B);

#ifdef __cplusplus
}
#endif
#endif
/* see LICENSE file for full text of license */
