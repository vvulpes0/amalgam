/** @file finsa.h
 * @brief Operations on finite-state automata
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 *
 * This header declares types and functions useful
 * in manipulating finite-state automata.
 * An automaton is an edge-labeled directed graph,
 * which can be thought of as several overlayed directed graphs,
 * one per edge.
 */
#ifndef FINSA_H
#define FINSA_H
#ifdef __cplusplus
extern "C" {
#if 0
} /* make the formatter happy */
#endif
#endif
#include <stddef.h>

struct bmatrix;
struct uilist;

/** @brief A finite-state automaton.
 *
 * There is exactly one initial state and it is necessarily the first. */
struct finsa
{
	/** a list of overlays */
	struct bmatrix ** graphs;
	/** an ascending list of accepting states */
	struct uilist * finals;
	/** how many overlays there are */
	size_t count;
};

/** @brief Deep copy an automaton. \f$\mathcal{O}(n)\f$
 *
 * Allocate sufficient memory for a new automaton and return the result.
 * The result is NULL upon failure or if \p M were of zero size.
 * @param[in] M the automaton to copy
 * @return a deep copy of \p M, or NULL upon failure
 */
struct finsa * fi_copy(struct finsa * M);

/** @brief Free an automaton and all its internal data.
 *
 * Every graph is free, then the finals, then the structure itself.
 * @param M the automaton to free
 */
void fi_free(struct finsa * M);

/** @brief Minimize an automaton.
 *
 * Merge Nerode-equivalent states.
 * @pre \p M is deterministic
 * @param[in,out] M the automaton
 * @return nonzero iff no error was encountered
 */
int fi_nerode(struct finsa * M);

/** @brief Retain only specific nodes.
 *
 * @param[in,out] M the automaton
 * @param[in] V the states to retain
 */
void fi_restrict(struct finsa * M, struct uilist * V);

/** @brief Retain only nodes reachable from the start.
 *
 * @param[in,out] M the automaton
 * @return nonzero iff trimming successful
 */
int fi_trim(struct finsa * M);

/** @brief Propagate past empty transitions.
 *
 * @param[in,out] M the automaton
 * @param e the character index of an empty transition
 */
void fi_rmeps(struct finsa * M, int e);

/** @brief Powerset construction.
 *
 * Incrementally construct an automaton whose states
 * correspond to sets of states of \p M.
 * An edge labelled \a s exists between states \a a and \a b iff
 * there is a state \a p in \a a and \a q in \a b
 * such that an edge labelled \a s exists between \a p and \a q.
 * The initial state from which the construction is seeded
 * is given by \p V.
 * @param[in] M the base automaton
 * @param[in] V the initial set of states
 * @return a new deterministic automaton
 */
struct finsa * fi_powerset(struct finsa * M, struct uilist * V);

/** @brief Construct a syntactic monoid.
 *
 * The syntactic monoid is an algebraic structure
 * derived from a Nerode-minimal automaton.
 * In this system, such a structure
 * is defined as an automaton with one symbol (one overlay) per node
 * and with no accepting states.
 * @pre inputs are in Myhill- or Nerode-minimal form
 * @post outputs are in Myhill-minimal form
 * @param[in] M the automaton
 * @return a representation of the syntactic monoid of \p M
 */
struct finsa * fi_smonoid(struct finsa * M);

#ifdef __cplusplus
}
#endif
#endif
