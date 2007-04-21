/**
 * This file conatins all the defintions and stuff to handle the player
 * object.
 * @author Pinkfish
 */
#ifndef __SYS__PLAYER
/** @ignore yes */
#define __SYS__PLAYER

#include <playtesters.h>

/** The message to print for not having enough social points. */
#define NO_POWER "Insufficient Social Points.\n"
/** The message to print when you do not have the ability to do something. */
#define NOT_ALLOWED "You do not have that ability yet.\n"
/** The base cost of a shout in social points. */
#define SHOUT_COST 10
/** The base cost of a tell in social points. */
#define TELL_COST 1
/** The base cost of an emote in social points. */
#define EMOTE_COST 1
/** The base cost of a remote in social points. */
#define REMOTE_COST 5
/** The base cost of an echo in social points. */
#define ECHO_COST 1
/** The base cost of an echoto in social points. */
#define ECHOTO_COST 40
/** The base cost of an emoteall in social points. */
#define EMOTEALL_COST 8
/** The base cost of a soul in social points. */
#define SOUL_COST 3
/** The cost of setting a position with the 'position' command */
#define SET_POSITION_COST 100
/** The property which keeps track of if new lines are printed or not. */
#define NO_LINE_PROP "no new line"
/** The standard race used by all players. */
#define RACE_STD "/std/races/human"
/** The maximum amount of idle time for a player. */
#define MAX_IDLE 900
/** The idle time at which they can be kicked off if the mud is full. */
#define FULL_IDLE 300
/**
 * The age in days at which you are considered to be mature.
 * @see /globla/player->query_mature()
 */
#define MATURITY 18

/** The character which is death.  This is the path to the poor thing. */
#define DEATH "/d/underworld/death/chars/death"
/** The book to give to people when they die. */
#define DEAD_BOOK "/obj/misc/dead_book"
/** The shadow to use when your dead, to stop most things from happening. */
#define DEATH_SHADOW "/std/shadows/misc/death_shadow"
/** The object which is offler.  This is used to shut the mud down. */
#define OFFLER "/obj/shut"
/**
 * The shadow offler gives you when your close to shutdown to stop certain
 * things from happening.
 */
#define OFFLER_SHADOW "/std/shadows/misc/offler_shadow"

/**
 * This is the amount of time between the muds automatic saves of the
 * players data.
 */
#define TIME_BETWEEN_SAVES 450
/** The property that is defined on a person when they pass out. */
#define PASSED_OUT_PROP "passed out"

/** The size of the tell history. */
#define MAX_TELL_HIS 20

/** The default amount of time in each round (heart_beat). */
#define ROUND_TIME 40
/** The default amount of time an action will take. */
#define DEFAULT_TIME 20 /* Time for a move or a look. */

/** The speed at which combat occurs, number of attacks a round. */
#define COMBAT_SPEED 2
/** Something neato to do with combat damage. */
#define COMBAT_DAMAGE 2

/**
 * The time at which to start warming people that the person being told is
 * idle.
 */
#define TELL_WARN_TIME 120
/** The maximum size allowed to be set for your description. */
#define MAX_DESC 2048

/** The object which defines all the stuff you get when you start the game. */
#define START_PLAYER "/global/newbie_junk"

/** The property to be used for defining a player killer. */
#define PKER "player_killer"
/** The misspelt property to be used for defining a player killer. */
#define PKER_MISSPELL "player killer"
/** The age you need to be to become a player killer. (2 days). */
#define PKER_MINIMUM_AGE (60 * 60 * 24 * 2)
/**
 * This property (if set on a player) means that they cannot become a
 * player killer.
 */
#define PKER_NOT_ALLOWED "not to be pk"

/** role playing mode property */
#define RPMODE "role_playing"

/** Minimum time between saves using save command. */
#define MIN_SAVE_REPEAT 900

/** The path to the player file. */
#define PLAYER_OB "/global/player"
/** The path to the creator file.  */
#define CREATOR_OB "/global/creator"
/** The path to the lord file.  */
#define LORD_OB "/global/lord"

/** The path to the auto load object. */
#define AUTO_LOAD_OB "/global/auto_load"
/** The missing item receipt path. */
#define PLAYER_RECEIPT "/obj/misc/al_receipt"

/** The unknown birthday type. */
#define UNKNOWN_BIRTHDAY "Unknown"

/** This is where the bodies of dead players are sent. */
#define PLAYER_MORGUE "/room/morgue"

/** This property is set on the player if they wish to allow
 * coloured souls to be received.
 */
#define PLAYER_ALLOW_COLOURED_SOULS "coloured souls"

/** This property is set on the player if they wish to see terrain
 * maps without colour
 */
#define PLAYER_PLAIN_MAPS "plain maps"

/**
 * This is the path to the title handler for the player.  Does all the
 * wonderful stuff to do with player title control.
 */
#define PLAYER_TITLE_HANDLER "/obj/handlers/title_handler"
/**
 * This is the path to the multiplayer handler.
 */
#define PLAYER_MULTIPLAYER_HANDLER "/obj/handlers/multiplayer"

/**
 * This it the maximum length that a friend tag can be.
 */
#define PLAYER_MAX_FRIEND_TAG_LEN 50

/** The property for earmuffs. */
#define PLAYER_EARMUFF_PROP "earmuffs"
/** The value to the earmuff that allows friends through */
#define PLAYER_ALLOW_FRIENDS_EARMUFF 2
/** The value to the earmuff that turns on earmuffs normally. */
#define PLAYER_EARMUFF_ON 1
/** The value to the earmuff that turns on earmuffs normally. */
#define PLAYER_EARMUFF_OFF 0

/** This is the flag to use in the flags variable to test for pk status. */
#define PLAYER_KILLER_FLAG 1

/** @ignore yes
 * Default nationality & region for guests
 */
#define DEFAULT_NATIONALITY "/std/nationality/morpork"
/** The default region for a player to be in. */
#define DEFAULT_REGION "Ankh-Morpork"

/**
 * The property to use for testing to see what errors should be shown
 * to creators.
 */
#define PLAYER_SHOW_ERRORS_PROP "show errors"

/**
 * This property to control how follow is responded to.
 */
#define PLAYER_OPTIONS_CONTROL_PROP "follow control"

#ifdef PLAYER_OPTIONS_CLASS
/**
 * The options control class.  Deals with all the properties of following
 * someone.
 * @member follow_friends allow the person to follow friends
 * @member follow_groups allow the person to follow groups
 * @member follow_everyone allow the person to auto follow everyone
 * @member lead_behind leads from behind
 * @member mxp_toggle the mxp toggle
 * @member extra any extra options
 */
class player_options_control {
   int follow_friends;
   int follow_groups;
   int follow_everyone;
   int lead_behind;
   int mxp_disable;
   mapping extra;
}
#endif

/**
 * THis is the path to the drunk effect.
 */
#define PLAYER_DRUNK_EFFECT "/std/effects/ingested/drunk"

#endif /* __SYS__PLAYER */
