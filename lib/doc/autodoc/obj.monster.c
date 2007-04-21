.DT
monster.c
DW_wibble autodoc help
monster.c

.SH Description
.SP 5 5

This file contains all the information relevant to creating an
npc (or monster).
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/guild-race, /global/events, /std/living/mon_actions, /std/living/response_mon and /std/living/living.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/route.h, /include/playtesters.h, /include/skills.h, /include/move_failures.h, /include/top_ten_tables.h, /include/map.h, /include/combat.h, /include/player.h, /include/wander.h, /include/monster.h, /include/living.h and /include/config.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_achat_string%^RESET%^
.EI
.SI 5
void add_achat_string(mixed weight, mixed chat)
.EI
.SP 7 5

This method adds a single chat string into the current list of
attack message chat strings.  See load_chat() for a longer description of
the chat string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
weight - the weight of the chat
.EP
.SP 9 5
chat - the new chat string
.EP

.SI 3
* %^BOLD%^add_chat_string%^RESET%^
.EI
.SI 5
void add_chat_string(mixed weight, mixed chat)
.EI
.SP 7 5

This method adds a single chat string into the current list of
chat strings.  See load_chat() for a longer description of
the chat string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
weight - the weight of the chat
.EP
.SP 9 5
chat - the new chat string
.EP

.SI 3
* %^BOLD%^add_combat_action%^RESET%^
.EI
.SI 5
void add_combat_action(int chance, string name, mixed action)
.EI
.SP 7 5

This method adds a combat action to the npc.  This is an action which
has a chance of occuring during combat.  The name is an identifier
which can be used to remove the action with later.  The action
itself can be a string, then that command will be executed.  If
the action is a function pointer then it will be evaluated with
two arguments, the first being the attacker, the second being the
target.


If the action is an array, if it is one element then the function specified will be called on the attacked with the same arguements as above. If the size of the array is two then the function will be called on the specified object with the arguments as above.
.EP

.SI 3
* %^BOLD%^add_enter_commands%^RESET%^
.EI
.SI 5
int add_enter_commands(mixed str)
.EI
.SP 7 5

This method adds a command to be called whenever the npc enters
a room.  If the command is a string, then it will be executed
as if they had typed it.  If it is a function then the function
will be evaluated and one argument (the npc itself) will be passed
in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the enter commands to add
.EP

.SI 3
* %^BOLD%^add_language%^RESET%^
.EI
.SI 5
void add_language(string str)
.EI
.SP 7 5

This method adds a language to the npc. 


After the sun has died away
.EP
.SP 7 5
The stars come out and glow
.EP
.SP 7 5
Lighting the embers of good intentions
.EP
.SP 7 5
Ghostly white, unhappily bright
.EP
.SP 7 5
Time lost, the day done
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language to add

.EP

.SI 3
* %^BOLD%^add_move_zone%^RESET%^
.EI
.SI 5
void add_move_zone(mixed zone)
.EI
.SP 7 5

This method adds a move zone onto the npc.  The move zones control
which areas the npcs will wander into, a move zone is set on the
room and the npcs will only enter rooms which have a matching
move zone.  If there is no move zone, then the npc will enter
any room.


If the parameter is an array each of the elements of the array will be added as a move zone.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone(s) to add
.EP

.SI 3
* %^BOLD%^add_spell_action%^RESET%^
.EI
.SI 5
void add_spell_action(string spell_object, int chance, string name, mixed action)
.EI
.SP 7 5

This method adds an action to the npc that will happen if a specified
spell is cast.  This is an action which
has a chance of occuring when a spell is being cast.
The name is an identifier
which can be used to remove the action with later.  The action
itself can be a string, then that command will be executed.  If
the action is a function pointer then it will be evaluated with
two arguments, the first being the caster, the second being the
target(s) array and the third being the magic arguments class.


If the action is an array, if it is one element then the function specified will be called on the attacked with the same arguements as above. If the size of the array is two then the function will be called on the specified object with an extra first argument being the npc which the effect is being called from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
spell_object - the spell to respond to
.EP
.SP 9 5
chance - the chance of it working
.EP
.SP 9 5
name - the name of the thing
.EP
.SP 9 5
action - the action to preform

.EP

.SI 3
* %^BOLD%^attack_permission%^RESET%^
.EI
.SI 5
int attack_permission(object ob1, object ob2, string stringy)
.EI
.SP 7 5


attack_permission function, added for use by the allow_attack simul.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
object - The person performing the action
.EP
.SP 9 5
object - The object being acted on, this object usually.
.EP
.SP 9 5
string - Attack type, this will be one of "combat", "theft", or "magic",
this lets you give your NPCs different responses for different attacks. As well
as make them immune to theft and magic (as an example)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int 1 if the action is denied, 0 is it can go through.
.EP

