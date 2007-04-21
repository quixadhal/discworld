#ifndef __MIME_H
#define __MIME_H

/**
 * The header file for the MIME header parser.
 */

/**
 * The path to the MIME parser.
 */
#define MIME "/obj/handlers/mime"

/**
 * Checks if c is a white-space character.
 * @param c the character to check
 * @return 1 if c is a white-space character, 0 otherwise
 */
#define isspace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n') || \
                    ((c) == 12) || ((c) == '\r') || ((c) == 11))

/**
 * Checks if c is a control character.
 * @param c the character to check
 * @return 1 if c is a control character, 0 otherwise
 */
#define iscntrl(c) (((c) == 127) || (((c) >= 0) && ((c) <= 31)))

/**
 * Checks if c is a hexadecimal digit.
 * @param c the character to check
 * @return 1 if c is a hexadecimal digit, 0 otherwise
 */
#define isxdigit(c) ((((c) >= 'A') && ((c) <= 'F')) || \
                     (((c) >= 'a') && ((c) <= 'f')) || \
                     (((c) >= '0') && ((c) <= '9')))

/**
 * Checks if c is an alphanumeric character.
 * @param c the character to check
 * @return 1 if c is a letter or digit, 0 otherwise
 */
#define isalnum(c) ((((c) >= '0') && ((c) <= '9')) || \
                    (((c) >= 'A') && ((c) <= 'Z')) || \
                    (((c) >= 'a') && ((c) <= 'z')))

/**
 * Checks if c is a digit character.
 * @param c the character to check
 * @return 1 if c is a digit, 0 otherwise
 */
#define isdigit(c) (((c) >= '0') && ((c) <= '9'))

class mime_header {
  mapping header_m;             /* keys: header fields, values: values */
  string *header_k;             /* ordered array of header fields */
}

#endif /* __MIME_H */
