.DT
talker.h
Disk World autodoc help
talker.h

.SH Description
.SP 5 5

The main include file for the talker effect and shadow.  
.EP
.SP 10 5


Written by Taffyd

Started 12/2/1998

.EP

.SH Classes

.SI 3
* %^BOLD%^cache_data%^RESET%^
class cache_data {
int playtester;
int talker_quest;
int apex_member;
}

.EI
.SP 7 5

This class is used to record channel cache data. It is used in conjunction
with a mapping.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
playtester - whether the player is a playtester or not.
.EP
.SP 9 5
apex_member - whether or not the player is an apex member.
.EP
.SP 9 5
talker_quest - whether or not the player has done the talker quest to
gain access to additional channels.

.EP

.SI 3
* %^BOLD%^talker_args%^RESET%^
class talker_args {
int status;
string * channels;
int verbose;
int local_echo;
mixed colour;
}

.EI
.SP 7 5

This class stores the effect arguments for the talker.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
status - whether or not the talker is on or off
.EP
.SP 9 5
what - channels the talker is listening to
.EP
.SP 9 5
verbose - whether the talker is in verbose or brief mode. (1 is verbose, 0 is brief)
.EP
.SP 9 5
local_echo - is local echo enabled
.EP
.SP 9 5
string - the colour that text is displayed on the screen.

.EP


.SH Defines
.SI 3
* %^BOLD%^CHANNEL_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^COLOUR_LIST%^RESET%^
.EI

.SI 3
* %^BOLD%^COLOUR_ON_OFF_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFAULT_COLOUR%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFAULT_COLOUR_SETTINGS%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFAULT_COLOUR_WITH(n)%^RESET%^
.EI

.SI 3
* %^BOLD%^DELETE_CHANNEL_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^ECHO_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^HIST%^RESET%^
.EI

.SI 3
* %^BOLD%^HISTORY_CHANNEL_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^HISTORY_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^LIST_CHANNEL_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^LIST_CHANNEL_PATTERN_BRIEF%^RESET%^
.EI

.SI 3
* %^BOLD%^LIST_FORMAT%^RESET%^
.EI
.SP 7 5

This is a printf format specification field used to format the talker 
list output.
.EP

.SI 3
* %^BOLD%^LIST_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^NEW_CHANNEL_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^ON_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^SET_COLOUR_FULL_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^SET_COLOUR_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^TOGGLE_COLOUR_PATTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^VERBOSE_PATTERN%^RESET%^
.EI

