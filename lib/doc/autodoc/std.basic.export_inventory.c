.DT
export_inventory.c
Disk World autodoc help
export_inventory.c

.SH Description
.SP 5 5

This file contains the control functions for exporting your inventory.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Mar  5 21:03:40 CST 1998

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_inventory_container%^RESET%^
.EI
.SI 5
void add_inventory_container(object cont)
.EI
.SP 7 5

This method adds a container to the list of registered containers.
A registered container will be used to enhance the inventory of
the current container.  This will only be used with find_match
inventory items.  The item *must* be contained inside this object
to be added.  When checking only visible added objects will have
their inventory added.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cont - the new container to add
.EP

.SI 3
* %^BOLD%^query_can_export_inventory%^RESET%^
.EI
.SI 5
int query_can_export_inventory()
.EI
.SP 7 5

This method returns if the container will export its inventory.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the container does export its inventory, 0 if not

.EP

.SI 3
* %^BOLD%^remove_inventory_container%^RESET%^
.EI
.SI 5
void remove_inventory_container(object cont)
.EI
.SP 7 5

This method removes a container to the list of registered containers.
A registered container will be used to enhance the inventory of
the current container.  This will only be used with find_match
inventory items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cont - the new container to add
.EP

.SI 3
* %^BOLD%^reset_can_export_inventory%^RESET%^
.EI
.SI 5
void reset_can_export_inventory()
.EI
.SP 7 5

This method disables the object from exporting its inventory.
.EP

.SI 3
* %^BOLD%^set_can_export_inventory%^RESET%^
.EI
.SI 5
void set_can_export_inventory()
.EI
.SP 7 5

This method allows the objects inside it (which are registered) to
export their invetories as well.  You do not want to be able to do
this with player or living objects (for instance).
The default value for this is reset()
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^find_expanded_inv_match%^RESET%^
.EI
.SI 5
object * find_expanded_inv_match(string words, object looker, object * things)
.EI
.SP 7 5

This method is not designed to be called from outside
the object itself.  It returns the list of objects able to
be matched that are container in containers in the container.
The third parameter is the set of objects the container
must appear in to be used.  This has to be handled sperately
so thet the plural matchin does not return the ones deep
inside your containers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the words which were matched with
.EP
.SP 9 5
looker - the person doing the matching
.EP
.SP 9 5
things - the current list of things matched.

.EP


