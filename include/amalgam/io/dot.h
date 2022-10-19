/** @file amalgam/io/dot.h
 * @brief Export a GraphViz DOT format file.
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 */
#ifndef AMALGAM_IO_DOT_H
#define AMALGAM_IO_DOT_H
#ifdef __cplusplus
extern "C" {
#if 0
} /* make the formatter happy */
#endif
#endif
#include <stdio.h> /* for FILE */

struct finsa;

/** @brief Draw an automaton as a DOT file.
 *
 * @param f the file to write
 * @param[in] M the automaton
 * @return the number of characters written
 */
int fi_todot(FILE * f, struct finsa * M);

/** @brief Draw the boxes of an eggbox diagram.
 *
 * @param f the file to write
 * @param[in] B the eggbox diagram
 * @return the number of characters written
 */
int sm_todot(FILE * f, struct eggbox * B);
#ifdef __cplusplus
}
#endif
#endif