.SI 3
* %^BOLD%^basic_setup%^RESET%^
.EI
.SI 5
void basic_setup(string race, string guild, int level)
.EI
.SP 7 5

This method sets up the basic abilities and race of the critter.  It
is equivalent to calling set_race(), set_guild(), and then set_level()
with the same parameters.  But those latter three functions are
deprecated and shouldn't be used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
race - this is the race of the character.  It should be a
race that's understood by the /std/race.c
.EP
.SP 9 5
guild - this is the guild, class, or profession of the NPC.
.EP
.SP 9 5
level - this is the base skill level of the NPC.  The
number is used by the race object to set ability scores, and
base skills.

.EP

.SI 3
* %^BOLD%^check_anyone_here%^RESET%^
.EI
.SI 5
int check_anyone_here()
.EI
.SP 7 5

This method checks to see if there are any players in the environment
of the npc.  This should be used to determine when chats should
be turned off and other things which should only work in the
presence of players.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is a player in the room, 0 otherwise

.EP

.SI 3
* %^BOLD%^cleaning_room%^RESET%^
.EI
.SI 5
int cleaning_room()
.EI

.SI 3
* %^BOLD%^combat_actions_call_back%^RESET%^
.EI
.SI 5
void combat_actions_call_back(object player, object target)
.EI
.SP 7 5

This is the call back from the combat effect to do something
wonderful and wild.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player
.EP
.SP 9 5
target - the target

.EP

.SI 3
* %^BOLD%^command_override%^RESET%^
.EI
.SI 5
void command_override(function func)
.EI
.SP 7 5

Use this function to set a function that is called with the NPCs input
before command() gets it, return 1 from the function if the
input needs no further parsing (ie the command is handled)

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - = function in the players environment to call.

.EP

.SI 3
* %^BOLD%^delay_command%^RESET%^
.EI
.SI 5
int delay_command(string words, int interval)
.EI
.SP 7 5

This method allows you to control the npc and get it to do
actions where they are queued as for players.   The command
is always delayed by delay even if there are no commands pending
unlike queue_command(). This function
is 100% compatible with queue_command() and init_command().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the action to perform
.EP
.SP 9 5
interval - to wait before the command.
.EP

.SI 3
* %^BOLD%^delete_queued_commands%^RESET%^
.EI
.SI 5
void delete_queued_commands()
.EI
.SP 7 5

This method throws away any queued commands.
It doesn't remove the call_out however if no
new commands are added there will be no effect.
.EP

.SI 3
* %^BOLD%^do_combat_action%^RESET%^
.EI
.SI 5
void do_combat_action(object player, object target, mixed action)
.EI
.SP 7 5

This method does a combat action.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
target - the target of the action
.EP

.SI 3
* %^BOLD%^do_command%^RESET%^
.EI
.SI 5
int do_command(string words)
.EI
.SP 7 5

This method allows you to control the npc and get it to do
actions.  This can be used for npc control and inteligence.

Be very careful with this command! This does not go through any
command queue like players have and so NPCs can end up doing
tons of commands in very short order.

If you're trying to make your NPC act like a player use
eue_command() instead

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the action to preform
.EP

.SI 3
* %^BOLD%^do_follow_move%^RESET%^
.EI
.SI 5
void do_follow_move(string dir)
.EI
.SP 7 5

This method is used to make the npcs follow after attackers when they
leave the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the dirction to follow them in

.EP

.SI 3
* %^BOLD%^do_move%^RESET%^
.EI
.SI 5
void do_move(string move)
.EI
.SP 7 5

This method causes the npc to move in the given direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
move - the direction to move

.EP

.SI 3
* %^BOLD%^do_move_after%^RESET%^
.EI
.SI 5
void do_move_after(int running_away)
.EI
.SP 7 5

This is called when the npc decides it must continue down
a certain route.  This will be called by the wander handler
and can be used to force the npc to wander along a route
faster.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
running_away - this is 1 if the npc is running away
.EP

.SI 3
* %^BOLD%^do_route_move%^RESET%^
.EI
.SI 5
void do_route_move()
.EI
.SP 7 5

This method moves the npc one more location along the route it
is following.
.EP

.SI 3
* %^BOLD%^do_spell_action%^RESET%^
.EI
.SI 5
void do_spell_action(object caster, object * targets, mixed action, mixed args)
.EI
.SP 7 5

