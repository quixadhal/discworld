#ifndef __SYS__INHUME
#define __SYS__INHUME

// many things in the AM guild use this file directly after
// <morpork.h>, which #defines LOG.
#ifdef LOG
#undef LOG
#endif

#undef DEBUG // Define to start logging skills info to LOGFILE. undefine for normal use.
// #define NOKILL  // For testing purposes -- noone will attack or die if this is defined

#define INHUME "/obj/handlers/secure_handlers/inhume"
#define NO_INHUME_PROP  "cannot inhume"
#define NO_CONTRACT_PROP "cannot place contracts"

// These defines are for the inhume handler...
#define LOGDIR          "/log/secure/assassins/"
#define LOG             ( LOGDIR + "INHUME" )
#define INHUMED         ( LOGDIR + "INHUMED" )
#define RECEIPTS        ( LOGDIR + "INHUME_RECEIPTS" )
#define LOGFILE         ( LOGDIR + "inhume_debug" )
#define PLAY_LOG        ( LOGDIR + "play_inhume_debug" )
#define TM_LOG          ( LOGDIR + "INHUME_TMS" )
#define NPC_LOG         ( LOGDIR + "NPC_CONTRACTS" )

/**
 * This class keeps track of all the contract details.
 * @member status Requested, Validated, Informed, Active, or Special
 * @member bastard The name of the person who requested/paid for it
 * @member value Cost of the contract, in money units (4 units = 1 AM pence)
 * @member time Time progress was last made
 * @member reveal Whether or not the requestor wants their identity revealed
 * @member message Message if any to be passed on to the victim
 * @member name String to be displayed on the receipt as the requestor's name
 * @member place Office where the contract was requested.
 * @member nodeath 1 if no life will be lost when the contract is closed,
 *   0 otherwise. (0 is normal)
 * @member originator The person who originally requested the contract,
 *   if different from the current bastard.
 */

//Size of contract class without location
#define CONTRACT_NO_LOCATION 10 

class contract {
   int status;
   string bastard;
   int value;
   int time;
   int reveal;
   string message;
   string name;
   string place;    //This is for money.
   int nodeath;
   string originator;
   string location; //This is for location!
} /* class contract */

#define EXP_TIME   2419200       // 28 days
#define TAXES      40            // how much they steal in taxes
#define SURCHARGE  10            // service charge for timed-out contracts
#define NOTME_MULT 1.1           // How much extra if you pay for a
                                 // contract someone else requested.
#define MIN_LEVEL  50            // minimum level for a contract
#define PK_MULT    0             // how much extra nonPKs cost
                                 // 0 indicates disallowed
#define COST_MULT  ( 5 * 400 )   // how much per level 400 = 1$AM
#define COST_GUILD ( 6 * 400 )   // Premium rate for contracts on
                                 // guild members.
#define MAXTIMES 4               // how many times can you inhume
                                 // an NPC.
#define NO_DEATH_MULT 0.5        // How much less you pay for a contract
                                 // that doesn't cause the victim to lose
                                 // a life.

/* failure values when validating contracts */
#define LOW          -1
#define NON_KILLER   -2
#define CRE          -3
#define TOO_SOON     -4

/* status values for contracts */
#define REQUESTED 0
#define VALIDATED 1
#define INFORMED 2
#define ACTIVE 3
#define SPECIAL 4

/* type defines for collect classes */
#define INHUME_COLLECT_NONE   0
#define INHUME_COLLECT_REFUND 1
#define INHUME_COLLECT_WAGES  2
#define INHUME_COLLECT_MIXED  3

/* defines for checking whether an assassin can handle PK contracts */
#define INHUME_PK_THRESH_GUILD_LEVEL   175
#define INHUME_PK_THRESH_NUM_CONTRACTS 4
#define INHUME_PK_THRESH_AGE_DAYS      10

/* Rate at which a given player can be inhumed. */
#define REPEAT_RATE ( 60 * 60 * 24 * 7 * 2 ) // 2 weeks

/* Time before an npc will start working on a player contract */
#define NPC_TIME ( 60 * 60 * 24 * 7 * 2 ) // 2 weeks

/* Number of crimes a player has to commit before they become
   inhume-on-sight on Guild grounds */
#define INHUME_ON_SIGHT_THRESH 10

/* How long it takes crime-free to have your crime count drop by one */
#define INHUME_CRIME_EXPIRY_TIME ( 60 * 60 * 24 * 3 )

/* How long you have to wait before another contract can pop up on this NPC */
#define INHUME_NPC_WAIT_TIME ( 60 * 60 * 12 )

#endif /* __SYS__INHUME */
