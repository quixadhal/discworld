/**
 * This is the file that contains the language bits needed for dealing with
 * languages.  This is for reading and so on too.
 * @author Pinkfish
 * @started in 1992 sometime
 */
#ifndef __SYS__LANGUAGE
/** @ignore yes */
#define __SYS__LANGUAGE

/** @ignore yes */
#define LANGUAGES "/obj/handlers/languages"
/**
 * The language handler.
 */
#define LANGUAGE_HAND "/obj/handlers/languages"
/**
 * The default language inheritable used in all languages so the interface
 * is consistent.
 */
#define LANGUAGE_INHERIT "/std/languages/language_inherit"

/**
 * Flag to specify that this language can be spoken.
 */
#define L_SPOKEN 1
/**
 * Flag to specify that this language can be written.
 */
#define L_WRITTEN 2
/**
 * Flag to specify that this language can be used over a distance (ie:
 * tells and shouts).
 */
#define L_DISTANCE 4
/**
 * Flag to specify that this language is magic language!  We handle these
 * somewhere differently to a normal language.
 */
#define L_MAGIC 8
/**
 * Flag to specify that the language is sized.
 */
#define L_SIZED 16
/**
 * Flag to specify that the language is always able to be spoken.
 */
#define L_ALWAYS_SPEAK 32

/**
 * The first element of the read array, the message itself.
 */ 
#define READ_MESS 0
/**
 * The first element of the read array, the message itself.
 */
#define READ_STR 0
/**
 * The second element of the read array, this is the type.  The type is
 * something like 'red splotched ink' or whatever.  It can be 0.
 */
#define READ_TYPE 1
/**
 * The third element of the read array is the language the message is written
 * in.  The language must exist and must be non-zero.
 */
#define READ_LANG 2
/**
 * The fourth element of the read array is the size of the text.  This is used
 * to make sure not too much stuff can be printed on any one page.
 */
#define READ_SIZE 3

/**
 * This is used to tag the transformation type as a say.
 * This is used inside the language inheritable to tag what type of
 * transformation is being done on the text. 
 */
#define SAY_TYPE "say"
/**
 * This is used to tag the transformation type as a whisper.
 * This is used inside the language inheritable to tag what type of
 * transformation is being done on the text. 
 */
#define WHISPER_TYPE "whisper"
/**
 * This is used to tag the transformation type as a shout.
 * This is used inside the language inheritable to tag what type of
 * transformation is being done on the text. 
 */
#define SHOUT_TYPE "shout"
/**
 * This is used to tag the transformation type as a tell.
 * This is used inside the language inheritable to tag what type of
 * transformation is being done on the text. 
 */
#define TELL_TYPE "tell"

/**
 * The start part of the languages skill tree.
 */
#define LANGUAGE_SKILL_START "other.language."
/**
 * The name of the spoken skill bit.
 */
#define SPOKEN_SKILL "spoken"
/**
 * The name of the written skill bit.
 */
#define WRITTEN_SKILL "written"

/* These are used by the say and lsay commands */
/** There is no text to say.  THis is used by the speech commands. */
#define NO_MESSAGE 0
/** The specific language is not spoken.  This is used by the speech commands. */
#define NOT_SPOKEN 1
/** The specific language is not known by the speaker.  This is used by the speech commands. */
#define NOT_KNOWN 2
/** The specific language is not allowed to be used at a distance.  This is used by the speech commands. */
#define NOT_DISTANCE 3
/** The whole message was just fine.  This is used by the speech commands. */
#define MESSAGE_OK 4
#endif /* __SYS__LANGUAGE */
