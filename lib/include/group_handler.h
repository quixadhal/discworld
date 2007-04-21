/**
 * This is the include file for the group system.  It is included
 * by at least the handler, the effect and the commands.         
 *
 * @author Tape
 */ 

#ifndef __GROUP_HANDLER_H__
/** @ignore yes */
#define __GROUP_HANDLER_H__

#include <command.h>

/**
 * This points to the location of the group handler.
 */
#define GROUP           "/obj/handlers/group_handler"

/**
 * This points to the location of the master group command file.
 */
#define GROUP_CMD       ( DIR_PLAYER_CMDS "/group" )
// #define GROUP_CMD       "/w/tape/cmds/ngroup"

/**
 * This defines where the master group command looks for
 * sub commands.
 */
#define GROUP_SUB_CMDS_DIR             (DIR_PLAYER_CMDS "/group_cmds/")
/**
 * This is the wildcard used to find the commands in the directory.
 */
#define GROUP_SUB_CMDS_FILE_WILDCARD   "*.c"

/**
 * This points to the group sub-command inheritable.
 */
#define GROUP_SUBCOMMAND_INHERITABLE   "/cmds/player/group_cmds/base_group"

/**
 * This points to the location of the group effect.
 */
#define EFFECT          "/std/effects/other/group"

/**
 * This points to the location of the group shadow.
 */
#define SHADOW_OB       "/std/shadows/other/group"

/** 
 * Invitations automatically time out after this many seconds.
 */
#define INVITE_TIMEOUT        120

/**
 * These words are automatically filtered out when converting
 * a group's short to its name with short_to_name().  They're
 * not case sensitive.
 */
#define INVALID_WORDS         ({ })

/**
 * This is sticked to the beginning of all group messages.  $channel$
 * is replaced by the group's channel name after making it look pretty.<p>
 *
 * The $colour$ tag is replaced with the custom group colour, if any
 * has been set.<p>
 *
 * Note: the $I$ bit must be there somewhere, unless you feel like
 * rewriting the group history function.  Plus it looks nice, so
 * what the hell.  Mind, it should be in the form "$I$+x,+y="
 */
#define GROUP_NOTIFY_PREFIX   "$I$+5,+0=[$colour$$channel$%^RESET%^] "

/**
 * If shared xp gained is below this number, it isn't shared.
 */
#define MIN_EXPERIENCE        0

/**
 * This defines the maximum number of characters you can have
 * in a group name.
 */
#define MAX_GROUP_NAME_SIZE                  30

/**
 * The maximum number of members a party can contain at a time.
 */
#define MAX_PEOPLE_PER_PARTY                 6

/**
 * The maximum number of invitations that can at one time be
 * pending.
 */
#define MAX_INVITATIONS_PER_PARTY_AT_ONCE    5

/**
 * The maximum number of people you can be assisting at a time.
 */
#define MAX_PEOPLE_YOU_CAN_ASSIST            1

/**
 * Set this to 1 if you want base stats (the ones the player
 * rearranged to) to be used for calculating xp share, instead
 * of the player's current stats.
 */
#define USE_BASE_STATS                       1

/**
 * A member's personal rating:<br>
 *   ( collective primary bonuses / number of bonuses *
 *    PRIMARY_BONUS_MULTIPLIER ) + ( bonus in fighting *
 *    FIGHTING_BONUS_MULTIPLIER )
 *<p>
 * A member's xp share: <br>
 *    Total xp from a kill *
 *    a member's personal rating / group's combined
 *    ratings
 */
#define PRIMARY_BONUS_MULTIPLIER     0.80
#define FIGHTING_BONUS_MULTIPLIER    0.20

/**
 * Path to the contemplate effect/command.
 */
#define CONTEMPLATE ( DIR_GUILD_CMDS "/magic/contemplate" )

/**
 * Name of the title type for a group used by set_title().
 */
#define GROUP_TITLE  "group"

/**
 * This defines how many characters of the group channel name is displayed
 * until it's cut off with three periods (...)  E.g. [groupna...] Tape: Bing.
 * It must be larger than 3, or 0 if no periods should be added.
 */
#define GROUP_CHANNEL_MAX_CHARS  15

/**
 * This defines what the custom colour label for the group is
 * called.
 */
#define GROUP_COLOUR_EVENT   "group"

#endif

