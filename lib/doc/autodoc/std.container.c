.DT
container.c
Disk World autodoc help
container.c

.SH Description
.SP 5 5

The basic container inheritable, please note this is *not* a container
like a bucket.  This now allows things inside the container to
inform us that they should be used as part of the inventory of this
object.  This means the contents of containers will look like they
are not inside containers to the outside world.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/obj/container.c, /obj/baggage.c, /obj/vessel.c and /obj/clothing.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/cute_look, /std/object, /global/auto_load and /std/basic/export_inventory.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/player.h, /include/move_failures.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_tracked_items%^RESET%^
.EI
.SI 5
nomask void add_tracked_items(int n_items)
.EI
.SP 7 5

This method handles the addition of tracked items.
Tracked items want to be notified if the container moves.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n_items - the number of tracked items added
.EP

.SI 3
* %^BOLD%^add_weight%^RESET%^
.EI
.SI 5
int add_weight(int n)
.EI
.SP 7 5

This method is called in the move functions, it adds extra
weight onto the object when something is moved inside it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n - the amount to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfuly added
.EP

.SI 3
* %^BOLD%^can_find_match_recurse_into%^RESET%^
.EI
.SI 5
int can_find_match_recurse_into(object looker)
.EI
.SP 7 5

This method allows the container to have stuff inside it checked.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
looker - the person doing the checking
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failur

.EP

.SI 3
* %^BOLD%^can_find_match_reference_inside_object%^RESET%^
.EI
.SI 5
int can_find_match_reference_inside_object(object thing, object looker)
.EI
.SP 7 5

This method checks to see if the find match code can actually
reference this object inside us.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to reference
.EP
.SP 9 5
looker - the person looking at it
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they can, 0 if they cannot

.EP

.SI 3
* %^BOLD%^disable_item_tracking%^RESET%^
.EI
.SI 5
nomask void disable_item_tracking()
.EI
.SP 7 5

This method disables item tracking
.EP

.SI 3
* %^BOLD%^do_restore_inventory_error%^RESET%^
.EI
.SI 5
int do_restore_inventory_error(object ob, int move_flag)
.EI
.SP 7 5

This method handles the case where the return of the move flag is not
MOVE_OK.  This can do whatever we want to make it work in a more useful
fashion.

.EP

.SI 3
* %^BOLD%^enable_item_tracking%^RESET%^
.EI
.SI 5
nomask void enable_item_tracking()
.EI
.SP 7 5

This method enables item tracking
.EP

.SI 3
* %^BOLD%^event_container_move%^RESET%^
.EI
.SI 5
nomask void event_container_move(object mover, mixed from, mixed to)
.EI
.SP 7 5

This method handles the movement of the containers of this object.
Tracked items want to be notified if the container moves or anything
containing it does.  The movement of the outmost moving container is 
propagated down to all tracked items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mover -    The outermost container object that moved
.EP
.SP 9 5
from -     The start environment of the outermost container 
.EP
.SP 9 5
to -       The destination of the outermost container 
.EP

.SI 3
* %^BOLD%^event_move_object%^RESET%^
.EI
.SI 5
void event_move_object(mixed from, mixed to)
.EI
.SP 7 5

This method is called from move to notify any tracked items contained of
the move using event_container_moved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from -   start
.EP
.SP 9 5
to -     destination

.EP

.SI 3
* %^BOLD%^find_inv_match%^RESET%^
.EI
.SI 5
object * find_inv_match(string words, object looker)
.EI
.SP 7 5

This method finds the matching objects inside this object
that are visible to the looker.  This will also use the
registered containers and add them into the array returned
(if visible and contained in the object).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the words matched on
.EP
.SP 9 5
looker - who is looking
.EP

.SI 3
* %^BOLD%^query_complete_weight%^RESET%^
.EI
.SI 5
int query_complete_weight()
.EI
.SP 7 5

This method returns the complete weight of the object.  This is the
weight of the container itself, plus the weight of the things
inside it
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the complete_weight()
.EP

.SI 3
* %^BOLD%^query_loc_weight%^RESET%^
.EI
.SI 5
int query_loc_weight()
.EI
.SP 7 5

This method returns the current local weight in this
container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the local weight

.EP

.SI 3
* %^BOLD%^query_max_items%^RESET%^
.EI
.SI 5
int query_max_items()
.EI
.SP 7 5

This method returns the maximum number of items that can
be carried in this container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum number of items
.EP

