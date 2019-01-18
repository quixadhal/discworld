.DT
race.c
Disk World autodoc help
race.c

.SH Description
.SP 5 5

This is called off of the monster to set up a race and
profession...   Race should be first then profession..
.EP
.SP 10 5


Written by Pinkfish

Started Sometime in 1991

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/print_object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/race.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^monster_heart_beat%^RESET%^
.EI
.SI 5
void monster_heart_beat(string race, string guild, mixed race_ob, mixed guild_ob)
.EI
.SP 7 5

This method is called from inside the npc to do the heartbeat on the
guild and race objects.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
race - the race to do a heart beat on
.EP
.SP 9 5
guild - the guild to do a heart beat on
.EP
.SP 9 5
race_ob - the object to use for the race
.EP
.SP 9 5
guild_ob - the object to use for the guild

.EP

.SI 3
* %^BOLD%^query_guild_path%^RESET%^
.EI
.SI 5
string query_guild_path(string guild_name)
.EI
.SP 7 5

This method returns the path associated with the specified guild.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
guild_name - the name of the guild
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the guild

.EP

.SI 3
* %^BOLD%^query_guilds%^RESET%^
.EI
.SI 5
mapping query_guilds()
.EI
.SP 7 5

This method returns all the current guilds in the race object.  It is a
mapping with matches between the name of the guild and guild object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the race mapping

.EP

.SI 3
* %^BOLD%^query_race_path%^RESET%^
.EI
.SI 5
string query_race_path(string race_name)
.EI
.SP 7 5

This method returns the path associated with the specified race.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
race_name - the name of the race
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the race

.EP

.SI 3
* %^BOLD%^query_races%^RESET%^
.EI
.SI 5
mapping query_races()
.EI
.SP 7 5

This method returns all the current races in the race object.  It is a
mapping with matches between the name of the race and race object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the race mapping

.EP

.SI 3
* %^BOLD%^query_valid_race%^RESET%^
.EI
.SI 5
int query_valid_race(string race)
.EI
.SP 7 5

This method checks to see if the specified race is valid or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
race - the race to check for validity
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists, 0 if it does not

.EP

.SI 3
* %^BOLD%^set_level%^RESET%^
.EI
.SI 5
void set_level(int lvl, string race, string guild)
.EI
.SP 7 5

This method sets the level of the npc with the specified attributes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lvl - the level to set the npc at
.EP
.SP 9 5
race - the race to set the npc with
.EP
.SP 9 5
guild - the guild to set the npc with

.EP


