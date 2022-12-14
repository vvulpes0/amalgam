/** @file amalgam/smonoid.h
 * @brief Analysis of syntactic monoids.
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 *
 * Every regular pattern can be associated with a finite semigroup.
 * This structure is fundamental in classifying pattern complexity.
 * An eggbox is a presentation of this structure
 * from which some decision problems are efficiently answered.
 * This header declares the structures and functions
 * needed for these tasks.
 *
 * Most of the implemented classification problems can be decided
 * using only the eggbox of the semigroup.
 * But with the exception of the simplest classes,
 * the local hierarchy (built on successor) and its tier-based extensions
 * require looking at every idempotent-generated local submonoid.
 * If you are interested only in classes based on general precedence
 * or the low-complexity local classes, sm_eggbox() suffices.
 * If you want to determine whether a give pattern is
 * (tier-based) locally (threshold) testable, then you will instead
 * need to use sm_localsm().
 */
#ifndef AMALGAM_SMONOID_H
#define AMALGAM_SMONOID_H
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
 * An eggbox diagram is a collection of D-classes.
 * Here each D-class is represented by as little information
 * as possible to successfully classify a language.
 * This structure alone suffices for many decision problems,
 * but others require more information.
 */
struct eggbox
{
	/** another box, if one exists */
	struct eggbox * next;
	/** whether eggs contain multiple elements */
	_Bool polyegg : 1;
	/** whether there are multiple R-classes */
	_Bool polyrow : 1;
	/** whether there are multiple L-classes */
	_Bool polycol : 1;
	/** whether this box is devoid of idempotents */
	_Bool irregular : 1;
	/** whether this box is regular but missing an idempotent */
	_Bool nonfull : 1;
};

/** a linked list of eggbox diagrams */
struct eggboxes
{
	/** another diagram, if one exists */
	struct eggboxes * next;
	/** the current diagram */
	struct eggbox * box;
};

/** @brief the structure over which all classifications may happen
 * simultaneously */
struct classifier
{
	/** a representation of the semigroup itself */
	struct eggbox * semigroup;
	/** a representation of its local submonoids.
	 * If the semigroup is itself a monoid,
	 * then the head of this list is object-identical
	 * to the representation of the semigroup itself
	 */
	struct eggboxes * localsm;
	/** is the semigroup commutative? */
	_Bool comm;
	/** are the proper local submonoids commmutative? */
	_Bool plcomm;
};

/** @brief Compute the submonoid generated by a set of elements.
 *
 * Restrict a semigroup to some subset of elements while guaranteeing
 * that the result has the structure of a monoid
 * by generating any combinations of elements not provided.
 * Like fi_smonoid() this guarantees that an identity element exists
 * and is the initial element.
 * @pre \p v is an ascending list
 * @pre \p M is a syntactic semigroup
 * @param[in] v the elements to use as generators
 * @param[in] M the semigroup to restrict
 * @return a newly allocated (syntactic) monoid
 */
struct finsa * sm_generate(struct uilist * v, struct finsa * M);

/** @brief Elements reachable through left-multiplication
 *
 * Compute an adjacency matrix for \p M where an element is set
 * iff the corresponding column \f$c\f$ can be reached by multiplying
 * an element \f$s\f$ to the left of the corresponding row \f$x\f$:
 * \f$c = sx\f$.
 * This is a left-coset matrix.
 * @param[in] M a syntactic semigroup or monoid
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
 * @param[in] M a syntactic semigroup or monoid
 * @return the right-coset matrix
 */
struct bmatrix * sm_rrel(struct finsa * M);

/** @brief Free an eggbox diagram and all its internal data.
 *
 * Each egg of each block is freed, then the block,
 * until no blocks remain.
 * @param B the eggbox diagram to free
 */
void sm_freebox(struct eggbox * B);

/** @brief Free a list of eggbox diagrams and all corresponding data.
 *
 * Each diagram is freed, then the node itself.
 * This continues until no nodes remain.
 * @param B the eggbox diagram to free
 */
void sm_freelist(struct eggboxes * B);

/** @brief Free a classifier structure and all its internal data.
 *
 * The semigroup and all of its proper local submonoids are freed,
 * alongside all associated data.
 * @param C the classifier structure to free
 */
void sm_free(struct classifier * C);

/** @brief Construct an eggbox diagram.
 *
 * Using the information of the syntactic semigroup,
 * an eggbox diagram is constructed.
 * @param[in] M a syntactic semigroup
 * @return an eggbox diagram for \p M, or NULL upon failure
 */
struct eggbox * sm_eggbox(struct finsa * M);

