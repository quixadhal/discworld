#ifndef __SYS__TUNE
#define __SYS__TUNE

/* TIME_BETWEEN_HEAL is scaling factor to speed or slow the rate of healing. */

#define TIME_BETWEEN_HEAL 5

/* minimum time between guild changes - obsolete currently*/

#define MIN_TIME 2*60*60*60   

/* tuning for cost of advanncing in guilds.  STD_COST is a base and LEVEL_DIV
   is a gradient factor.  Increasing STD_COST makes skills more expensive and
   decreasing LEVEL_DIV will increase the rate at which the skills increase in
   cost.  LEVEL_DIV of 3 gives a rate of 1/3 */

#define STD_COST 125
#define LEVEL_DIV 3
#define DEFAULT_COST 10
#define COST_DIV 20

/*
 * The time_div define is how many xp per time unit is used in teaching 
 * skills.  This affects how long it takes to learn a skill.
 */
#define TIME_DIV 300

/*
 * Er, stuff to do with broadcast
 */
#define MAX_AL 10000
#define SHOUT_DISTANCE 1000

#endif /* __SYS__TUNE */