This method does a combat action.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caster - the caster of the spell
.EP
.SP 9 5
targets - the targets of the spell
.EP
.SP 9 5
action - the action to do
.EP
.SP 9 5
args - the spell arguements

.EP

.SI 3
* %^BOLD%^drunk_check%^RESET%^
.EI
.SI 5
int drunk_check(string str)
.EI

.SI 3
* %^BOLD%^event_fight_in_progress%^RESET%^
.EI
.SI 5
void event_fight_in_progress(object me, object him)
.EI
.SP 7 5

This event is called when a fight is in progress.  It will
be used for things like joining into currently running
fights and initiating combat with spell casters.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
me - the person initiating the attack
.EP
.SP 9 5
him - the person being attacked

.EP

.SI 3
* %^BOLD%^event_npc_ritual_casting%^RESET%^
.EI
.SI 5
void event_npc_ritual_casting(object ritual, object caster, object * targets, class spell_argument args)
.EI
.SP 7 5

This is the event called when a ritual is being cast.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
spell_ob - the ritual
.EP
.SP 9 5
caster - the caster of the ritual
.EP
.SP 9 5
targets - the targets of the ritual
.EP
.SP 9 5
args - the ritual arguements

.EP

.SI 3
* %^BOLD%^event_npc_spell_casting%^RESET%^
.EI
.SI 5
void event_npc_spell_casting(object spell, object caster, object * targets, class spell_argument args)
.EI
.SP 7 5

This is the event called when a spell is being cast.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
spell_ob - the spell object
.EP
.SP 9 5
caster - the caster of the spell
.EP
.SP 9 5
targets - the targets of the spell
.EP
.SP 9 5
args - the spell arguements

.EP

.SI 3
* %^BOLD%^expand_mon_string%^RESET%^
.EI
.SI 5
void expand_mon_string(mixed str)
.EI
.SP 7 5

This method executes the string passed in.  It handles all the
stuff which is needed from the chat_string stuff.

If the input is a function pointer then it is evaluated with one
parameter, being the npc.

If the input is a string then the first letter determines what will
be done with it.  All these are passed through expand_string
so that exciting things can be done.

.EP
.SO 8 2 -12

	*	# - A call_other will be generated. The parameters are separated by ':'s, so "#frog:bing:fred:chicken" would call
.EO
.SO 8 2 -12
		this_object()->bing("fred", "chicken");
.EO
.SO 8 2 -12
		. ', ", : - These will generate a 'say', 'lsay' or 'emote'. @ - This will run the passed in command. Eg: "@frog" would cause the soul command frog to be used. Anything else will be used as a message to be sent to everyone in the room. 
.EO
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the thing to execute
.EP

.SI 3
* %^BOLD%^expand_string%^RESET%^
.EI
.SI 5
string expand_string(string in_str, object on)
.EI
.SP 7 5

This method is used to expand the message strings used in the
npc messages.  It is used for chat strings and such things like
that.  The strings it expands are of the form:
.EP
.SP 7 5
$lname$, $mname$, $aname$, ...
.EP
.SP 7 5
The first letter determines the type of object being referenced. They are: 
.EP
.SP 7 5
m 
.EP
.SP 10 5
Me! The npc itself. 
.EP
.SP 7 5
l 
.EP
.SP 7 5
A living object, choose a random living object in the npcs environment. 
.EP
.SP 7 5
a 
.EP
.SP 10 5
Chooses a random attacker from those attacking the npc. 
.EP
.SP 7 5
o 
.EP
.SP 10 5
Choose a random object in the inventory of the npc. 
.EP
.SP 7 5
After the first letter is a type of information being request. 
.EP
.SP 10 5
name 
.EP
.SP 13 5
The name of the selected object. 
.EP
.SP 10 5
cname 
.EP
.SP 13 5
The capitalised name of the selected object. 
.EP
.SP 10 5
gender 
.EP
.SP 13 5
The gender string of the selected object (male, female, neuter). 
.EP
.SP 10 5
poss 
.EP
.SP 13 5
The possessive string of the selected object. 
.EP
.SP 10 5
obj 
.EP
.SP 13 5
The objective string of the selected object. 
.EP
.SP 10 5
pronoun 
.EP
.SP 13 5
The pronoun string of the selected object. 
.EP
.SP 10 5
gtitle 
.EP
.SP 13 5
The guild title of the selected object (only useful on livings). 
.EP
.SP 10 5
ashort 
.EP
.SP 13 5
The a_short() call. 
.EP
.SP 10 5
possshort 
.EP
.SP 13 5
The poss_short() call. 
.EP
.SP 10 5
theshort 
.EP
.SP 13 5
The the_short() call. 
.EP
.SP 10 5
oneshort 
.EP
.SP 13 5
The one_short() call. 
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
in_str - the input string
.EP
.SP 9 5
on - the object to use for the 'o' matching

