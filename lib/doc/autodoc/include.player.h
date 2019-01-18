.DT
player.h
Disk World autodoc help
player.h

.SH Description
.SP 5 5

This file conatins all the defintions and stuff to handle the player
object.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^player_options_control%^RESET%^
class player_options_control {
int follow_friends;
int follow_groups;
int follow_everyone;
int lead_behind;
int mxp_disable;
mapping extra;
}

.EI
.SP 7 5

The options control class.  Deals with all the properties of following
someone.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
follow_friends - allow the person to follow friends
.EP
.SP 9 5
follow_groups - allow the person to follow groups
.EP
.SP 9 5
follow_everyone - allow the person to auto follow everyone
.EP
.SP 9 5
lead_behind - leads from behind
.EP
.SP 9 5
mxp_toggle - the mxp toggle
.EP
.SP 9 5
extra - any extra options

.EP


.SH Defines
.SI 3
* %^BOLD%^AUTO_LOAD_OB%^RESET%^
.EI
.SP 7 5
The path to the auto load object. 
.EP

.SI 3
* %^BOLD%^COMBAT_DAMAGE%^RESET%^
.EI
.SP 7 5
Something neato to do with combat damage. 
.EP

.SI 3
* %^BOLD%^COMBAT_SPEED%^RESET%^
.EI
.SP 7 5
The speed at which combat occurs, number of attacks a round. 
.EP

.SI 3
* %^BOLD%^CREATOR_OB%^RESET%^
.EI
.SP 7 5
The path to the creator file.  
.EP

.SI 3
* %^BOLD%^DEAD_BOOK%^RESET%^
.EI
.SP 7 5
The book to give to people when they die. 
.EP

.SI 3
* %^BOLD%^DEATH%^RESET%^
.EI
.SP 7 5
The character which is death.  This is the path to the poor thing. 
.EP

.SI 3
* %^BOLD%^DEATH_SHADOW%^RESET%^
.EI
.SP 7 5
The shadow to use when your dead, to stop most things from happening. 
.EP

.SI 3
* %^BOLD%^DEFAULT_REGION%^RESET%^
.EI
.SP 7 5
The default region for a player to be in. 
.EP

.SI 3
* %^BOLD%^DEFAULT_TIME%^RESET%^
.EI
.SP 7 5
The default amount of time an action will take. 
.EP

.SI 3
* %^BOLD%^ECHO_COST%^RESET%^
.EI
.SP 7 5
The base cost of an echo in social points. 
.EP

.SI 3
* %^BOLD%^ECHOTO_COST%^RESET%^
.EI
.SP 7 5
The base cost of an echoto in social points. 
.EP

.SI 3
* %^BOLD%^EMOTE_COST%^RESET%^
.EI
.SP 7 5
The base cost of an emote in social points. 
.EP

.SI 3
* %^BOLD%^EMOTEALL_COST%^RESET%^
.EI
.SP 7 5
The base cost of an emoteall in social points. 
.EP

.SI 3
* %^BOLD%^FULL_IDLE%^RESET%^
.EI
.SP 7 5
The idle time at which they can be kicked off if the mud is full. 
.EP

.SI 3
* %^BOLD%^LORD_OB%^RESET%^
.EI
.SP 7 5
The path to the lord file.  
.EP

.SI 3
* %^BOLD%^MATURITY%^RESET%^
.EI
.SP 7 5

The age in days at which you are considered to be mature.
.EP

.SI 3
* %^BOLD%^MAX_DESC%^RESET%^
.EI
.SP 7 5
The maximum size allowed to be set for your description. 
.EP

.SI 3
* %^BOLD%^MAX_IDLE%^RESET%^
.EI
.SP 7 5
The maximum amount of idle time for a player. 
.EP

.SI 3
* %^BOLD%^MAX_TELL_HIS%^RESET%^
.EI
.SP 7 5
The size of the tell history. 
.EP

.SI 3
* %^BOLD%^MIN_SAVE_REPEAT%^RESET%^
.EI
.SP 7 5
Minimum time between saves using save command. 
.EP

.SI 3
* %^BOLD%^NO_LINE_PROP%^RESET%^
.EI
.SP 7 5
The property which keeps track of if new lines are printed or not. 
.EP

.SI 3
* %^BOLD%^NO_POWER%^RESET%^
.EI
.SP 7 5
The message to print for not having enough social points. 
.EP

.SI 3
* %^BOLD%^NOT_ALLOWED%^RESET%^
.EI
.SP 7 5
The message to print when you do not have the ability to do something. 
.EP

.SI 3
* %^BOLD%^OFFLER%^RESET%^
.EI
.SP 7 5
The object which is offler.  This is used to shut the mud down. 
.EP

.SI 3
* %^BOLD%^OFFLER_SHADOW%^RESET%^
.EI
.SP 7 5

The shadow offler gives you when your close to shutdown to stop certain
things from happening.

.EP

.SI 3
* %^BOLD%^PASSED_OUT_PROP%^RESET%^
.EI
.SP 7 5
The property that is defined on a person when they pass out. 
.EP

.SI 3
* %^BOLD%^PKER%^RESET%^
.EI
.SP 7 5
The property to be used for defining a player killer. 
.EP

.SI 3
* %^BOLD%^PKER_MINIMUM_AGE%^RESET%^
.EI
.SP 7 5
The age you need to be to become a player killer. (2 days). 
.EP

