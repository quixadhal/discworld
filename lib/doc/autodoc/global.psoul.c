.DT
psoul.c
Disk World autodoc help
psoul.c

.SH Description
.SP 5 5

This class controls the entire command queue for all player
objects.  Whenever a player tries to execute a command,
it gets placed in a queue here and eventually (during the
player's heart_beat()), the command will (hopefully) be executed.



Since this class is a nexus of control for player commands, it is also the natural place for a number of other utilities and functions which also affect the execution of all commands. Among these are things like drunk_check(), do_soul(), etc. 
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/global/new_parse->add_command(), /global/player->heart_beat() and query_passed_out_message()

.EP
.SH Change history
.SP 5 5
3 November 1997 -- Sin
     Documented the bejeesus out of this thing.

4 Novemebr 1997 - Pinkfish
     Updated the documentation and changed the interupt system slightly.

22 Feburary 1998 - Pinkfish
     Fixed up the problems with the queueing system.

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/alias and /global/nickname.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/soul.h, /include/living.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_time_left%^RESET%^
.EI
.SI 5
int adjust_time_left(int i)
.EI
.SP 7 5

Change the amount of time a player has left.  You call this after a command
has been executed to make it take more time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the amount of time units to change by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of time left

.EP

.SI 3
* %^BOLD%^bypass_queue%^RESET%^
.EI
.SI 5
void bypass_queue()
.EI
.SP 7 5

To make the next single command be executed directly rather
than being placed in the command queue, call this function.

.EP

.SI 3
* %^BOLD%^command_override%^RESET%^
.EI
.SI 5
void command_override(function func)
.EI
.SP 7 5

Use this function to set a function that is called with the players input
before the command handlers get to it, return 1 from the function if the
input needs no further parsing (ie the command is handled)

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - = function in the players environment to call.

.EP

.SI 3
* %^BOLD%^do_soul%^RESET%^
.EI
.SI 5
void do_soul(string str, mixed bing)
.EI
.SP 7 5

All soul commands eventually call this function to output their
messages.  This is nothing more than a wrapper for say(), but
it provides a convenient name by which a shadow on the
player object can replace any soul behavior.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string being printed
.EP

.SI 3
* %^BOLD%^interupt_command%^RESET%^
.EI
.SI 5
void interupt_command(object interupter)
.EI
.SP 7 5

This method interupts the current command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
interupter - the person interupting the command

.EP

.SI 3
* %^BOLD%^lower_check%^RESET%^
.EI
.SI 5
int lower_check(string str)
.EI
.SP 7 5

This function will get called when all other commands and actions
have refused to do anything for this input from the user.  This
function adds some extra time for the user, and then returns.



This function is registered via add_action() with a priority of -10000. If you want to bypass it, then use a priority higher than that. If you want to see what strings get dropped by the entire add_action() stack, then use a priority lower. 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the user's input is "stop", otherwise 0.

.EP

.SI 3
* %^BOLD%^no_time_left%^RESET%^
.EI
.SI 5
void no_time_left()
.EI
.SP 7 5

Ensure that the player has no more time for executing commands.
This will force the next command to be queued.

.EP

.SI 3
* %^BOLD%^query_interupt_command%^RESET%^
.EI
.SI 5
mixed * query_interupt_command()
.EI
.SP 7 5

This method returns the current value associated with tine interupt
command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current interupt command data

.EP

.SI 3
* %^BOLD%^query_queued_commands%^RESET%^
.EI
.SI 5
int query_queued_commands()
.EI
.SP 7 5

You can use this function to see if there are any commands
queued for this player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of queued commands

.EP

.SI 3
* %^BOLD%^query_time_left%^RESET%^
.EI
.SI 5
int query_time_left()
.EI
.SP 7 5

The amount of time units left.  A time unit is 1/40th of a second.

.EP

.SI 3
* %^BOLD%^remove_queue%^RESET%^
.EI
.SI 5
void remove_queue()
.EI
.SP 7 5

This is called by the stop command.  It sets the entire queue back to
empty.  It calls the interrupt functions and stuff if they need to be
called.

.EP

.SI 3
* %^BOLD%^set_interrupt_command%^RESET%^
.EI
.SI 5
void set_interrupt_command(function func)
.EI
.SP 7 5

Fixes a spelling error.   This one only takes a function pointer as an
input.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function pointer to call back with

.EP

.SI 3
* %^BOLD%^set_interupt_command%^RESET%^
.EI
.SI 5
void set_interupt_command(mixed func, mixed ob, mixed arg)
.EI
.SP 7 5

Sets the function to be executed if the command is interrupted.
It is also executed if teh command finished.  If it is interrupted
the first arguement to the called function will be the amount of time
it had left to complete.  If it complets successfuly, this
argument will be 0.  If the first argument is a function pointer,
this will be used instead.

Eg: set_interupt_command("frog", this_object());

void frog(int time_left, mixed arg) { 

... 

} 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call back
.EP
.SP 9 5
ob - the object to call the function on
.EP
.SP 9 5
arg - the argument to pass to the function
.EP

.SI 3
* %^BOLD%^soul_com_force%^RESET%^
.EI
.SI 5
int soul_com_force(string str)
.EI
.SP 7 5

Some of the soul commands force the target to do something.
An example is the 'tickle' soul, which forces the target to
'giggle'.  Those soul-forces call this function.  But only
the soul object can use this function: any other object which calls
this function will be ignored.  This prevents this function
from being used to bypass the security checking on the
'force' command.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the command being forced
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the command was ignored, otherwise 1.

.EP

.SI 3
* %^BOLD%^soul_commands%^RESET%^
.EI
.SI 5
void soul_commands()
.EI
.SP 7 5

This is a setup function that is called by the player object.
It is used to register the lower_check() and drunk_check()
functions.  Plus it initializes the alias object, the
nickname object, and the history object.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^drunk_check%^RESET%^
.EI
.SI 5
int drunk_check(string str)
.EI
.SP 7 5

This poorly named function was originally used to affect the
player's behavior when they are drunk, and to prevent any
player from doing anything in the event that they are passed
out.  Now the function also is responsible for adding commands
to the player's command queue, for implementing the 'stop'
and 'restart' commands, and for ensuring that the player can quit
the game, even when queueing.



To see if a player is passed out, it checks the "passed out" property. If that property is nonzero, then the player will be prevented from doing the command unless that player is also a creator. By default, it will print a message that says: "You are unconscious. You can't do anything.\n". If the function query_passed_out_message() is defined on the player object (usually by a shadow), and returns a string, then that string is printed instead. 

This function is registered with add_action("drunk_check", "*", 10000), so it can be avoided by registering a higher priority add_action. But that is not necessary, because if bypass_queue() has been called, this function will do nothing. Please note that you should not be using add_action. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the command being executed

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if nothing was done, 1 if drunk_check() blocked
the command.
.EP

.SI 3
* %^BOLD%^flush_queue%^RESET%^
.EI
.SI 5
void flush_queue()
.EI
.SP 7 5

This method flushes all the queued commands.  It increments the time by the
ROUND_TIME define and checks to see if any of the commands now need to be
executed.  This should be called each heart beat..

.EP

.SI 3
* %^BOLD%^process_input%^RESET%^
.EI
.SI 5
string process_input(string str)
.EI
.SP 7 5

This is the command called by the driver on a player object every
time a command is executed.  It expands the history comands.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to expand
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the expanded history string

.EP