.EP

.SI 3
* %^BOLD%^get_next_route_direction%^RESET%^
.EI
.SI 5
string get_next_route_direction()
.EI
.SP 7 5

This method gets the next direction to go in the route which is
currently being followed.  It will remove this direction off the
array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the next direction to go down
.EP

.SI 3
* %^BOLD%^give_money%^RESET%^
.EI
.SI 5
int give_money(int base, int rand, string type)
.EI
.SP 7 5

This method is used to give some startup money to the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
base - the base amount of money to give (fixed)
.EP
.SP 9 5
rand - the random amount of money to give
.EP
.SP 9 5
type - the type of money to give (default: "copper")
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the return value of adjust_money()
.EP

.SI 3
* %^BOLD%^init_command%^RESET%^
.EI
.SI 5
varargs void init_command(string str, int tim)
.EI
.SP 7 5

This method allows you submit delayed commands to the npc
via a call_out.
.EP

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping args, object ob)
.EI

.SI 3
* %^BOLD%^init_equip%^RESET%^
.EI
.SI 5
void init_equip()
.EI
.SP 7 5

This method makes the npc initialise all their equipment, like hold
it and stuff.
.EP

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping args)
.EI

.SI 3
* %^BOLD%^load_a_chat%^RESET%^
.EI
.SI 5
void load_a_chat(int chance, mixed * c_s)
.EI
.SP 7 5

This method loads up the set of chat strings to use while in combat.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chance - the chance of the chat occuring
.EP
.SP 9 5
c_s - the chat string to use
.EP

.SI 3
* %^BOLD%^load_chat%^RESET%^
.EI
.SI 5
void load_chat(int chance, mixed * c_s)
.EI
.SP 7 5

This method loads up the chat strings for the npc.  This will be
an array containing pairs of elements, the first pair is the
weighting of the chat and the second is the chat to use.


All the weights in the array are added up and then a random number is chosen in the weighting. Then that element is looked up in the array. This way you can control a chat and make it rare. 

If the chat string is an array then this a story, the story will be executed one after another and no other chats will be executed in between. If the first parameter of the story array is a number it will be used as a 1/1000 chance of the next story line being displayed. Special strings can be used which will replace with object names, see expand_mon_string() for further information. 

The chat chance is a chance (in 1000) of the chat occuring. You will need to play with this yourself to see which frequency of chatting you wish for your npcs. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chance - the chance in 1000 of a chat working every 2 seconds
.EP
.SP 9 5
c_s - the chat string to use
.EP

.SI 3
* %^BOLD%^move_me_to%^RESET%^
.EI
.SI 5
varargs void move_me_to(string dest, int delay)
.EI
.SP 7 5

This method will move the npc to the specified destination.  The
npc will walk from where they currently are to the destination using
the time delay specified between the movements.


If the location is reached then the function "stopped_route" will be called on the npc. The first arguement to the function will be 0 if the npc did not reach its destination and 1 if it did.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - the destination to go to
.EP
.SP 9 5
delay - the time delay between each move
.EP

.SI 3
* %^BOLD%^query_achat_chance%^RESET%^
.EI
.SI 5
int query_achat_chance()
.EI
.SP 7 5

This method returns the current chat chance for attack messages on
the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current attack message chat chance
.EP

.SI 3
* %^BOLD%^query_achat_string%^RESET%^
.EI
.SI 5
string * query_achat_string()
.EI
.SP 7 5

This method queries the current chat string for attack messages on the
npc.  See load_chat() for a longer description of how the
chat string is formatted.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current attack message chat string
.EP

.SI 3
* %^BOLD%^query_aggressive%^RESET%^
.EI
.SI 5
int query_aggressive()
.EI
.SP 7 5

This method returns the current aggressive level of the npc.
If the aggressive is set to 1, then the npc will attack all players
that enter its environment.  If the aggressive is set to 2 then
the npc will attack everything (including other npcs).


See the function start_attack() for information about things you can do to stop aggressive npcs attacking things.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the aggressive level of the npc
.EP

.SI 3
* %^BOLD%^query_always_return_to_default_position%^RESET%^
.EI
.SI 5
int query_always_return_to_default_position()
.EI
.SP 7 5

This method returns the status of the flag that makes the npc return
to the default position if its position is changed.  The flag
specified the length of time to wait before causing the
default position to be restored.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time to wait before the position is restored
.EP

