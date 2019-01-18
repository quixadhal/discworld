.DT
close_lock.c
Disk World autodoc help
close_lock.c

.SH Description
.SP 5 5

This class keeps track of whether things are open or closed,
locked or unlocked.  Trap functions that are triggered when
an object is opened or unlocked are also handled.  When
you need an object that is locked you should remember to
use set_key() and set_difficulty().  When cloning an object
and moving other objects inside it you should remember to
make sure the object is open before moving things into it,
and then close it if neccessary.

.EP
.SP 10 5


Written by Gototh

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h, /include/move_failures.h and /include/clothing.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_close_lock_commands%^RESET%^
.EI
.SI 5
void add_close_lock_commands(object player)
.EI
.SP 7 5

This method adds the lock commands to the specified player.
It's been abstracted out of init() to make it easier for some
odd situations (like the t-shop).

.EP

.SI 3
* %^BOLD%^pick_unlock%^RESET%^
.EI
.SI 5
int pick_unlock(object player)
.EI
.SP 7 5

This is called when someone successfully unlocks the object.
If there is a trap, and the trap function returns true, the
object will not be opened.  If the trap function returns
false, the object will still be opened even if the trap is
sprung.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The player who is unlocking the object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Returns 1 if the object was unlocked, and 0 otherwise.

.EP

.SI 3
* %^BOLD%^query_autolock%^RESET%^
.EI
.SI 5
int query_autolock()
.EI
.SP 7 5

This tells us whether an object will lock automatically when closed.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for true 0 for false;

.EP

.SI 3
* %^BOLD%^query_closed%^RESET%^
.EI
.SI 5
int query_closed()
.EI
.SP 7 5

This function tells you whether an object is open or closed.
It returns 1 if the object is closed, and 0 if the object is open.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if closed, 0 if open.
.EP

.SI 3
* %^BOLD%^query_closed_locked_status%^RESET%^
.EI
.SI 5
int query_closed_locked_status()
.EI
.SP 7 5

This method returns the closed and locked status.  The status is
0 if open and unlocked, 1 if locked, 2 if closed and 3 if closed
and locked.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
closed and locked status

.EP

.SI 3
* %^BOLD%^query_difficulty%^RESET%^
.EI
.SI 5
int query_difficulty()
.EI
.SP 7 5

This returns the difficulty picking the lock on the object.
By default it is set to 200 unless it has been changed with
set_difficulty().

.EP

.SI 3
* %^BOLD%^query_key%^RESET%^
.EI
.SI 5
string query_key()
.EI
.SP 7 5

This returns the property of the key that will unlock the
object.

.EP

.SI 3
* %^BOLD%^query_lock_trap_func%^RESET%^
.EI
.SI 5
string query_lock_trap_func()
.EI
.SP 7 5

This returns the name of the function to be called when
some attempts to unlock the object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The function to be called when someone attempts
to unlock the object.
.EP

.SI 3
* %^BOLD%^query_lock_trap_ob%^RESET%^
.EI
.SI 5
object query_lock_trap_ob()
.EI
.SP 7 5

This returns the object on which the trap function is stored.
The trap function is called when someone tries to unlock the
the object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The function to be called when the object is opened.
.EP

.SI 3
* %^BOLD%^query_locked%^RESET%^
.EI
.SI 5
int query_locked()
.EI
.SP 7 5

This tells us whether an object is locked.  It returns 1 if
the object is locked and 0 is the object is unlocked.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if locked and 0 if unlocked.

.EP

.SI 3
* %^BOLD%^query_open%^RESET%^
.EI
.SI 5
int query_open()
.EI
.SP 7 5

This function tells you whether an object is open or closed.
It returns 1 if the object is open and 0 if the object is closed.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if open, 0 if closed.
.EP

.SI 3
* %^BOLD%^query_open_trap_func%^RESET%^
.EI
.SI 5
string query_open_trap_func()
.EI
.SP 7 5

This returns the name of the function to be called when
someone tries to open the object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The function to be called when the object is opened.
.EP

.SI 3
* %^BOLD%^query_open_trap_ob%^RESET%^
.EI
.SI 5
object query_open_trap_ob()
.EI
.SP 7 5

This returns the object on which the trap function is stored.
The trap function is called when someone tries to open the
the object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The function to be called when the object is opened.
.EP

.SI 3
* %^BOLD%^query_pick_skill%^RESET%^
.EI
.SI 5
string query_pick_skill()
.EI
.SP 7 5

This returns the name of the skill which is used when
determining if the object can be unlocked.

.EP

.SI 3
* %^BOLD%^query_stuck%^RESET%^
.EI
.SI 5
int query_stuck()
.EI
.SP 7 5

This tells you whether an object is stuck or not.  It will
return 1 if the object is stuck, and 0 if the object is not
stuck.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if the object is stuck, and 0 if it is not.

.EP

