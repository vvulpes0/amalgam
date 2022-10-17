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
 *
 * Most of the implemented classification problems can be decided
 * using only the eggbox of the monoid.
 * But with the exception of the simplest classes,
 * the local hierarchy (built on successor) and its tier-based extensions
 * require looking at every idempotent-generated local submonoid.
 * If you are interested only in classes based on general precedence
 * or the low-complexity local classes, sm_eggbox() suffices.
 * If you want to determine whether a give pattern is
 * (tier-based) locally (threshold) testable, then you will instead
 * need to use sm_localsm().
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

/** a linked list of eggbox diagrams */
struct eggboxes
{
	/** another diagram, if one exists */
	struct eggboxes * next;
	/** the current diagram */
	struct eggbox * box;
};

/** @brief Compute the submonoid generated by a set of elements.
 *
 * Restrict a monoid to some subset of elements while guaranteeing
 * that the result still has the structure of a monoid
 * by generating any combinations of elements not provided.
 * Like fi_smonoid() this guarantees that an identity element exists
 * and is the initial element.
 * @pre \p v is an ascending list
 * @pre \p M is a syntactic monoid
 * @param[in] v the elements to use as generators
 * @param[in] M the monoid to restrict
 * @return a newly allocated syntactic monoid
 */
struct finsa * sm_generate(struct uilist * v, struct finsa * M);

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

/** @brief Free a list of eggbox diagrams and all corresponding data.
 *
 * Each diagram is freed, then the node itself.
 * This continues until no nodes remain.
 * @param B the eggbox diagram to free
 */
void sm_freelist(struct eggboxes * B);

/** @brief Construct an eggbox diagram.
 *
 * Using the information of the syntactic monoid,
 * an eggbox diagram is constructed.
 * @param[in] M a syntactic monoid
 * @return an eggbox diagram for \p M, or NULL upon failure
 */
struct eggbox * sm_eggbox(struct finsa * M);

/** @brief Find submonoids generated by idempotents.
 *
 * For each idempotent element \f$e\f$ of a semigroup \f$S\f$,
 * compute the set \f$eSe\f$ and generate its eggbox diagram.
 * If \p x is non-NULL then the commutativity of each proper submonoid
 * is computed and the logical AND of these results
 * is output using this pointer.
 * @param[in] M the monoid to subdivide
 * @param[out] x whether all local proper submonoids were commutative
 * @return eggbox diagrams for each local submonoid
 */
struct eggboxes * sm_localsm(struct finsa * M, int * x);

/** @brief Apply a decision problem to local submonoids.
 * @param[in] B eggboxes of local submonoids
 * @param[in] f the decision procedure to apply
 * @param x whether to ignore the full monoid
 * @return nonzero iff all inspected monoids are deemed satisfactory
 */
int sm_ese(struct eggboxes * B, int(*f)(struct eggbox *), int x);

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

/** @brief Decide if eggboxes represent a locally testable pattern.
 *
 * A locally testable pattern is one describable in propositional
 * (quantifier-free) logic using only the successor operation.
 *
 * Equivalent to <tt>sm_ese(B, sm_issemilat, x)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is locally testable
 */
int sm_islt(struct eggboxes * B, int x);

/** @brief Decide if eggboxes represent a tier-based LT pattern.
 *
 * A tier-based locally testable pattern is one describable
 * in propositional (quantifier-free) logic using only
 * a single tier-successor operation.
 *
 * Equivalent to <tt>sm_ese(B, sm_issemilat, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally testable
 */
int sm_istlt(struct eggboxes * B);

/** @brief Decide if eggboxes represent a locally J-trivial pattern.
 *
 * A locally J-trivial pattern is one whose local submonoids
 * are all J-trivial (PT).  This is a proper superclass of the
 * class of languages of Dot-Depth One, so nonmembership here
 * is nonmembership there as well.
 *
 * Equivalent to <tt>sm_ese(B, sm_ispt, x)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is locally J-trivial
 */
int sm_islj(struct eggboxes * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally J-trivial pattern.
 *
 * Is the projected subsemigroup locally J-trivial?
 *
 * Equivalent to <tt>sm_ese(B, sm_ispt, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally J-trivial
 */
int sm_istlj(struct eggboxes * B);

/** @brief Decide if eggboxes represent a locally L-trivial pattern.
 *
 * A locally L-trivial pattern is one whose local submonoids
 * are all L-trivial.
 *
 * Equivalent to <tt>sm_ese(B, sm_isltriv, x)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is locally L-trivial
 */
int sm_islltriv(struct eggboxes * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally L-trivial pattern.
 *
 * Is the projected subsemigroup locally L-trivial?
 *
 * Equivalent to <tt>sm_ese(B, sm_isltriv, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally L-trivial
 */
int sm_istlltriv(struct eggboxes * B);

/** @brief Decide if eggboxes represent a locally R-trivial pattern.
 *
 * A locally R-trivial pattern is one whose local submonoids
 * are all R-trivial.
 *
 * Equivalent to <tt>sm_ese(B, sm_isrtriv, x)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is locally R-trivial
 */
int sm_islrtriv(struct eggboxes * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally R-trivial pattern.
 *
 * Is the projected subsemigroup locally R-trivial?
 *
 * Equivalent to <tt>sm_ese(B, sm_isrtriv, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally R-trivial
 */
int sm_istlrtriv(struct eggboxes * B);

/** @brief Decide if eggboxes represent a locally DA pattern.
 *
 * A locally DA pattern is one describable in first-order logic
 * restricted to two variables using any combination of the
 * general precedence operation and the successor operation.
 *
 * Equivalent to <tt>sm_ese(B, sm_isda, x)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is locally DA
 */
int sm_islda(struct eggboxes * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally R-trivial pattern.
 *
 * A tier-based locally DA pattern is one describable in
 * first-order logic restricted to two variables
 * using any combination of the general precedence operation
 * and a single tier-successor operation.
 *
 * Equivalent to <tt>sm_ese(B, sm_isda, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally DA
 */
int sm_istlda(struct eggboxes * B);

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

/** @brief Decide if a monoid is commutative.
 *
 * @param[in] M the monoid
 * @return nonzero iff \f$ab=ba\f$ for all pairs of elements
 */
int sm_iscom(struct finsa * M);

/** @brief Decide if a monoid is aperiodic and commutative.
 *
 * Equivalent to <tt>sm_issf(B) && sm_iscom(M)</tt>.
 * If you would like to decide this class without retaining the monoid,
 * simply note its commutativity before freeing it.
 * An aperiodic commutative monoid represents
 * a locally threshold testable language with a factor width of 1.
 * @param[in] B an eggbox diagram
 * @param[in] M the monoid \p B came from
 * @return nonzero iff the monoid is star-free and commutative.
 */
int sm_isacom(struct eggbox * B, struct finsa * M);

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