.SI 3
* %^BOLD%^query_cannot_change_position%^RESET%^
.EI
.SI 5
int query_cannot_change_position()
.EI
.SP 7 5

This method returns the current value of the unable to change
position flag.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the unable to change position flag
.EP

.SI 3
* %^BOLD%^query_cap_name%^RESET%^
.EI
.SI 5
string query_cap_name()
.EI
.SP 7 5

This method returns the current capitalized name of the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current capitalized name
.EP

.SI 3
* %^BOLD%^query_chat_chance%^RESET%^
.EI
.SI 5
int query_chat_chance()
.EI
.SP 7 5

This method returns the current chat chance for messages on
the npc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current chat chance
.EP

.SI 3
* %^BOLD%^query_chat_string%^RESET%^
.EI
.SI 5
string * query_chat_string()
.EI
.SP 7 5

This method queries the current chat string for messages on the
npc.  See load_chat() for a longer description of how the
chat string is formatted.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current chat string
.EP

.SI 3
* %^BOLD%^query_combat_actions%^RESET%^
.EI
.SI 5
mixed * query_combat_actions()
.EI
.SP 7 5

This method returns the current array of combat actions on the
npc.


The array will have the format of: 
.EP
.SP 7 5
   ({
.EP
.SP 7 5
       action1_chance,
.EP
.SP 7 5
       action1_name,
.EP
.SP 7 5
       action1_action,
.EP
.SP 7 5
       ...
.EP
.SP 7 5
    })
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the combat action array
.EP

.SI 3
* %^BOLD%^query_death_xp%^RESET%^
.EI
.SI 5
int query_death_xp()
.EI
.SP 7 5

This method returns the amount of death experiecne that would be
gained by killing the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of death experience for the npc

.EP

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_enter_commands%^RESET%^
.EI
.SI 5
string * query_enter_commands()
.EI
.SP 7 5

This method returns the current array of enter commands.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current array of enter commands
.EP

.SI 3
* %^BOLD%^query_fight_type%^RESET%^
.EI
.SI 5
int query_fight_type()
.EI
.SP 7 5

This method returns the flag which allows the npc to join into fights.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the npc is to join fights, 0 if not
.EP

.SI 3
* %^BOLD%^query_follow_speed%^RESET%^
.EI
.SI 5
int query_follow_speed()
.EI
.SP 7 5

This method queries the speed at which the npc will follow
after a player when they leave combat.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current follow speed
.EP

.SI 3
* %^BOLD%^query_following_route%^RESET%^
.EI
.SI 5
string * query_following_route()
.EI
.SP 7 5

This method returns the current array of directions we are following
as a route.
.EP

.SI 3
* %^BOLD%^query_guild%^RESET%^
.EI
.SI 5
string query_guild()
.EI
.SP 7 5

This method returns the current guild of the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
this current guild of the npc
.EP

.SI 3
* %^BOLD%^query_guild_ob%^RESET%^
.EI
.SI 5
mixed query_guild_ob()
.EI
.SP 7 5

This returns the guild object associated with the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the guild object associated with the npc
.EP

.SI 3
* %^BOLD%^query_join_fights%^RESET%^
.EI
.SI 5
string query_join_fights()
.EI
.SP 7 5

This method returns the message to use when joining into fights.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the message to print when joining a fight
.EP

.SI 3
* %^BOLD%^query_last_route_direction%^RESET%^
.EI
.SI 5
int query_last_route_direction()
.EI
.SP 7 5

This method tells us if the npc is currently following a route.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there are route directions to follow still
.EP

.SI 3
* %^BOLD%^query_level%^RESET%^
.EI
.SI 5
int query_level()
.EI
.SP 7 5

This method returns the current guild level of the npc.  This is
a pass through call to a function on the guild object associated
with this npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current guild level of the object
.EP

.SI 3
* %^BOLD%^query_move_after%^RESET%^
.EI
.SI 5
mixed query_move_after()
.EI
.SP 7 5

This method returns the current move after values.
It returns an array of the form:
.EP
.SP 7 5
   ({
.EP
.SP 7 5
     after,
.EP
.SP 7 5
     rand,
.EP
.SP 7 5
    })
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the move after values
.EP

.SI 3
* %^BOLD%^query_move_zones%^RESET%^
.EI
.SI 5
string * query_move_zones()
.EI
.SP 7 5

This method returns the current list of move zones on the npc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current list of move zones
.EP

.SI 3
* %^BOLD%^query_ok_turn_off_heart_beat%^RESET%^
.EI
.SI 5
int query_ok_turn_off_heart_beat()
.EI
.SP 7 5

