#define BASE            "/obj/soul/"
#define COMPILER        (BASE + "scompile.c")
#define RELAYER         (BASE + "srelay.c")
#define SOUL_DEF_DIR    (BASE + "soul_defs")
#define SAVE_FILE	"/save/soul/soul"

/*
 * This is done for a quick stringp() check in the relayer.
 */

#define ADJ           0
#define TP_NAME       1
#define OB_NAME       2
#define TP_PRONOUN    3
#define OB_PRONOUN    4
#define TP_POSSESSIVE 5
#define OB_POSSESSIVE 6
#define TP_NPOSS      7
#define OB_NPOSS      8
#define TP_FOO        9
#define OB_FOO        10

/*
 * Stupid macroses, we hates them, we does. But they are handy sometimes.
 */

#define TP this_player()
#define OB0 targets[0]
#define S(str) (str ? str : "")

