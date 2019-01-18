.DT
player.c
Disk World autodoc help
player.c

.SH Description
.SP 5 5

The main player object.  Does all the player related stuff.
.EP
.SP 10 5


Written by Pinkfish

Started December 1991

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/spells, /global/log, /global/line_ed, /global/psoul, /global/finger, /global/events, /std/living/living, /global/start_position, /global/guild-race, /global/more_string, /global/family, /global/more_file, /global/path and /global/pweath.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/log.h, /include/nroff.h, /include/refresh.h, /include/config.h, /include/player_handler.h, /include/playerinfo.h, /include/player.h, /include/tune.h, /include/clubs.h, /include/weather.h, /include/playtesters.h, /include/error_handler.h, /include/db.h, /include/board.h, /include/mail.h, /include/group_handler.h, /include/library.h, /include/top_ten_tables.h, /include/command.h, /include/bounty.h, /include/am_time.h, /include/quest_handler.h, /include/drinks.h, /include/skills.h, /include/login_handler.h, /include/login.h and /include/living.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^player_info%^RESET%^
class player_info {
int hb_num;
int level;
int level_time;
int save_inhibit;
int update_tmps_call_out;
int last_save;
object snoopee;
mapping titles;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_deaths%^RESET%^
.EI
.SI 5
void adjust_deaths(int i)
.EI
.SP 7 5

This method changes the number of times the player has died.  This
can only be positive.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the amount of deaths to adjust by, must be +ve
.EP

.SI 3
* %^BOLD%^adjust_max_deaths%^RESET%^
.EI
.SI 5
int adjust_max_deaths(int i)
.EI
.SP 7 5

Changes the current number of maxium deaths.  This adjust
the number of times a player can die before they are totaly dead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the amount of deaths to adjust by, must be +ve

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current total maximum deaths
.EP

.SI 3
* %^BOLD%^allow_save%^RESET%^
.EI
.SI 5
void allow_save()
.EI
.SP 7 5

This method allows the player to save again after they have been
disallowed.
.EP

.SI 3
* %^BOLD%^check_dark%^RESET%^
.EI
.SI 5
int check_dark(int light)
.EI
.SP 7 5

Can the player see?  Checks to see if the player can see at the passed
in light level.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they can see, 0 if they cannot

.EP

.SI 3
* %^BOLD%^disallow_save%^RESET%^
.EI
.SI 5
void disallow_save()
.EI
.SP 7 5

This method stops the player from being able to save.  This is done while
the inventory is regenerating to stop problems with inventory loss due
to logging off before it has finished.
.EP

.SI 3
* %^BOLD%^do_load_auto%^RESET%^
.EI
.SI 5
void do_load_auto()
.EI
.SP 7 5

Starts the autoloading process.  This is called just after a player
logs in to start up the auto loading process.

.EP

.SI 3
* %^BOLD%^do_quit%^RESET%^
.EI
.SI 5
int do_quit()
.EI
.SP 7 5

quits the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
zero if it failed, non zero on success
.EP

.SI 3
* %^BOLD%^examine%^RESET%^
.EI
.SI 5
int examine(string arg)
.EI
.SP 7 5

Forces the player to examine something.  This does the same as the look_me
function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arg - the thing to look at.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on a failure, non zero on success.
.EP

.SI 3
* %^BOLD%^extra_score%^RESET%^
.EI
.SI 5
string extra_score()
.EI
.SP 7 5

This method returns any extra information associated with the score.  This
method will mostly be overridden by shadows which wish to place more
information into the score command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the extra information to place in the score

.EP

.SI 3
* %^BOLD%^glance%^RESET%^
.EI
.SI 5
int glance(string str)
.EI
.SP 7 5

Force the player to glance.  Forces the player to glance in a
certain direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the direction to glance in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 ifg the command failed, non zer if it succeeded

.EP

.SI 3
* %^BOLD%^heart_beat%^RESET%^
.EI
.SI 5
void heart_beat()
.EI
.SP 7 5

The main heart beat function.  This is called by the driver
every 2 seconds on the player.  Does all the maintence
 stuff like fixing up hps and stuff like that.

.EP

.SI 3
* %^BOLD%^idle_out%^RESET%^
.EI
.SI 5
void idle_out()
.EI
.SP 7 5

Checks to see if they have idled out.  This is called from within
the heart beat code.
.EP

.SI 3
* %^BOLD%^look_me%^RESET%^
.EI
.SI 5
int look_me(string str)
.EI
.SP 7 5

Force the player to look.  Forces the player to look at something
or just around.  If the input is set to "" then they look around.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - what to look at
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it failed, non zero on success
.EP

.SI 3
* %^BOLD%^move_player_to_start%^RESET%^
.EI
.SI 5
void move_player_to_start(string bong, int new_flag, string c_name, string ident, int go_invis)
.EI
.SP 7 5

This method is called from within the login code to start up the
new player, set their name and move them into the correct
location on the mud.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bong - the name of the player
.EP
.SP 9 5
new_flag - are the a new player?
.EP
.SP 9 5
c_name - their capital name
.EP
.SP 9 5
ident - the ident from the authorisation code
.EP
.SP 9 5
go_invis - go invsible on login

.EP

.SI 3
* %^BOLD%^net_dead%^RESET%^
.EI
.SI 5
void net_dead()
.EI
.SP 7 5

Called when the player goes net dead.  This is called
by the driver when a player goes net dead.  Turns them into
a statue and stuff.

.EP

.SI 3
* %^BOLD%^query_activity_counter%^RESET%^
.EI
.SI 5
int query_activity_counter()
.EI
.SP 7 5

Return the current value of this players activity_counter
(used for debugging)

.EP

.SI 3
* %^BOLD%^query_cap_name%^RESET%^
.EI
.SI 5
nomask string query_cap_name()
.EI
.SP 7 5

This method returns the current capitalised name of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current capitalised name
.EP

.SI 3
* %^BOLD%^query_creator%^RESET%^
.EI
.SI 5
int query_creator()
.EI
.SP 7 5

This method returns if the object is a creator or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the object is a creator, 0 if not

.EP

.SI 3
* %^BOLD%^query_deaths%^RESET%^
.EI
.SI 5
int query_deaths()
.EI
.SP 7 5

This method returns the number of times the player has died.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of times the player has died
.EP

.SI 3
* %^BOLD%^query_last_log_on%^RESET%^
.EI
.SI 5
int query_last_log_on()
.EI
.SP 7 5

This method returns the time at which the player last logged on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last logged on time

.EP

.SI 3
* %^BOLD%^query_level%^RESET%^
.EI
.SI 5
int query_level()
.EI
.SP 7 5

Returns the players level.  Uses their current guild to determine their
guild level.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
their guild level

.EP

.SI 3
* %^BOLD%^query_mature%^RESET%^
.EI
.SI 5
int query_mature()
.EI
.SP 7 5

This method determins if the player is mature or not yet.  It is based
 on the amount of time on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the player is mature, 0 if they are not

.EP

.SI 3
* %^BOLD%^query_max_deaths%^RESET%^
.EI
.SI 5
int query_max_deaths()
.EI
.SP 7 5

Returns the maxium number of deaths.  This is the maxium number of times they
can die before they are totaly dead.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maxiumm number fo deaths

.EP

.SI 3
* %^BOLD%^query_monitor%^RESET%^
.EI
.SI 5
int query_monitor()
.EI
.SP 7 5

Is rthe monitor turned on?  The hit point monitor which is
displyed during combat...

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is on, 0 if not

.EP

.SI 3
* %^BOLD%^query_no_logins%^RESET%^
.EI
.SI 5
int query_no_logins()
.EI
.SP 7 5

This method returns the number of times the player has logged onto
Discworld.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of logins

.EP

.SI 3
* %^BOLD%^query_object_type%^RESET%^
.EI
.SI 5
string query_object_type(object player)
.EI
.SP 7 5

This method returns the one letter object type which is used in the
finger command to display the type of object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the one letter object type

.EP

.SI 3
* %^BOLD%^query_prevent_shadow%^RESET%^
.EI
.SI 5
int query_prevent_shadow(object ob)
.EI
.SP 7 5

Prevent the object from shadowing us?  This checks to see if we should
prevent this object from shadowing the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we are prventing the shadow

.EP

.SI 3
* %^BOLD%^query_refresh_time%^RESET%^
.EI
.SI 5
int query_refresh_time()
.EI
.SP 7 5

This method returns the time at which the player last refreshed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last refresh time

.EP

.SI 3
* %^BOLD%^query_save_inhibit%^RESET%^
.EI
.SI 5
int query_save_inhibit()
.EI
.SP 7 5

This method returns the value of the disallowing save variable.  If this
is a non-zero value then the player must not be saved.
.EP

.SI 3
* %^BOLD%^query_snoopee%^RESET%^
.EI
.SI 5
nomask object query_snoopee()
.EI
.SP 7 5

Returns the object snooping us.  If someone is snooping the player it
returns the object doing the snooping.  If no one is snooping the player
it returns 0.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object snooping the player
.EP

.SI 3
* %^BOLD%^query_start_time%^RESET%^
.EI
.SI 5
int query_start_time()
.EI
.SP 7 5

This is the date on which the character was started.  The very first
time the player ever logged on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time at which the player first logged on

.EP

.SI 3
* %^BOLD%^query_time_on%^RESET%^
.EI
.SI 5
nomask int query_time_on()
.EI
.SP 7 5

The amount of time on line.  This is the total amount of time online
in seconds from when they first started playing.  The return from
this method is *negative*, you will need to make it positive to use it
most likely.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the total log on time in seconds (the return value is *negative*)

.EP

.SI 3
* %^BOLD%^query_title%^RESET%^
.EI
.SI 5
string query_title()
.EI
.SP 7 5

This method returns the list of titles associated with this player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of titles
.EP

.SI 3
* %^BOLD%^query_titles%^RESET%^
.EI
.SI 5
mapping query_titles()
.EI
.SP 7 5

This method returns the mapping of all the title associated with this
player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current mapping of titles
.EP

.SI 3
* %^BOLD%^quit%^RESET%^
.EI
.SI 5
int quit()
.EI
.SP 7 5

A forced quit.
This is the function called by external objects to quit
a player. It avoids checks for things like being in combat.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it failed, non zero if it succeeded.
.EP

.SI 3
* %^BOLD%^quit_alt%^RESET%^
.EI
.SI 5
int quit_alt(int verbose)
.EI
.SP 7 5

This is the quit code called by the command 'quit'
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if failed
.EP

.SI 3
* %^BOLD%^remove_ghost%^RESET%^
.EI
.SI 5
void remove_ghost()
.EI
.SP 7 5

This method removes the ghost from the player.  This stops them
being a ghost and turns them back into a real person.
.EP

.SI 3
* %^BOLD%^remove_title%^RESET%^
.EI
.SI 5
void remove_title(string type)
.EI
.SP 7 5

This method removes the title of the given type from the title mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of title to remove
.EP

.SI 3
* %^BOLD%^restart_heart_beat%^RESET%^
.EI
.SI 5
int restart_heart_beat()
.EI
.SP 7 5

Restarts their heartbeat if it has got turned off for some reason.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^run_away%^RESET%^
.EI
.SI 5
int run_away()
.EI
.SP 7 5

Called when the player wimpys out of a place.  This does all the run
away stuff anfd things like that.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if succeeded in running away, 0 if not

.EP

.SI 3
* %^BOLD%^save%^RESET%^
.EI
.SI 5
nomask int save()
.EI
.SP 7 5

This method is deprecated.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1
.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

Saves the player.  Saves the player to disc, doing all the autoload
stuff and such like.
.EP

.SI 3
* %^BOLD%^save_with_auto_load%^RESET%^
.EI
.SI 5
void save_with_auto_load(mixed al)
.EI

.SI 3
* %^BOLD%^second_life%^RESET%^
.EI
.SI 5
mixed second_life()
.EI
.SP 7 5

This method is called when the player dies.  It creates the corpse
and handles all the bits and pieces that should be dealth with
when a player dies.


If this method returns 0 then the automatic death handling code in the living object is used instead of this.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^set_monitor%^RESET%^
.EI
.SI 5
void set_monitor(int i)
.EI
.SP 7 5

Set the hit point minitor.  This sets the value of the hit point
monitor flag.  If it is set to 0, the monitor is turned off.  If
it is set to 1 the monitor is turned on, higher values cause the monitor
to be displayed every N heartbeats.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new value for the hit point monitor flag

.EP

.SI 3
* %^BOLD%^set_title%^RESET%^
.EI
.SI 5
void set_title(string type, string title)
.EI
.SP 7 5

This method sets the current title type to be of a certain name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of title to set
.EP
.SP 9 5
title - the string to set it to
.EP

.SI 3
* %^BOLD%^toggle_wimpy%^RESET%^
.EI
.SI 5
int toggle_wimpy(string str)
.EI
.SP 7 5

This method changes the current value of the wimpy variables.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new value of the wimpy variable
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_max_deaths%^RESET%^
.EI
.SI 5
void set_max_deaths(int i)
.EI
.SP 7 5

Sets the maximum number of deaths.  This sets the maximum number of times a player can
die.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the nukber of deaths to set it to
.EP

.SI 3
* %^BOLD%^write_prompt%^RESET%^
.EI
.SI 5
void write_prompt()
.EI


