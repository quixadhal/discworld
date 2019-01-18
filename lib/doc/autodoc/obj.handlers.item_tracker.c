.DT
item_tracker.c
Disk World autodoc help
item_tracker.c

.SH Description
.SP 5 5

This handler keeps track of unique (hopefully) per player items in the game.
.EP
.SP 10 5


Written by Shaggy

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/quest_handler.h and /include/library.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^delete_item%^RESET%^
.EI
.SI 5
int delete_item(mixed pl_name, mixed item)
.EI
.SP 7 5

This method deletes the state/location information of a specific quest item
owned by a player.


If the item isn't actually owned by the player it returns 0
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP
.SP 9 5
item -      item object or identifier
.EP

.SI 3
* %^BOLD%^query_all_item_states%^RESET%^
.EI
.SI 5
mixed query_all_item_states(mixed pl_name)
.EI
.SP 7 5

This method returns all the information about quest items
owned by a player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP

.SI 3
* %^BOLD%^query_item_copies%^RESET%^
.EI
.SI 5
int query_item_copies(mixed pl_name, mixed item)
.EI
.SP 7 5

This method returns the total number of known copies of a specific quest item
owned by a player.


If the item isn't actually owned by the player it doesn't matter.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP
.SP 9 5
item -      item object or identifier
.EP

.SI 3
* %^BOLD%^query_item_state%^RESET%^
.EI
.SI 5
mixed query_item_state(mixed pl_name, mixed item)
.EI
.SP 7 5

This method returns the state/location of a specific quest item
owned by a player.


If the item isn't actually owned by the player it returns 0
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP
.SP 9 5
item -      item object or identifier
.EP

.SI 3
* %^BOLD%^save_all_item_states%^RESET%^
.EI
.SI 5
int save_all_item_states(mixed pl_name)
.EI
.SP 7 5

This method saves the mapping of state/location 
information for items owned by a player to disk.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP
.SP 9 5
item -      item object or identifier
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful
.EP

.SI 3
* %^BOLD%^save_all_item_states_delete_cache%^RESET%^
.EI
.SI 5
int save_all_item_states_delete_cache(mixed pl_name)
.EI
.SP 7 5

This method saves the mapping of state/location 
information for items owned by a player to disk and deletes
the entry in the cache.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP
.SP 9 5
item -      item object or identifier
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful
.EP

.SI 3
* %^BOLD%^set_item_state%^RESET%^
.EI
.SI 5
int set_item_state(mixed pl_name, mixed item, mixed state)
.EI
.SP 7 5

This method sets the state/location of a specific quest item
owned by a player.


If the item isn't actually owned by the player it returns 0
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name -   player object or name
.EP
.SP 9 5
item -      item object or identifier
.EP