.SI 3
* %^BOLD%^query_max_weight%^RESET%^
.EI
.SI 5
int query_max_weight()
.EI
.SP 7 5

This method returns the maximum amount of weight that can
be carried in this container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum weight
.EP

.SI 3
* %^BOLD%^query_ownership%^RESET%^
.EI
.SI 5
string query_ownership()
.EI
.SP 7 5

This returns who owns the container, if it set to 0 then no one
owns it.  This is used in the theft determination for the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
who owns the container
.EP

.SI 3
* %^BOLD%^query_player%^RESET%^
.EI
.SI 5
object query_player()
.EI
.SP 7 5

This method returns the player who is loading the container
during the autoload sequence.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the player who loaded the object

.EP

.SI 3
* %^BOLD%^query_prevent_insert%^RESET%^
.EI
.SI 5
int query_prevent_insert()
.EI
.SP 7 5

If this is true, this container cannot be put into other containers.
.EP

.SI 3
* %^BOLD%^query_tracked_items%^RESET%^
.EI
.SI 5
nomask int query_tracked_items()
.EI
.SP 7 5

This method returns the number of tracked item contained.
Tracked items want to be notified if the container moves.
.EP

.SI 3
* %^BOLD%^remove_tracked_items%^RESET%^
.EI
.SI 5
nomask void remove_tracked_items(int n_items)
.EI
.SP 7 5

This method handles the removal of tracked items.
Tracked items want to be notified if the container moves.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n_items - the number of tracked items  removed
.EP

.SI 3
* %^BOLD%^reset_prevent_insert%^RESET%^
.EI
.SI 5
int reset_prevent_insert()
.EI
.SP 7 5

This allows this container to be put into other containers (default).
.EP

.SI 3
* %^BOLD%^set_max_items%^RESET%^
.EI
.SI 5
void set_max_items(int number)
.EI
.SP 7 5

This method sets the maximum number of items that can
be carried in this container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new maximum number of items
.EP

.SI 3
* %^BOLD%^set_max_weight%^RESET%^
.EI
.SI 5
void set_max_weight(int number)
.EI
.SP 7 5

This method sets the maximum amount of weight that can
be carried in this container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new maximum weight
.EP

.SI 3
* %^BOLD%^set_ownership%^RESET%^
.EI
.SI 5
void set_ownership(string word)
.EI
.SP 7 5

This sets who owns the container, if it set to 0 then no one
owns it.  This is used in the theft determination for the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - who owns the container
.EP

.SI 3
* %^BOLD%^set_player%^RESET%^
.EI
.SI 5
void set_player(object thing)
.EI
.SP 7 5

This method is used in the auto loading sequence to set the
player who is loading the container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the player loading the container
.EP

.SI 3
* %^BOLD%^set_prevent_insert%^RESET%^
.EI
.SI 5
int set_prevent_insert()
.EI
.SP 7 5

items21 stops this container from being put into other containers.
.EP

.SI 3
* %^BOLD%^set_tracked_item_status_reason%^RESET%^
.EI
.SI 5
nomask void set_tracked_item_status_reason(string reason)
.EI
.SP 7 5

This method is called to adjust the status reason.
occurences.  
.EP

.SI 3
* %^BOLD%^test_add%^RESET%^
.EI
.SI 5
int test_add(object ob, int flag)
.EI
.SP 7 5

This method allows things to be added into us.  If we have an 
environment check that for the ability to add into us.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object being added
.EP
.SP 9 5
flag - the add flags
.EP

.SI 3
* %^BOLD%^test_remove%^RESET%^
.EI
.SI 5
int test_remove(object thing, int flag, mixed dest)
.EI
.SP 7 5

This method checks to see if the object can be taken out of
us.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object coming out
.EP
.SP 9 5
flag - the move flag
.EP
.SP 9 5
dest - the destination object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can be taken out, 0 if not.

.EP

.SI 3
* %^BOLD%^update_loc_weight%^RESET%^
.EI
.SI 5
void update_loc_weight()
.EI
.SP 7 5

This method determins the current local weight from all the
objects inside the container
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^handle_restore_inventory%^RESET%^
.EI
.SI 5
int handle_restore_inventory(object ob)
.EI
.SP 7 5

This method handles moving objects into the inventory from an auto
load.  This should be over ridden by things inheriting us to
make sure that the objects can be moved into the inventory.  It
should handle the bypassing of open/close/locked etc flags.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to move into ourselves

.EP


