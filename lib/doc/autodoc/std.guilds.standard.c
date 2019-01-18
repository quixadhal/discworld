.DT
standard.c
Disk World autodoc help
standard.c

.SH Description
.SP 5 5

This is where you would set up any shadows you want on the player
object, or just do what you will..
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h, /include/tune.h, /include/skills.h and /include/guilds.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_command%^RESET%^
.EI
.SI 5
int add_command(string name, int add_if_player)
.EI

.SI 3
* %^BOLD%^add_position%^RESET%^
.EI
.SI 5
int add_position(string sub_guild, string position, int max, string * abilities, mixed * description)
.EI
.SP 7 5

Dummy function for guilds without positions
This method adds a new position for the subguild.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - position to add
.EP
.SP 9 5
max - the maximum number of people appointed to this position
.EP
.SP 9 5
abilities - the abilities associated with this position
.EP
.SP 9 5
description - the descriptions associated with this position
({ title|({male_title,female_title}), ... })
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
DEITY_POSITION_OK if successful, DEITY_POSITION_FAIL if the
position already exists
.EP

.SI 3
* %^BOLD%^add_spell%^RESET%^
.EI
.SI 5
int add_spell(string name, mixed ob, string func_name, int add_if_player)
.EI

.SI 3
* %^BOLD%^adjust_primaries%^RESET%^
.EI
.SI 5
string adjust_primaries(object thing, int level)
.EI

.SI 3
* %^BOLD%^appoint_position%^RESET%^
.EI
.SI 5
int appoint_position(string sub_guild, string position, string player_name)
.EI
.SP 7 5

Dummy function for guilds without positions
This method appoints the player to a position.   If sub_guild is 0,
the guild object is supposed to figure out the subguild from the 
player_name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - the position
.EP
.SP 9 5
player_name - the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
POSITION_FAIL if it failed, POSITION_OK if it succeeded.

.EP

.SI 3
* %^BOLD%^cast_spell%^RESET%^
.EI
.SI 5
int cast_spell(string name, mixed bing)
.EI

.SI 3
* %^BOLD%^dismiss_position%^RESET%^
.EI
.SI 5
int dismiss_position(string sub_guild, string position, string player_name)
.EI
.SP 7 5

Dummy function for guilds without positions
This method dismisses the player from a position.   If sub_guild is 0,
the guild object is supposed to figure out the subguild from the 
player_name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - the position
.EP
.SP 9 5
player_name - the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
POSITION_FAIL if it failed, POSITION_OK if it succeeded.

.EP

.SI 3
* %^BOLD%^do_command%^RESET%^
.EI
.SI 5
int do_command(string name, mixed param)
.EI

.SI 3
* %^BOLD%^leave_guild%^RESET%^
.EI
.SI 5
void leave_guild()
.EI

.SI 3
* %^BOLD%^player_heart_beat%^RESET%^
.EI
.SI 5
void player_heart_beat(string guild, object thing)
.EI

.SI 3
* %^BOLD%^player_quit%^RESET%^
.EI
.SI 5
void player_quit()
.EI

.SI 3
* %^BOLD%^player_save%^RESET%^
.EI
.SI 5
void player_save()
.EI

.SI 3
* %^BOLD%^query_commands%^RESET%^
.EI
.SI 5
string * query_commands()
.EI

.SI 3
* %^BOLD%^query_gp_skill%^RESET%^
.EI
.SI 5
string query_gp_skill()
.EI

.SI 3
* %^BOLD%^query_guild_ability%^RESET%^
.EI
.SI 5
int query_guild_ability(mixed thing, string ability)
.EI
.SP 7 5

Dummy function for guilds without positions
Test if a player/npc has a specific ability.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - either a string, being the name of a player, or an object
being the actual player/npc to check for a guild ability.
.EP
.SP 9 5
ability - the ability to check for.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if thing has that ability, 0 if not.

.EP

.SI 3
* %^BOLD%^query_level%^RESET%^
.EI
.SI 5
int query_level(object thing)
.EI
.SP 7 5

This method figures out the level of the npc or player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to find the level of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the level of the thing

.EP

.SI 3
* %^BOLD%^query_position%^RESET%^
.EI
.SI 5
string query_position(string sub_guild, string player_name)
.EI
.SP 7 5

Dummy function for guilds without positions
This method returns the position held by the player. If sub_guild is
0, the guild object is supposed to figure out what sub_guild (if any)
is relevant.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
player_name - the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the position the player is appointed to in sub_guild.

