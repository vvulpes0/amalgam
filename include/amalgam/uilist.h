/** @file uilist.h
 * @brief Linked lists of unsigned ints
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 *
 * A fully general linked list might have a <tt>(void*)</tt> pointer
 * for its data.  That results in an excessive amount of operations,
 * so this module declares linked lists over unsigned integers.
 * These are sufficient for indexing any portable array in C99,
 * while also remaining relatively compact.
 */
#ifndef UILIST_H
#define UILIST_H
#ifdef __cplusplus
extern "C" {
#if 0
} /* make the formatter happy */
#endif
#endif
/** @brief A linked list containing unsigned integers. */
struct uilist
{
	/** the next node */
	struct uilist * next;
	/** this node's contents */
	unsigned int value;
};

/** @brief Deep copy. \f$\mathcal{O}(n)\f$.
 *
 * Allocate sufficient memory to copy a list.
 * @param[in] p the list
 * @return a newly allocated copy of the input, NULL on failure
 */
struct uilist * ui_copy(struct uilist * p);

/** @brief Release memory. \f$\mathcal{O}(n)\f$.
 *
 * Release all memory held by a list.
 * @param p the list
 */
void ui_free(struct uilist * p);

/** @brief Determine if ascending lists intersect. \f$\mathcal{O}(n)\f$.
 *
 * @pre inputs are sorted
 * @param[in] a one of the ascending list operands
 * @param[in] b the other ascending list operand
 * @return nonzero iff the lists have a nonempty intersection
 */
int ui_has_intersect(struct uilist * a, struct uilist * b);

/** @brief Determine if lists are equal. \f$\mathcal{O}(n)\f$.
 *
 * @param[in] a one of the operands
 * @param[in] b the other operand
 * @return nonzero iff the lists have the same elements in the same order
 */
int ui_eq(struct uilist * a, struct uilist * b);

/** @brief Intersect lists. \f$\mathcal{O}(n)\f$.
 *
 * Contract \p a to contain only elements in \p b.
 * Deleted elements are freed.
 * The original head is likely invalidated; the new head is returned.
 * @pre inputs are sorted
 * @post outputs are sorted
 * @param[in,out] a the accumulating list
 * @param[in] b the elements to retain
 * @return a pointer to the new head of \p a
 */
struct uilist * ui_intersect(struct uilist * a, struct uilist * b);

/** @brief Combine lists. \f$\mathcal{O}(n)\f$.
 *
 * Expand \p a to contain any elements in \p b
 * which it did not already contain.
 * If \p a were NULL, a deep copy of \p b is returned.
 * Otherwise, the head of \p a is returned.
 * If \p a were non-NULL and the result is successful,
 * the return value is \p a itself.
 * @pre inputs are sorted
 * @post outputs are sorted
 * @param[in,out] a the accumulating list
 * @param[in] b the elements to potentially add
 * @return a pointer to the new head of the merged list
 */
struct uilist * ui_merge(struct uilist * a, struct uilist * b);
#ifdef __cplusplus
}
#endif
#endif
/* see LICENSE file for full text of license */
