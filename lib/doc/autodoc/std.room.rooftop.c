.DT
rooftop.c
Disk World autodoc help
rooftop.c

.SH Description
.SP 5 5

This is a standard rooftop inheritable. It allows you to setup a rooftop
location including such things as slope, strength, jumping locations etc.

.EP
.SP 10 5


Written by shrike

.EP



.SH See also
.SP 5 5
set_wall
.EP
.SH Example
.SI 5
inherit "/std/rooftop"

void setup() {
   set_fall_damage( "all", 500 );
   set_slope( 60, "ledge", PATH + "womble01" );
   set_weak_messages(
     "The ledge crumbles under your weight.",
     "$short$ falls from the crumbling ledge.",
     "$short$ comes crashing to the ground.",
     "The paving stones shift beneath your feet.",
   );
   set_weak_roof( 2000, PATH + "womble01" );
   set_jump( ({ "north", "n" }), PATH + "ledge01", PATH + "womble01", 6 );
}

.EI
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/outside.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_fall_damage%^RESET%^
.EI
.SI 5
mixed query_fall_damage(string type)
.EI
.SP 7 5

Find out the damage for falling from this location.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - The type ( weak, slope, step, jump, or all )
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
An int, or an integer array of the damage(s)

.EP

.SI 3
* %^BOLD%^set_corpse_slope_messages%^RESET%^
.EI
.SI 5
void set_corpse_slope_messages(string from, string to)
.EI
.SP 7 5

Set the messages for when a corpse slides off a sloping roof.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the message shown in the room the corpse is leaving.
.EP
.SP 9 5
to - the message shown in the room the corpse is entering.

.EP

.SI 3
* %^BOLD%^set_fall_damage%^RESET%^
.EI
.SI 5
void set_fall_damage(string type, int damage)
.EI
.SP 7 5

Set the amount of damage the player will take if they fall off this
roof.  This amount is somewhat randomised.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - The type of fall (weak, slope, step or jump; all to set all damages the same)
.EP
.SP 9 5
damage - The amount of damage.

.EP

.SI 3
* %^BOLD%^set_ghost_fall_messages%^RESET%^
.EI
.SI 5
void set_ghost_fall_messages(string player, string from, string to)
.EI
.SP 7 5

Set the messages for when a ghost falls off a roof.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the message shown to the player.
.EP
.SP 9 5
from - the message shown in the room the player is leaving.
.EP
.SP 9 5
to - the message shown in the room the player is entering.

.EP

.SI 3
* %^BOLD%^set_item_slope_messages%^RESET%^
.EI
.SI 5
void set_item_slope_messages(string from, string to)
.EI
.SP 7 5

Set the messages for when a non-corpse item slides off a sloping roof.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the message shown in the room the object is leaving.
.EP
.SP 9 5
to - the message shown in the room the object is entering.

.EP

.SI 3
* %^BOLD%^set_jump%^RESET%^
.EI
.SI 5
int set_jump(mixed dir, string dest, string fall_dest, int distance)
.EI
.SP 7 5

Setup a jumping exit. The player will be able to "jump <dir>" and a skill test will be performed and they'll succeed or fail. Streets in AM are usually 5-20 wide. if you want to do something other than the standard fall function, you'll need to mask do_fall() in the child room. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - direction names
.EP
.SP 9 5
dest - the destination
.EP
.SP 9 5
fall_dest - the destination if they fall
.EP
.SP 9 5
distance - the distance of the jump
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
-1 if any of the keys in dir already exists, 0 if dir is
malformed (string and string * are the only acceptable types), or
1 on success.

.EP

.SI 3
* %^BOLD%^set_jump_failure_messages%^RESET%^
.EI
.SI 5
void set_jump_failure_messages(string player, string from, string to)
.EI
.SP 7 5

Set the messages for when a player falls off by failing to jump across
a gap.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the message shown to the player.
.EP
.SP 9 5
from - the message shown in the room the player is leaving.
.EP
.SP 9 5
to - the message shown in the room the player is entering.

.EP

.SI 3
* %^BOLD%^set_jump_success_messages%^RESET%^
.EI
.SI 5
void set_jump_success_messages(string player, string from, string to)
.EI
.SP 7 5

Set the messages for when a player successfully jumps a gap.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the message shown to the player.
.EP
.SP 9 5
from - the message shown in the room the player is leaving.
.EP
.SP 9 5
to - the message shown in the room the player is entering.

.EP

.SI 3
* %^BOLD%^set_jump_tm_messages%^RESET%^
.EI
.SI 5
void set_jump_tm_messages(string * messages)
.EI
.SP 7 5

Set the messages for when a player gets a TM by jumping. These messages
will be selected at random, and then coloured appropriately.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
messages - the array of messages from which to choose.

.EP

.SI 3
* %^BOLD%^set_slope%^RESET%^
.EI
.SI 5
void set_slope(int angle, string loc, string dest)
.EI
.SP 7 5

Set the gradient of this location.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
angle - the angle (in degrees).
.EP
.SP 9 5
loc - a short description of the area to show to players.
.EP
.SP 9 5
dest - the fall destination.

.EP

.SI 3
* %^BOLD%^set_slope_messages%^RESET%^
.EI
.SI 5
void set_slope_messages(string player, string from, string to)
.EI
.SP 7 5

Set the messages for when a player slips off a sloping roof.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the message shown to the player.
.EP
.SP 9 5
from - the message shown in the room the player is leaving.
.EP
.SP 9 5
to - the message shown in the room the player is entering.

.EP

.SI 3
* %^BOLD%^set_step_messages%^RESET%^
.EI
.SI 5
void set_step_messages(string player, string from, string to)
.EI
.SP 7 5

Set the messages for when a player falls off by moving in the wrong
direction.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the message shown to the player.
.EP
.SP 9 5
from - the message shown in the room the player is leaving.
.EP
.SP 9 5
to - the message shown in the room the player is entering.

.EP

.SI 3
* %^BOLD%^set_weak_messages%^RESET%^
.EI
.SI 5
void set_weak_messages(string player, string from, string to, string warn)
.EI
.SP 7 5

Set the messages for when a player falls through a weak roof.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the message shown to the player.
.EP
.SP 9 5
from - the message shown in the room the player is leaving.
.EP
.SP 9 5
to - the message shown in the room the player is entering.
.EP
.SP 9 5
warn - the message shown to the room just before the roof collapses.

.EP

.SI 3
* %^BOLD%^set_weak_roof%^RESET%^
.EI
.SI 5
void set_weak_roof(int maxweight, string dest)
.EI
.SP 7 5

Set the maximum weight this location can support.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
maxweight - the maximum weight capacity of the roof.
.EP
.SP 9 5
dest - the room you go to if the roof collapses.

.EP

.SI 3
* %^BOLD%^test_remove%^RESET%^
.EI
.SI 5
int test_remove(object ob, int flag, mixed dest)
.EI


