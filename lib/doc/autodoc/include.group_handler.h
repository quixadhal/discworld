.DT
group_handler.h
Disk World autodoc help
group_handler.h

.SH Description
.SP 5 5

This is the include file for the group system.  It is included
by at least the handler, the effect and the commands.         

.EP
.SP 10 5


Written by Tape

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/command.h.
.EP

.SH Defines
.SI 3
* %^BOLD%^CONTEMPLATE%^RESET%^
.EI
.SP 7 5

Path to the contemplate effect/command.

.EP

.SI 3
* %^BOLD%^EFFECT%^RESET%^
.EI
.SP 7 5

This points to the location of the group effect.

.EP

.SI 3
* %^BOLD%^FIGHTING_BONUS_MULTIPLIER%^RESET%^
.EI

.SI 3
* %^BOLD%^GROUP%^RESET%^
.EI
.SP 7 5

This points to the location of the group handler.

.EP

.SI 3
* %^BOLD%^GROUP_CHANNEL_MAX_CHARS%^RESET%^
.EI
.SP 7 5

This defines how many characters of the group channel name is displayed
until it's cut off with three periods (...)  E.g. [groupna...] Tape: Bing.
It must be larger than 3, or 0 if no periods should be added.

.EP

.SI 3
* %^BOLD%^GROUP_CMD%^RESET%^
.EI
.SP 7 5

This points to the location of the master group command file.

.EP

.SI 3
* %^BOLD%^GROUP_COLOUR_EVENT%^RESET%^
.EI
.SP 7 5

This defines what the custom colour label for the group is
called.

.EP

.SI 3
* %^BOLD%^GROUP_NOTIFY_PREFIX%^RESET%^
.EI
.SP 7 5

This is sticked to the beginning of all group messages.  $channel$
is replaced by the group's channel name after making it look pretty.

The $colour$ tag is replaced with the custom group colour, if any has been set.

Note: the $I$ bit must be there somewhere, unless you feel like rewriting the group history function. Plus it looks nice, so what the hell. Mind, it should be in the form "$I$+x,+y=" 
.EP

.SI 3
* %^BOLD%^GROUP_SUB_CMDS_DIR%^RESET%^
.EI
.SP 7 5

This defines where the master group command looks for
sub commands.

.EP

.SI 3
* %^BOLD%^GROUP_SUB_CMDS_FILE_WILDCARD%^RESET%^
.EI
.SP 7 5

This is the wildcard used to find the commands in the directory.

.EP

.SI 3
* %^BOLD%^GROUP_SUBCOMMAND_INHERITABLE%^RESET%^
.EI
.SP 7 5

This points to the group sub-command inheritable.

.EP

.SI 3
* %^BOLD%^GROUP_TITLE%^RESET%^
.EI
.SP 7 5

Name of the title type for a group used by set_title().

.EP

.SI 3
* %^BOLD%^INVALID_WORDS%^RESET%^
.EI
.SP 7 5

These words are automatically filtered out when converting
a group's short to its name with short_to_name().  They're
not case sensitive.

.EP

.SI 3
* %^BOLD%^INVITE_TIMEOUT%^RESET%^
.EI
.SP 7 5

Invitations automatically time out after this many seconds.

.EP

.SI 3
* %^BOLD%^MAX_GROUP_NAME_SIZE%^RESET%^
.EI
.SP 7 5

This defines the maximum number of characters you can have
in a group name.

.EP

.SI 3
* %^BOLD%^MAX_INVITATIONS_PER_PARTY_AT_ONCE%^RESET%^
.EI
.SP 7 5

The maximum number of invitations that can at one time be
pending.

.EP

.SI 3
* %^BOLD%^MAX_PEOPLE_PER_PARTY%^RESET%^
.EI
.SP 7 5

The maximum number of members a party can contain at a time.

.EP

.SI 3
* %^BOLD%^MAX_PEOPLE_YOU_CAN_ASSIST%^RESET%^
.EI
.SP 7 5

The maximum number of people you can be assisting at a time.

.EP

.SI 3
* %^BOLD%^MIN_EXPERIENCE%^RESET%^
.EI
.SP 7 5

If shared xp gained is below this number, it isn't shared.

.EP

.SI 3
* %^BOLD%^PRIMARY_BONUS_MULTIPLIER%^RESET%^
.EI
.SP 7 5

A member's personal rating:
.EP
.SP 7 5
( collective primary bonuses / number of bonuses * PRIMARY_BONUS_MULTIPLIER ) + ( bonus in fighting * FIGHTING_BONUS_MULTIPLIER ) 

A member's xp share: 
.EP
.SP 7 5
Total xp from a kill * a member's personal rating / group's combined ratings 
.EP

.SI 3
* %^BOLD%^SHADOW_OB%^RESET%^
.EI
.SP 7 5

This points to the location of the group shadow.

.EP

.SI 3
* %^BOLD%^USE_BASE_STATS%^RESET%^
.EI
.SP 7 5

Set this to 1 if you want base stats (the ones the player
rearranged to) to be used for calculating xp share, instead
of the player's current stats.

.EP