This method returns 1 if it is ok to turn of the npc's heart beat.
THis can be overridden for times when the heart beat needs to be
kept on for some reason.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the heart beat should go off, 0 if it should stay on

.EP

.SI 3
* %^BOLD%^query_queued_commands%^RESET%^
.EI
.SI 5
mixed * query_queued_commands()
.EI
.SP 7 5

This method returns the queued command list.
.EP

.SI 3
* %^BOLD%^query_race%^RESET%^
.EI
.SI 5
string query_race()
.EI
.SP 7 5

This method returns the current race of the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current race of the object
.EP

.SI 3
* %^BOLD%^query_race_ob%^RESET%^
.EI
.SI 5
mixed query_race_ob()
.EI
.SP 7 5

This returns the race object associated with the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the race object associated with the npc
.EP

.SI 3
* %^BOLD%^query_spell_actions%^RESET%^
.EI
.SI 5
mapping query_spell_actions()
.EI
.SP 7 5

This method returns the list of spell actions present on the
npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of spell actions

.EP

.SI 3
* %^BOLD%^query_throw_out%^RESET%^
.EI
.SI 5
mixed * query_throw_out()
.EI
.SP 7 5

This method returns the current throw out array.
The array consists of
({
.EP
.SP 7 5
  hps,
.EP
.SP 7 5
  chance,
.EP
.SP 7 5
  their_mess,
.EP
.SP 7 5
  everyone_mess
.EP
.SP 7 5
})
.EP
.SP 7 5
The parameters are the same as used in the set_throw_out function.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the throw out array

.EP

.SI 3
* %^BOLD%^query_true_location%^RESET%^
.EI
.SI 5
string query_true_location()
.EI
.SP 7 5

This method returns the true location of the npc.  This is
the real room it is in, not the room it is currently in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the true location of the npc
.EP

.SI 3
* %^BOLD%^query_virtual_move%^RESET%^
.EI
.SI 5
int query_virtual_move()
.EI

.SI 3
* %^BOLD%^queue_command%^RESET%^
.EI
.SI 5
varargs int queue_command(string words, int interval)
.EI
.SP 7 5

This method allows you to control the npc and get it to do
actions where they are queued as for players.  If there are no
commands pending the command is executed immediately.  This function
is 100% compatible with delay_command() and init_command().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the action to perform
.EP
.SP 9 5
interval - to wait before processing another command.
If omitted defaults to 2 seconds as per players
.EP

.SI 3
* %^BOLD%^rand_num%^RESET%^
.EI
.SI 5
int rand_num(int no, int type)
.EI
.SP 7 5

This method generates a random number.  It used to setup the random
stats.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no - the number of times to roll the dice
.EP
.SP 9 5
type - the size of the dice
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the randomly generate number

.EP

.SI 3
* %^BOLD%^real_room%^RESET%^
.EI
.SI 5
void real_room(string check_room)
.EI
.SP 7 5

This method moves the npc to room it is really supposed to be in.
This is used with the virtual moving
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
check_room - the place where it thinks we are
.EP

.SI 3
* %^BOLD%^remove_achat_string%^RESET%^
.EI
.SI 5
void remove_achat_string(mixed chat)
.EI
.SP 7 5

This method attempts to remove the given chat string from the
current list of attack message chat strings.  The chat message is checked
to see if it exists in the array, the weighting of the
string is ignored.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chat - the chat string to remove
.EP

.SI 3
* %^BOLD%^remove_chat_string%^RESET%^
.EI
.SI 5
void remove_chat_string(mixed chat)
.EI
.SP 7 5

This method attempts to remove the given chat string from the
current list of chat strings.  The chat message is checked
to see if it exists in the array, the weighting of the
string is ignored.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chat - the chat string to remove
.EP

.SI 3
* %^BOLD%^remove_combat_action%^RESET%^
.EI
.SI 5
int remove_combat_action(string name)
.EI
.SP 7 5

This method will remove the combat action with the specified name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is found and removed, 0 if not
.EP

.SI 3
* %^BOLD%^remove_move_zone%^RESET%^
.EI
.SI 5
void remove_move_zone(mixed zone)
.EI
.SP 7 5

This method removes a move zone from the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone to remove
.EP

.SI 3
* %^BOLD%^remove_spell_action%^RESET%^
.EI
.SI 5
int remove_spell_action(string name)
.EI
.SP 7 5

This method removes the specified spell action.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the spell to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not

.EP

.SI 3
* %^BOLD%^reset_enter_commands%^RESET%^
.EI
.SI 5
void reset_enter_commands()
.EI
.SP 7 5

