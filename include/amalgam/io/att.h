/** @file amalgam/io/att.h
 * @brief Import an AT&T tabular format automaton.
 * @author Dakotah Lambert
 * @version 0.1.0
 * @date 2022
 * @copyright MIT
 *
 * An automaton file consists of arcs and accepting states.
 * An arc is composed of three whitespace-separated fields:
 * a source, a destination, and an input symbol.
 * For compatibility with OpenFST, lines with up to five fields
 * are accepted, but only the first three are inspected.
 * The source and destination are states, denoted by integers.
 * The input symbol is an arbitrary string not containing whitespace,
 * although the string "0" (without the quotes) is reserved
 * for use as an input of the empty string.
 * An accepting state is a single integer,
 * but again for compatibility with OpenFST
 * lines with up to two fields are accepted,
 * but only the first is inspected.
 *
 * Example:
 *
 * 0 1 b<br>
 * 0 1 c<br>
 * 0 2 a<br>
 * 0<br>
 * 1 1 b<br>
 * 1 1 c<br>
 * 1<br>
 * 2 2 a<br>
 * 2 2 c<br>
 * 2 3 b<br>
 * 3 2 a<br>
 * 3 2 c<br>
 * 3 3 b<br>
 * 3<br>
 */
#ifndef AMALGAM_IO_ATT_H
#define AMALGAM_IO_ATT_H
#ifdef __cplusplus
extern "C" {
#if 0
} /* make the formatter happy */
#endif
#endif
#include <stdio.h> /* for FILE */

struct finsa;

/** @brief Read an AT&T tabular format file.
 *
 * Read a file and construct an automaton from valid lines.
 * Lines which could not be understood are ignored.
 * This function does not seek, and therefore should be compatible
 * with standard streams.
 * @param f the file to read
 * @return a newly formed automaton
 */
struct finsa * fi_fromatt(FILE * f);
#ifdef __cplusplus
}
#endif
#endif
