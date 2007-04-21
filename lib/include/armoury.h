/**
 * The include file for all the armoury stuff.
 * @author Pinkfish
 */
#ifndef __SYS__ARMOURY
#define __SYS__ARMOURY

/**
 * The path to the armour.
 */
#define ARMOURY "/obj/handlers/armoury"
/**
 * The property to use for no recycling.
 */
#define ARMOURY_NO_RECYCLING_PROP "no recycling"
/**
 * The property to use for finding the recycling area of the object.
 */
#define ARMOURY_RECYCLING_AREA_PROP "recycling area"

#ifdef UNUSED
/* comment this rubbish out.  Not needed any more I think? */
#define ARM_A_SIZE 15
#define A_OBJ 0
#define A_ANAME 1
#define A_ALIAS 2
#define A_PLURALS 3
#define A_ADJ 4
#define A_SHORT 5
#define A_LONG 6
#define A_WEIGHT 7
#define A_VALUE 8
#define A_COND 9
#define A_CHANCE 10
#define A_ATYPE 11
#define A_DEX_MINUS 12
#define A_ARMOURS 13
#define A_IMMUNE 14
/* weapons junk... */

#define WEAP_A_SIZE 14
#define W_OBJ 0
#define W_NAME 1
#define W_ALIAS 2
#define W_PLURALS 3
#define W_ADJ 4
#define W_SHORT 5
#define W_LONG 6
#define W_WEIGHT 7
#define W_VALUE 8
#define W_COND 9
#define W_DAM_CHANCE 10
#define W_SKILL 11
#define W_ATTACKS 12
#define W_UNDAMAGE 13
#endif

#endif /* __SYS__ARMOURY */