This method resets the array of enter commands back to nothing.
.EP

.SI 3
* %^BOLD%^run_away%^RESET%^
.EI
.SI 5
int run_away()
.EI
.SP 7 5

This method is used to make the npc run away.  This will be
called by the combat code for wimpy when the npc is bellow the
number of points used to trigger the wimpy action.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfuly ran away

.EP

.SI 3
* %^BOLD%^set_achat_chance%^RESET%^
.EI
.SI 5
void set_achat_chance(int i)
.EI
.SP 7 5

This method sets the current chat chance for attack messages on the
npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the attack message chat chance
.EP

.SI 3
* %^BOLD%^set_achat_string%^RESET%^
.EI
.SI 5
void set_achat_string(string * chat)
.EI
.SP 7 5

This method sets the current chat string for attack messages on the
npc.  See load_chat() for a longer description of how the
chat string is formatted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chat - the new chat attack message strings.
.EP

.SI 3
* %^BOLD%^set_aggressive%^RESET%^
.EI
.SI 5
void set_aggressive(int a)
.EI
.SP 7 5

This method sets the current aggressive level of the npc.
If the aggressive is set to 1, then the npc will attack all players
that enter its environment.  If the aggressive is set to 2 then
the npc will attack everything (including other npcs).


See the function start_attack() for information about things you can do to stop aggressive npcs attacking things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
a - the new aggressive level

.EP

.SI 3
* %^BOLD%^set_always_return_to_default_position%^RESET%^
.EI
.SI 5
void set_always_return_to_default_position(int tim)
.EI
.SP 7 5

This method sets the status of the flag that makes the npc return
to the default position if its position is changed.  The flag
specified the length of time to wait before causing the
default position to be restored.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
tim - the time to wait before the position is restored
.EP

.SI 3
* %^BOLD%^set_cannot_change_position%^RESET%^
.EI
.SI 5
void set_cannot_change_position(int flag)
.EI
.SP 7 5

This method sets the value of the unable to change position flag.
This flag will be checked by the soul, and by anything else which
deliberatly changes someones position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - the unable to change position flag
.EP

.SI 3
* %^BOLD%^set_cap_name%^RESET%^
.EI
.SI 5
void set_cap_name(string s)
.EI
.SP 7 5

This method set the current capitalized name of the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the capitalized name of the npc
.EP

.SI 3
* %^BOLD%^set_chat_chance%^RESET%^
.EI
.SI 5
void set_chat_chance(int i)
.EI
.SP 7 5

This method sets the current chat chance for messages on the
npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the chat chance
.EP

.SI 3
* %^BOLD%^set_chat_string%^RESET%^
.EI
.SI 5
void set_chat_string(string * chat)
.EI
.SP 7 5

This method sets the current chat string for messages on the
npc.  See load_chat() for a longer description of how the
chat string is formatted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chat - the new chat strings.
.EP

.SI 3
* %^BOLD%^set_follow_speed%^RESET%^
.EI
.SI 5
void set_follow_speed(int f)
.EI
.SP 7 5

This method sets the speed at which the npc will follow
after a player when they leave combat.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current follow speed
.EP

.SI 3
* %^BOLD%^set_guild%^RESET%^
.EI
.SI 5
int set_guild(string str)
.EI
.SP 7 5

This method is deprecated.  Use basic_setup() instead.

This method sets the current guild of the npc to the
passed in value.  The guild should be one of
the guilds listed in the /std/race.c object.
This is used in conjuction with the
race when set_level is called to setup the default
attributes for the npc.  This should only be
called *before* set_level() is called.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new guild for the npc
.EP

.SI 3
* %^BOLD%^set_guild_ob%^RESET%^
.EI
.SI 5
void set_guild_ob(mixed g)
.EI
.SP 7 5

This method sets the guild object associated with the npc.
This will probably not do what you expect and cannot be used
in conjucton with set_level to set the guild.  It is called
by /std/race.c when set_level() is called.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g - the new guild object

.EP

.SI 3
* %^BOLD%^set_join_fight_type%^RESET%^
.EI
.SI 5
void set_join_fight_type(int i)
.EI
.SP 7 5

This method sets the flag which allows the npc to join into fights.
If this is set to a non-zero value then the npc will join into
fights in progress using the fight joining message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - 1 if the npc is to join fights, 0 if not
.EP

.SI 3
* %^BOLD%^set_join_fights%^RESET%^
.EI
.SI 5
void set_join_fights(string str)
.EI
.SP 7 5

This method sets the message to use when joining into fights.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print when joining a fight
.EP

