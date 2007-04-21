/**
 * THis is the header file for the object parser.  It is based on some
 * code by Scatter at Dawn Whispers to achive it's parsing.
 * <pre>
 * File name    : newparser.h
 * Creator      : Scatter
 * Creation date: 2000-04-20
 * </pre>
 *                
 * Revision history:
 * Date        Creator   Desc
 * 2000-04-20  Scatter   File created.
 * 2000-05-20  Pinkfish  Modified for Discworld
 */

#ifndef ___OBJ_PARSER_H
/** @ignore yes */
#define ___OBJ_PARSER_H

#ifndef OBJ_PARSER_NO_CLASSES
/**
 * The class used for object matching returns.
 * @entry result the result of the operation
 * @entry objects the objects which were matched or errored on
 * @entry text the text which matched or was errored on
 */
class obj_match
{
   int result;
   object *objects;
   string text;
}

/**
 * This class used to retain context over various invocations of the parser.
 * @entry ordinal the ordinal number to check, ie: 1st, 2nd
 * @entry number_included used with plurals, ie: 2 frogs
 * @entry fraction the fraction of the object to get ie: ({ 1, 20 })
 * @entry ignore_rest a flag set to say that all the rest of the matches
 * are irrelvant
 * @entry him the object corresponding to the him result
 * @entry her the object corresponding to the her result
 * @entry it the object corresponding to the it result
 * @entry plural the objects matching to the 'them' result
 */
class obj_match_context
{
   int ordinal;
   int number_included;
   int* fraction;
   int ignore_rest;
   int no_ambiguous;

   object him;
   object her;
   object it;
   object* plural;
}
#endif

/** The return result was correctly matched. */
#define OBJ_PARSER_SUCCESS         100
/**
 * There was no match on the string in the text field of the obj_match
 * class.
 */
#define OBJ_PARSER_NO_MATCH        315
/**
 * It is too dark to match on the string in the text field of the obj_match
 * class.
 */
#define OBJ_PARSER_TOO_DARK        316
/**
 * It is not possible to do fractional matches on multiple objects.
 * The objects field has the objects it failed to match on and the text it
 * failed to match on.
 */
#define OBJ_PARSER_FRACTION        317
/**
 * The fraction specified is invalid.  The fraction is in the text field of
 * the class.
 */
#define OBJ_PARSER_BAD_FRACTION    318
/**
 * This is only used inside the player objects.  Means that you cannot do
 * remove player actions from this location.
 */
#define OBJ_PARSER_BAD_ENVIRONMENT 319
/**
 * This is only used inside the player object.  It says that the matched
 * objects were not living.
 */
#define OBJ_PARSER_NOT_LIVING      320
/**
 * THis means an ambiguous match was noticed.  An ambiguous match is
 * where 'bag' matches more than one bag and the player did not force
 * it to recognise a specific one.
 */
#define OBJ_PARSER_AMBIGUOUS       322
/**
 * This means that not enough of the specified object were matched.  For
 * example if someone did '5 coins' and only 3 were matched.
 */
#define OBJ_PARSER_NOT_ENOUGH      323

/** The flag to say that the match is a singular id. */
#define OBJ_PARSER_MATCH_SINGULAR 4
/** The flag to say that the match is a plural id. */
#define OBJ_PARSER_MATCH_PLURAL   16

/**
 * The value of the singular parameter, means it matches everything and 
 * ignores failure results.  These are used as flags.
 */
#define OBJ_PARSER_TYPE_EXISTENCE 1
/** Match players.  These are used as flags. */
#define OBJ_PARSER_TYPE_PLAYER    2
/**
 * Allow things like 1 carrots to match.  These are used as flags.
 */
#define OBJ_PARSER_TYPE_SLOPPY_MATCHING 4
/**
 * Disallow nested matching.  These are used as flags.
 */
#define OBJ_PARSER_TYPE_NO_NESTED 8
/**
 * Only match living things.  These are used as flags.
 */
#define OBJ_PARSER_TYPE_LIVING 16

/* Put in by Ceres in an attempt to make the mud work again! */
#define OBJ_PARSER_TYPE_OBJECT 0

/** The index of the match type in the return array. */
#define OBJ_PARSER_MATCH_TYPE     0
/** The index of the objects in the return array. */
#define OBJ_PARSER_OBJECTS        1

/**
 * This property controls if the player will be informed of ambiguous
 * references or not.
 */
#define OBJ_PARSER_AMBIGUOUS_PROP "no ambiguous"
/**
 * This property controls if the player will be use the 'and' keyword as
 * a break or as a regular character.
 */
#define OBJ_PARSER_USE_AND_AS_BREAK_PROP "use and"

#endif