/** @brief Find proper submonoids generated by (nonidentity) idempotents.
 *
 * For each nonidentity idempotent element \f$e\f$ of a semigroup \f$S\f$,
 * compute the set \f$eSe\f$ and generate its eggbox diagram.
 * If \p x is non-NULL then the commutativity of each proper submonoid
 * is computed and the logical AND of these results
 * is output using this pointer.
 * @param[in] M the semigroup to subdivide
 * @param[out] x whether all proper local submonoids were commutative
 * @return eggbox diagrams for each proper local submonoid
 */
struct eggboxes * sm_localsm(struct finsa * M, int * x);

/** @brief Distill a semigroup into its bare essence.
 *
 * Construct the classifier struct over which all decision problems operate.
 */
struct classifier * sm_classifier(struct finsa * M);

/** @brief Apply a decision problem to local submonoids.
 * @param[in] B eggboxes of local submonoids
 * @param[in] f the decision procedure to apply
 * @param x whether to use the projective subsemigroup
 * @return nonzero iff all inspected monoids are deemed satisfactory
 */
int sm_ese(struct classifier * B, int(*f)(struct eggbox *), int x);

/** @brief Decide if an eggbox represents a trivial pattern.
 *
 * A trivial pattern is one describable with but a single state.
 * It accepts either all strings or no strings over its alphabet.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is trivial
 */
int sm_istriv(struct classifier * B);

/** @brief Decide if an eggbox represents a nilpotent pattern.
 *
 * A pattern is nilpotent iff it is (co-)finite:
 * either it or its complement consists of a finite number of words.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is nilpotent
 */
int sm_isn(struct classifier * B, int x);

/** @brief Decide if an eggbox represents a tier-based nilpotent pattern.
 *
 * A pattern is tier-based nilpotent iff either it or its complement
 * consists of a finite number of words over some tier,
 * and non-tier symbols can be freely inserted and deleted.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based nilpotent
 */
int sm_istn(struct classifier * B);

/** @brief Decide if an eggbox represents a definite pattern.
 *
 * A pattern is definite iff it is described
 * by a Boolean combination of suffixes.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is definite
 */
int sm_isd(struct classifier * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based definite pattern.
 *
 * A pattern is tier-based definite iff it is described
 * by a Boolean combination of suffixes on some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based definite
 */
int sm_istd(struct classifier * B);

/** @brief Decide if an eggbox represents a reverse definite pattern.
 *
 * A pattern is definite iff it is described
 * by a Boolean combination of prefixes.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is reverse definite
 */
int sm_isk(struct classifier * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based reverse definite pattern.
 *
 * A pattern is tier-based definite iff it is described
 * by a Boolean combination of prefixes on some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based reverse definite
 */
int sm_istk(struct classifier * B);

/** @brief Decide if an eggbox represents a generalized definite pattern.
 *
 * A pattern is generalized definite iff it is described
 * by a Boolean combination of prefixes and suffixes.
 * @param[in] B the eggbox diagram
 * @param x nonzero iff a solitary identity element should be ignored
 * @return nonzero iff the pattern is generalized definite
 */
int sm_isgd(struct classifier * B, int x);

/** @brief Decide if an eggbox represents
 * a tier-based generalized definite pattern.
 *
 * A pattern is tier-based generalized definite iff it is described
 * by a Boolean combination of prefixes and suffixes on some tier.
 * A solitary identity element is always ignored.
 * @param[in] B the eggbox diagram
 * @return nonzero iff the pattern is tier-based generalized definite
 */
int sm_istgd(struct classifier * B);

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
int sm_islt(struct classifier * B, int x);

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
int sm_istlt(struct classifier * B);

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
int sm_islj(struct classifier * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally J-trivial pattern.
 *
 * Is the projected subsemigroup locally J-trivial?
 *
 * Equivalent to <tt>sm_ese(B, sm_ispt, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally J-trivial
 */
int sm_istlj(struct classifier * B);

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
int sm_islltriv(struct classifier * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally L-trivial pattern.
 *
 * Is the projected subsemigroup locally L-trivial?
 *
 * Equivalent to <tt>sm_ese(B, sm_isltriv, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally L-trivial
 */
int sm_istlltriv(struct classifier * B);

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
int sm_islrtriv(struct classifier * B, int x);

/** @brief Decide if eggboxes represent a tier-based locally R-trivial pattern.
 *
 * Is the projected subsemigroup locally R-trivial?
 *
 * Equivalent to <tt>sm_ese(B, sm_isrtriv, 1)</tt>.
 * @param[in] B the eggbox diagrams of local submonoids
 * @return nonzero iff the pattern is tier-based locally R-trivial
 */
int sm_istlrtriv(struct classifier * B);

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
int sm_islda(struct classifier * B, int x);

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
int sm_istlda(struct classifier * B);

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