.EP

.SI 3
* %^BOLD%^query_position_abilities%^RESET%^
.EI
.SI 5
string * query_position_abilities(string sub_guild, string position)
.EI
.SP 7 5

Dummy function for guilds without positions
This method returns the abilities for a position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - position to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the position don't exist, an array of abilities
if is does
.EP

.SI 3
* %^BOLD%^query_position_appointments%^RESET%^
.EI
.SI 5
string * query_position_appointments(string sub_guild, string position)
.EI
.SP 7 5

Dummy function for guilds without positions
This method returns the appointments for this position in this 
subguild.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - position to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the position don't exist, an array of appointments
if is does, with 0's for unfilled positions
.EP

.SI 3
* %^BOLD%^query_position_description%^RESET%^
.EI
.SI 5
mixed * query_position_description(string sub_guild, string position)
.EI
.SP 7 5

Dummy function for guilds without positions
This method returns the description array for this position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - position to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the position don't exist, an array of descriptions if 
it does, currently defined to hold the title
({ title|({male_title,female_title}), ... })
.EP

.SI 3
* %^BOLD%^query_positions%^RESET%^
.EI
.SI 5
mapping query_positions(string sub_guild, string position)
.EI
.SP 7 5

Dummy function for guilds without positions
This method returns all positions for this guild.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the position don't exist, an array of appointments
if is does, with 0's for unfilled positions
.EP

.SI 3
* %^BOLD%^query_skill_cost%^RESET%^
.EI
.SI 5
int query_skill_cost(string skill)
.EI

.SI 3
* %^BOLD%^query_skill_max_level%^RESET%^
.EI
.SI 5
int query_skill_max_level(string skill)
.EI

.SI 3
* %^BOLD%^query_skills%^RESET%^
.EI
.SI 5
string * query_skills()
.EI

.SI 3
* %^BOLD%^query_spells%^RESET%^
.EI
.SI 5
mixed * query_spells()
.EI

.SI 3
* %^BOLD%^query_start_pos%^RESET%^
.EI
.SI 5
string query_start_pos(object thing)
.EI

.SI 3
* %^BOLD%^query_title%^RESET%^
.EI
.SI 5
string query_title(object player)
.EI
.SP 7 5

This method returns the title of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to find the title of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the title name

.EP

.SI 3
* %^BOLD%^remove_command%^RESET%^
.EI
.SI 5
int remove_command(string name)
.EI

.SI 3
* %^BOLD%^remove_position%^RESET%^
.EI
.SI 5
int remove_position(string sub_guild, string position)
.EI
.SP 7 5

Dummy function for guilds without positions
This method removes a position for this subguild.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP
.SP 9 5
position - position to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
POSITION_OK if successful, POSITION_FAIL if the
position didn't exist
.EP

.SI 3
* %^BOLD%^remove_spell%^RESET%^
.EI
.SI 5
int remove_spell(string name)
.EI

.SI 3
* %^BOLD%^reset_positions%^RESET%^
.EI
.SI 5
void reset_positions(string sub_guild)
.EI
.SP 7 5

Dummy function for guilds without positions
This method removes all appointments for a guild.   
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sub_guild - the sub guild
.EP

.SI 3
* %^BOLD%^set_gp%^RESET%^
.EI
.SI 5
void set_gp(object thing)
.EI

.SI 3
* %^BOLD%^set_gp_skill%^RESET%^
.EI
.SI 5
void set_gp_skill(string word)
.EI

.SI 3
* %^BOLD%^set_level%^RESET%^
.EI
.SI 5
void set_level(object thing, int level, string guild)
.EI

.SI 3
* %^BOLD%^set_new_level%^RESET%^
.EI
.SI 5
void set_new_level(mixed * args)
.EI

.SI 3
* %^BOLD%^set_skills%^RESET%^
.EI
.SI 5
void set_skills(string * words)
.EI

.SI 3
* %^BOLD%^set_start_pos%^RESET%^
.EI
.SI 5
void set_start_pos(string word)
.EI

.SI 3
* %^BOLD%^skills_advanced%^RESET%^
.EI
.SI 5
void skills_advanced(object thing, string skill, int level)
.EI
.SP 7 5

This method is called whenever a player advances a skill.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the player advacing the skill
.EP
.SP 9 5
skill - the name of the skill
.EP
.SP 9 5
level - the level it is advanced to

.EP

.SI 3
* %^BOLD%^start_player%^RESET%^
.EI
.SI 5
void start_player(object pl)
.EI


