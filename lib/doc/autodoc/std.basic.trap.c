.DT
trap.c
Disk World autodoc help
trap.c

.SH Description
.SP 5 5

This is a basic trap system for adding traps to container type objects (or
anything that uses /std/basic/close_lock.

To setup the trap use setup_trap(). Functions are available to find out
information about a trap on an object.

If you wish to create a more advanced trap you can use the set_lock_trap and
set_open_trap functions in /std/basic/close_lock.

.EP
.SP 10 5


Written by ceres
.EP



.SH See also
.SP 5 5
/std/basic/close_lock and setup_trap

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
* %^BOLD%^query_trap_armed%^RESET%^
.EI
.SI 5
int query_trap_armed()
.EI
.SP 7 5

Get whether the trap is armed or not.

.EP

.SI 3
* %^BOLD%^query_trap_data%^RESET%^
.EI
.SI 5
mixed * query_trap_data()
.EI
.SP 7 5

Get data on the basic lock trap. Data is returned as a mixed array of
stuff. This is really only used for the autoload. :)

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mixed array of trap data.

.EP

.SI 3
* %^BOLD%^query_trap_description%^RESET%^
.EI
.SI 5
string query_trap_description()
.EI
.SP 7 5

Get the trap description text.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string description of the trap.

.EP

.SI 3
* %^BOLD%^query_trap_difficulty%^RESET%^
.EI
.SI 5
int query_trap_difficulty()
.EI
.SP 7 5

Find out the difficulty of this trap.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int difficulty

.EP

.SI 3
* %^BOLD%^query_trap_effect%^RESET%^
.EI
.SI 5
mixed query_trap_effect()
.EI
.SP 7 5

Get the trap effects.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mixed either an int (for the damage) or a mixed variable for
  the effect to add to the triggerer.

.EP

.SI 3
* %^BOLD%^query_trap_message%^RESET%^
.EI
.SI 5
mixed query_trap_message()
.EI
.SP 7 5

Get the trap trigger messages

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mixed a string or array of messages

.EP

.SI 3
* %^BOLD%^query_trap_trigger%^RESET%^
.EI
.SI 5
string query_trap_trigger()
.EI
.SP 7 5

Get the trigger condition for this trap.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string the trigger condition.

.EP

.SI 3
* %^BOLD%^set_trap_armed%^RESET%^
.EI
.SI 5
void set_trap_armed(int i)
.EI
.SP 7 5

Set the trap as armed or disarmed.

.EP

.SI 3
* %^BOLD%^setup_trap%^RESET%^
.EI
.SI 5
void setup_trap(int difficulty, string trigger, string description, mixed message, mixed effect)
.EI
.SP 7 5

This sets up a basic trap on a lock.  The trap will be triggered when
someone locks/unlocks/picks the lock (depending on trigger). If
triggered the message will be displayed and the effect will be added
to the player.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
difficulty - skill bonus needed to avoid triggering the trap
.EP
.SP 9 5
trigger - "unlock", "pick, "open",  for which activities should
trigger the trap.
.EP
.SP 9 5
description - a description of the trap for someone who looks at
  it.
.EP
.SP 9 5
message - The message to be displayed to the player. If it's an array
the first index will be printed to the player the second
to the room.
.EP
.SP 9 5
effect - the effect to be added to the trigerer. If the effect is an
int the players hps will be reduced by that amount. If it is
a string that effect will be added to the player, if it's an
array the first index will be treated as the effect the rest
as parameters to the effect.

.EP