.SI 3
* %^BOLD%^set_level%^RESET%^
.EI
.SI 5
void set_level(int i)
.EI
.SP 7 5

This function is deprecated.  Use basic_setup() instead.

This method sets the level of the npc.  This should only be called
*after* the race and guild are set.  If this is called before that
the results will be unexpected.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the level to set the npc to

.EP

.SI 3
* %^BOLD%^set_move_after%^RESET%^
.EI
.SI 5
void set_move_after(int after, int rand)
.EI
.SP 7 5

This method sets the speed at which the npc will randomly
wander around.  The npc will wander at the speed:
.EP
.SP 7 5
speed = after + random(rand)
.EP
.SP 7 5
This is called every time the npc sets up for its next move. 

The move zones control which areas the npcs will wander into, a move zone is set on the room and the npcs will only enter rooms which have a matching move zone. If there is no move zone, then the npc will enter any room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
after - the fixed amount of time
.EP
.SP 9 5
rand - the random amount of time
.EP

.SI 3
* %^BOLD%^set_race%^RESET%^
.EI
.SI 5
int set_race(string str)
.EI
.SP 7 5

This function is deprecated.  Use basic_setup() instead.

This method sets the race of the npc.  The race should be one of
the races listed in the /std/race.c object.
This is used in conjuction with the
guild when set_level is called to setup the default
attributes for the npc.  This should only be
called *before* set_level() is called.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the race to set
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1
.EP

.SI 3
* %^BOLD%^set_race_ob%^RESET%^
.EI
.SI 5
void set_race_ob(mixed r)
.EI
.SP 7 5

This method sets the race object associated with the npc.
This will probably not do what you expect and cannot be used
in conjucton with set_level to set the race.  It is called
by /std/race.c when set_level() is called.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
r - the new race object

.EP

.SI 3
* %^BOLD%^set_random_stats%^RESET%^
.EI
.SI 5
void set_random_stats(int no, int type)
.EI
.SP 7 5

This method sets the stats for the npc to some exciting random
values.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no - the number of times to roll the dice
.EP
.SP 9 5
type - the size of the dice
.EP

.SI 3
* %^BOLD%^set_throw_out%^RESET%^
.EI
.SI 5
void set_throw_out(int hps, int chance, string their_mess, string everyone_mess)
.EI
.SP 7 5

This method is used to determine when to throw people out of a
room.  This is what detritus uses to throw people out of the
mended drum when it gets a bit rowdy.


The hps is the level of hps at which the npc will start throwing people out with the chance of it occuring (chance is a percentage). 

People will be thrown into a random room, if the property "no throw out"is specified on the room then they will not be thrown into that room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
hps - the number of hps at which to start throwing people out
.EP
.SP 9 5
chance - the percentage chance of being thrown out
.EP
.SP 9 5
their_mess - the message to show them
.EP
.SP 9 5
everyone_mess - the message to show everyone else
.EP

.SI 3
* %^BOLD%^set_true_location%^RESET%^
.EI
.SI 5
void set_true_location(string word)
.EI
.SP 7 5

This method sets the true location of the npc.  This is
the real room it is in, not the room it is currently in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new true location of the npc
.EP

.SI 3
* %^BOLD%^set_virtual_move%^RESET%^
.EI
.SI 5
void set_virtual_move(int number)
.EI
.SP 7 5

This method sets the current virual move ability of the npc.
NB: This is currently disabled virtual moving is not possible
at all.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - 1 for virtual moving, 0 for not
.EP

.SI 3
* %^BOLD%^setup_nationality%^RESET%^
.EI
.SI 5
void setup_nationality(string nationality, string region)
.EI
.SP 7 5

This method sets up a nationality and sets up a region in the
nationality for the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
nationality - the nationality to set
.EP
.SP 9 5
region - the region in the nationality

.EP

.SI 3
* %^BOLD%^start_attack%^RESET%^
.EI
.SI 5
void start_attack(object who)
.EI
.SP 7 5

This method check to see if the npc should start attacking someone
when they enter the npcs environment.  It is called from inside
init().  It will only attack if the agressive is set and the
person is visible to be attacked.  The property
"no attack"can be set on the npc (or player) to stop them being attacked.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
who - the person to potentially start attacking
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^got_the_route%^RESET%^
.EI
.SI 5
void got_the_route(string * route, int delay, string dest)
.EI
.SP 7 5

This method is called by the move_me_to function after the
route handler has successfuly discovered the route to follow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
route - the route to follow
.EP
.SP 9 5
delay - the delay to follow it with
.EP
.SP 9 5
dest - route destination
.EP


