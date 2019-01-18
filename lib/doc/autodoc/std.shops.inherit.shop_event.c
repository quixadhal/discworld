.DT
shop_event.c
Disk World autodoc help
shop_event.c

.SH Description
.SP 5 5

This inheritable allows broadcasting of shop events to whoever
wants to hear them.  The most useful case of this is broadcasting
to yourself so you can tell when shops do specific things.  The
shop event will always be called 'event_shop' + type.  For example
'event_shop_failed_buy'.  Each specific
shop will setup different parameters to the shop event, except
the first three parameters will always be the event name,
shop object and the third is the player.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Oct 23 12:23:41 PDT 2000

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_shop_observer%^RESET%^
.EI
.SI 5
void add_shop_observer(object observer)
.EI
.SP 7 5

This method adds an object into the list to receive shop events.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
observer - the observer to add

.EP

.SI 3
* %^BOLD%^broadcast_shop_event%^RESET%^
.EI
.SI 5
int broadcast_shop_event(string type, object player, mixed args ...)
.EI
.SP 7 5

This method sends the event to all the people who have been set to
receive shop events.  The shop event function is called
event_shop(object shop, ...).


If this method returns 1 then the default messages are supressed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the event
.EP
.SP 9 5
player - the player in the event
.EP
.SP 9 5
args - the arguements in the event
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 to supress the default messages

.EP

.SI 3
* %^BOLD%^remove_shop_observer%^RESET%^
.EI
.SI 5
void remove_shop_observer(object observer)
.EI
.SP 7 5

This method removes an object from the list to recieve shop events.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
observer - the observer to remove

.EP


