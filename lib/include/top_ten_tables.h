/**
 * The defines for the top ten tables.
 * @author Deutha
 */
#ifndef __TOP_TEN_TABLES_H
/** @ignore yes */
#define __TOP_TEN_TABLES_H

/**
 * The path to the top ten handler.
 */
#define TOP_TEN_HANDLER "/obj/handlers/top_ten_handler"

/**
 * The index of the name of the player in the returned array.
 */
#define TOP_TEN_NAME 0
/**
 * The index of the name of the player's rating in the returned array.
 */
#define TOP_TEN_RATING 1
/**
 * The index of the name of the player's guild level in the returned array.
 */
#define TOP_TEN_LEVEL 2
/**
 * The index of the name of the player's age in the returned array.
 */
#define TOP_TEN_AGE 3

/**
 * The index of the family with the most members.
 */
#define TOP_TEN_LARGEST_FAMILY    0
/**
 * The index of the family with the highest average age.
 */
#define TOP_TEN_OLDEST_FAMILY     1
/**
 * The index of the family with the highest average quest points.
 */
#define TOP_TEN_MOST_QUEST_POINTS 2
/**
 * The index of the guilds mapping for the most of each guild.
 */
#define TOP_TEN_MOST_GUILD        3
/**
 * The index of the family with the most pks.
 */
#define TOP_TEN_MOST_PKS          4
/**
 * The index of the largest single gender family.
 */
#define TOP_TEN_SINGLE_GENDER     5
/**
 * The index of the family that has the oldest logged on players.
 */
#define TOP_TEN_OLDEST_LOGONS     6
/**
 * The index of the family that has the msot relationships per person.
 */
#define TOP_TEN_MOST_RELATIONSHIPS 7

class top_ten_family_guild {
   int num;
   string top_family_name;
}

#endif