.SI 3
* %^BOLD%^query_transparent%^RESET%^
.EI
.SI 5
int query_transparent()
.EI
.SP 7 5

This method returns the current transparent
value of the object.
A transparent object is one in which you can see the
inventory even if it is closed.  This is also
used for light propogration.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the object is transparent

.EP

.SI 3
* %^BOLD%^query_unlocked%^RESET%^
.EI
.SI 5
int query_unlocked()
.EI
.SP 7 5

This tells us whether an object is unlocked.  It returns 1 if
the object is unlocked and 0 is the object is locked.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if unlocked and 0 if locked.

.EP

.SI 3
* %^BOLD%^reset_transparent%^RESET%^
.EI
.SI 5
void reset_transparent()
.EI
.SP 7 5

This method removes the current objects transparent
status.
A transparent object is one in which you can see the
inventory even if it is closed.  This is also
used for light propogration.

.EP

.SI 3
* %^BOLD%^set_autolock%^RESET%^
.EI
.SI 5
void set_autolock(int number)
.EI
.SP 7 5

This sets the object so that it will lock automatically when closed.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - 1 for autolock, 0 for no autolock.

.EP

.SI 3
* %^BOLD%^set_closed%^RESET%^
.EI
.SI 5
void set_closed()
.EI
.SP 7 5

This sets the object as being closed.  If you are moving
objects into the object, make sure that you do not set
the object as closed and then try to move the objects in.
Move the objects in and then close the object.

.EP

.SI 3
* %^BOLD%^set_difficulty%^RESET%^
.EI
.SI 5
void set_difficulty(int i)
.EI
.SP 7 5

This sets how difficult the lock on an object is to pick.
By default it is set to 200.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - The difficulty of the lock to pick.

.EP

.SI 3
* %^BOLD%^set_key%^RESET%^
.EI
.SI 5
void set_key(mixed val)
.EI
.SP 7 5

This sets the property of the key that will unlock the object. This can
be a string, or a function pointer which will evaluate to a string.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
val - The property the key must have to unlock the object.

.EP

.SI 3
* %^BOLD%^set_lock_trap%^RESET%^
.EI
.SI 5
void set_lock_trap(object ob, string func)
.EI
.SP 7 5

This sets the function to be called when someone attempts to
unlock the object.  The function does not have to be in the
object's file, and can be specified using the 'ob' paramater.
In most cases the function will be stored in the same file,
so use this_object() in these cases.

The function will be called with a single string parameter of "lock",
"unlock" or "pick"

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - The object in which the function is stored.
.EP
.SP 9 5
func - The function to be called.
.EP

.SI 3
* %^BOLD%^set_locked%^RESET%^
.EI
.SI 5
void set_locked()
.EI
.SP 7 5

This sets the object as being locked.  Remember to set the
object as being closed before using this.  In most cases you
should probably use set_key() to set the key which opens the
lock and use set_difficulty() to set the skill level required
to pick the lock.

.EP

.SI 3
* %^BOLD%^set_open%^RESET%^
.EI
.SI 5
void set_open()
.EI
.SP 7 5

This sets the object as being open.  Note that to find out
if an object is open we use query_closed() which will return
0 if the object is open.

.EP

.SI 3
* %^BOLD%^set_open_trap%^RESET%^
.EI
.SI 5
void set_open_trap(object ob, string func)
.EI
.SP 7 5

This sets the function to be called when someone attempts to
open the object.  The function does not have to be in the
object's file, and can be specified using the 'ob' paramater.
In most cases the function will be stored in the same file,
so use this_object() in these cases.

The function will be called with a single string parameter of "open" or
"close"

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - The object in which the function is stored.
.EP
.SP 9 5
func - The function to be called.
.EP

.SI 3
* %^BOLD%^set_pick_skill%^RESET%^
.EI
.SI 5
void set_pick_skill(string str)
.EI
.SP 7 5

This sets the skill that will be used when attempting to
pick the lock.  By default it is set to "covert.lockpick.safes"
so it need only be changed if you want to check a different
skill.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - The skill to use when picking the lock.

.EP

.SI 3
* %^BOLD%^set_stuck%^RESET%^
.EI
.SI 5
void set_stuck(int i)
.EI
.SP 7 5

This can set an object as being stuck, or make it unstuck.
Stuck objects cannot be opened, closed, locked or unlocked.
Using set_stuck(1) will make the object stuck, and set_stuck(0)
will unstick it.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - Use 1 to make the object stuck, and 0 to unstick it.

.EP

.SI 3
* %^BOLD%^set_transparent%^RESET%^
.EI
.SI 5
void set_transparent()
.EI
.SP 7 5

This sets the object as being transparent.
A transparent object is one in which you can see the
inventory even if it is closed.

.EP

.SI 3
* %^BOLD%^set_unlocked%^RESET%^
.EI
.SI 5
void set_unlocked()
.EI
.SP 7 5

This sets the object as being unlocked.

.EP


