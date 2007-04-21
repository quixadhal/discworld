/**
 * This is the support stuff for the peopler command...
 * The tags are all placed into an array.  The MASKs for RIGHT and CENTER
 * can be applied to the tags to make them centered or right justified
 * respectively.
 * @author Pinkfish
 */

#ifndef __PEOPLE_H
/** @ignore yes */
#define __PEOPLE_H

/** A name field. */
#define NAME 0
/** A Capitalized name field. */
#define C_NAME 1
/** This is used to put spaces into the output. */
#define STRING 2
/** The gender field. */
#define GENDER 3
/** The ip number field. */
#define IP_NUMBER 4
/** The ip name field. */
#define IP_NAME 5
/** The object type field (C, p, etc). */
#define TYPE 6
/** The players age field. */
#define AGE 7
/** The players guild field. */
#define GUILD 8
/** The players race field. */
#define RACE 9
/** The plauyers level field. */
#define LEVEL 10
/** The players terminal type field. */
#define TERMINAL 11
/** The current numbr of rows for the player. */
#define ROWS 12
/** The current number of columns for the player. */
#define COLS 13
/** The current room the player is in. */
#define ROOM 14
/** The players effective user id. */
#define EUID 15
/** The players user id. */
#define UID 16
/** The creator file. */
#define CFILE 17
/** The current directory of the player. */
#define CUR_DIR 18
/** Is the player allowed to be on together with someone from the same IP? */
#define ND_ALLOWED 19

/** The Center mask, center the info. */
#define CENTER 128
/** The right justify mask. */
#define RIGHT 64
/** The mask of change flags. */
#define MASK (RIGHT+CENTER)
/** The list of abbreviations for the types. */
#define ABBREV ({ \
          "n", \
          "N", \
          "",  \
          "G", \
          "#", \
          "I", \
          "T", \
          "A", \
          "g", \
          "R", \
          "L", \
          "t", \
          "r", \
          "c", \
          "O", \
          "E", \
          "U", \
          "F", \
          "/", \
          "a"  \
        })
/** The names of the types, given in the header. */
#define DE_NAMES ({ \
          "Name", \
          "Capitalized Name", \
          "", \
          "Gender", \
          "IP Number", \
          "IP Name", \
          "Type", \
          "Age", \
          "Guild", \
          "Race", \
          "Level", \
          "Terminal type", \
          "Rows", \
          "Cols", \
          "Room", \
          "Euid", \
          "Uid", \
          "Creator file", \
          "Current dir", \
          "Not allowed with" \
        })

/* This is the default people listing thingy. */
/** The default set of of tags for the people command. */
#define P_DEFAULT ({ \
          NAME, 15, \
          TYPE|CENTER, 3, \
          GENDER|CENTER, 6, \
          STRING, " ", \
          GUILD|CENTER, 10, \
          STRING, " ", \
          LEVEL|RIGHT, 4, \
          STRING, " ", \
          AGE, 5, \
          ROOM, 30 \
        })
/** The default set of tags for the quick people command. */
#define QP_DEFAULT ({ \
          AGE|RIGHT, 4, \
          STRING, " ", \
          NAME, 30, \
        })
/** The default set of flage for the term command. */
#define T_DEFAULT ({ \
          NAME, 12, \
          ROWS|CENTER, 5, \
          STRING, " ", \
          COLS|CENTER, 4, \
          STRING, " ", \
          TERMINAL, 30, \
        })
/** The default set of flags for the netstat command. */
#define N_DEFAULT ({ \
          NAME, 12, \
          STRING, " ", \
          IP_NUMBER, 16, \
          STRING, " ", \
          IP_NAME, 30, \
        })
#define SN_DEFAULT ({ \
          IP_NAME, 30, \
          STRING, " ", \
          IP_NUMBER, 16, \
          STRING, " ", \
          NAME, 12, \
        })
/** The default set of flags for the dirs command. */
#define D_DEFAULT ({ \
          NAME, 12, \
          STRING, " ", \
          CUR_DIR, 25, \
          STRING, " ", \
          ROOM, 40, \
        })
/** The default set of flags for the netdups command. */
#define ND_DEFAULT ({ \
          NAME, 12, \
          STRING, " ", \
          IP_NUMBER, 16, \
          STRING, " ", \
          IP_NAME, 20, \
          STRING, " ", \
          ND_ALLOWED, 100, \
        })

#endif
