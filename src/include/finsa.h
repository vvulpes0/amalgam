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

/** @brief Free an automaton and all its internal data.
 *
 * Every graph is free, then the finals, then the structure itself.
 * @param M the automaton to free
 */
void fi_free(struct finsa * M);

/** Construct a syntactic monoid.
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