.SI 3
* %^BOLD%^PKER_MISSPELL%^RESET%^
.EI
.SP 7 5
The misspelt property to be used for defining a player killer. 
.EP

.SI 3
* %^BOLD%^PKER_NOT_ALLOWED%^RESET%^
.EI
.SP 7 5

This property (if set on a player) means that they cannot become a
player killer.

.EP

.SI 3
* %^BOLD%^PLAYER_ALLOW_COLOURED_SOULS%^RESET%^
.EI
.SP 7 5
This property is set on the player if they wish to allow
coloured souls to be received.

.EP

.SI 3
* %^BOLD%^PLAYER_ALLOW_FRIENDS_EARMUFF%^RESET%^
.EI
.SP 7 5
The value to the earmuff that allows friends through 
.EP

.SI 3
* %^BOLD%^PLAYER_DRUNK_EFFECT%^RESET%^
.EI
.SP 7 5

THis is the path to the drunk effect.

.EP

.SI 3
* %^BOLD%^PLAYER_EARMUFF_OFF%^RESET%^
.EI
.SP 7 5
The value to the earmuff that turns on earmuffs normally. 
.EP

.SI 3
* %^BOLD%^PLAYER_EARMUFF_ON%^RESET%^
.EI
.SP 7 5
The value to the earmuff that turns on earmuffs normally. 
.EP

.SI 3
* %^BOLD%^PLAYER_EARMUFF_PROP%^RESET%^
.EI
.SP 7 5
The property for earmuffs. 
.EP

.SI 3
* %^BOLD%^PLAYER_KILLER_FLAG%^RESET%^
.EI
.SP 7 5
This is the flag to use in the flags variable to test for pk status. 
.EP

.SI 3
* %^BOLD%^PLAYER_MAX_FRIEND_TAG_LEN%^RESET%^
.EI
.SP 7 5

This it the maximum length that a friend tag can be.

.EP

.SI 3
* %^BOLD%^PLAYER_MORGUE%^RESET%^
.EI
.SP 7 5
This is where the bodies of dead players are sent. 
.EP

.SI 3
* %^BOLD%^PLAYER_MULTIPLAYER_HANDLER%^RESET%^
.EI
.SP 7 5

This is the path to the multiplayer handler.

.EP

.SI 3
* %^BOLD%^PLAYER_OB%^RESET%^
.EI
.SP 7 5
The path to the player file. 
.EP

.SI 3
* %^BOLD%^PLAYER_OPTIONS_CONTROL_PROP%^RESET%^
.EI
.SP 7 5

This property to control how follow is responded to.

.EP

.SI 3
* %^BOLD%^PLAYER_PLAIN_MAPS%^RESET%^
.EI
.SP 7 5
This property is set on the player if they wish to see terrain
maps without colour

.EP

.SI 3
* %^BOLD%^PLAYER_RECEIPT%^RESET%^
.EI
.SP 7 5
The missing item receipt path. 
.EP

.SI 3
* %^BOLD%^PLAYER_SHOW_ERRORS_PROP%^RESET%^
.EI
.SP 7 5

The property to use for testing to see what errors should be shown
to creators.

.EP

.SI 3
* %^BOLD%^PLAYER_TITLE_HANDLER%^RESET%^
.EI
.SP 7 5

This is the path to the title handler for the player.  Does all the
wonderful stuff to do with player title control.

.EP

.SI 3
* %^BOLD%^RACE_STD%^RESET%^
.EI
.SP 7 5
The standard race used by all players. 
.EP

.SI 3
* %^BOLD%^REMOTE_COST%^RESET%^
.EI
.SP 7 5
The base cost of a remote in social points. 
.EP

.SI 3
* %^BOLD%^ROUND_TIME%^RESET%^
.EI
.SP 7 5
The default amount of time in each round (heart_beat). 
.EP

.SI 3
* %^BOLD%^RPMODE%^RESET%^
.EI
.SP 7 5
role playing mode property 
.EP

.SI 3
* %^BOLD%^SET_POSITION_COST%^RESET%^
.EI
.SP 7 5
The cost of setting a position with the 'position' command 
.EP

.SI 3
* %^BOLD%^SHOUT_COST%^RESET%^
.EI
.SP 7 5
The base cost of a shout in social points. 
.EP

.SI 3
* %^BOLD%^SOUL_COST%^RESET%^
.EI
.SP 7 5
The base cost of a soul in social points. 
.EP

.SI 3
* %^BOLD%^START_PLAYER%^RESET%^
.EI
.SP 7 5
The object which defines all the stuff you get when you start the game. 
.EP

.SI 3
* %^BOLD%^TELL_COST%^RESET%^
.EI
.SP 7 5
The base cost of a tell in social points. 
.EP

.SI 3
* %^BOLD%^TELL_WARN_TIME%^RESET%^
.EI
.SP 7 5

The time at which to start warming people that the person being told is
idle.

.EP

.SI 3
* %^BOLD%^TIME_BETWEEN_SAVES%^RESET%^
.EI
.SP 7 5

This is the amount of time between the muds automatic saves of the
players data.

.EP

.SI 3
* %^BOLD%^UNKNOWN_BIRTHDAY%^RESET%^
.EI
.SP 7 5
The unknown birthday type. 
.EP

