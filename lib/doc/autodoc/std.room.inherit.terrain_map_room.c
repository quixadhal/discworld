.DT
terrain_map_room.c
Disk World autodoc help
terrain_map_room.c

.SH Description
.SP 5 5

The overall room inherit for a 'mapped area'
all mapped area rooms should inherit from this.


MUST OVERRIDE: 

query_map_file()
.EP
.SP 10 5


Written by  Dek

Started March 2001

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/login_handler.h and /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_feature%^RESET%^
.EI
.SI 5
void add_feature(string title, string direc, int range, mixed items)
.EI
.SP 7 5

Add a new visible feature to the room, so it knows to query the handler
for descriptions and so on. The zone info is needed as an easy way for
the handler to get access to which rooms see which features. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title of the feature
.EP
.SP 9 5
direc - the direction of the feature
.EP
.SP 9 5
range - the range to the feature
.EP
.SP 9 5
items - any add_items associated with the feature

.EP

.SI 3
* %^BOLD%^can_enter_from%^RESET%^
.EI
.SI 5
int can_enter_from(string type)
.EI
.SP 7 5

Can we enter from rooms of type x? Override if you want to use it - 
1 means yes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of room to check

.EP

.SI 3
* %^BOLD%^can_exit_to%^RESET%^
.EI
.SI 5
int can_exit_to(string type)
.EI
.SP 7 5

Can we go to rooms of type x? Override if you want to use it - 
1 means yes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of room to check

.EP

.SI 3
* %^BOLD%^can_view_feature%^RESET%^
.EI
.SI 5
int can_view_feature(string type)
.EI
.SP 7 5

Checks to see if the feature can be viewed or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the feature
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can be seen

.EP

.SI 3
* %^BOLD%^do_a_feature_chat%^RESET%^
.EI
.SI 5
void do_a_feature_chat()
.EI
.SP 7 5

This makes one of the features in the terrain send us a chat.

.EP

.SI 3
* %^BOLD%^do_an_outside_chat%^RESET%^
.EI
.SI 5
void do_an_outside_chat()
.EI
.SP 7 5

This method will print an outside chat.

.EP

.SI 3
* %^BOLD%^do_gocoords%^RESET%^
.EI
.SI 5
int do_gocoords(int x, int y)
.EI
.SP 7 5

This is a command used by creators to go to specific locations in the grid.

.EP

.SI 3
* %^BOLD%^do_outside_chats%^RESET%^
.EI
.SI 5
int do_outside_chats()
.EI
.SP 7 5

Do we *want* outside and feature chats? Override if you want to use 
1 means yes.

.EP

.SI 3
* %^BOLD%^query_coords%^RESET%^
.EI
.SI 5
mixed * query_coords()
.EI
.SP 7 5

This method returns the co-ordinates of this room.

.EP

.SI 3
* %^BOLD%^query_feature_item_desc%^RESET%^
.EI
.SI 5
string query_feature_item_desc(string title)
.EI
.SP 7 5

This method returns the item description for the feature.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the feature to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the description of the feature

.EP

.SI 3
* %^BOLD%^query_feature_range%^RESET%^
.EI
.SI 5
int query_feature_range(string title)
.EI
.SP 7 5

This figures out the range to the specified feature.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature

.EP

.SI 3
* %^BOLD%^query_features%^RESET%^
.EI
.SI 5
mapping query_features()
.EI
.SP 7 5

This method returns all of our features.

.EP

.SI 3
* %^BOLD%^query_installed%^RESET%^
.EI
.SI 5
int query_installed()
.EI

.SI 3
* %^BOLD%^query_map_handler%^RESET%^
.EI
.SI 5
object query_map_handler()
.EI
.SP 7 5

This method returns the map handler associated with this room.

.EP

.SI 3
* %^BOLD%^receive_message%^RESET%^
.EI
.SI 5
void receive_message(string mess_class, string message)
.EI
.SP 7 5

Receive messages from the handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
message - the actual message

.EP

.SI 3
* %^BOLD%^remove_feature%^RESET%^
.EI
.SI 5
void remove_feature(string title)
.EI
.SP 7 5

Removes the specified feature.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the feature to remove

.EP

.SI 3
* %^BOLD%^reset_handler%^RESET%^
.EI
.SI 5
void reset_handler()
.EI
.SP 7 5

Added for convenience. BEWARE! When called, this will dest every single 
room in this terrain, dumping everything in the void. When I think of
a decent way to handle this, I will.

.EP

.SI 3
* %^BOLD%^set_coords%^RESET%^
.EI
.SI 5
void set_coords(int x, int y)
.EI
.SP 7 5

Setup the main coordinates for this room. Please note that these
are not DW coords, but local ones for the use of the handler. 

.EP

.SI 3
* %^BOLD%^set_extra_long%^RESET%^
.EI
.SI 5
void set_extra_long(string extra)
.EI
.SP 7 5

Called by the handler to set our extra_look strings.

.EP

.SI 3
* %^BOLD%^set_installed%^RESET%^
.EI
.SI 5
void set_installed()
.EI

.SI 3
* %^BOLD%^set_map_handler%^RESET%^
.EI
.SI 5
void set_map_handler(mixed handler)
.EI
.SP 7 5

This sets the map handler, OBSOLETE. This should be done by overriding query_map_handler()
in the main inherited file. It doesn't get set fast enough otherwise.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
handler - the new handler

.EP

.SI 3
* %^BOLD%^set_outside_types%^RESET%^
.EI
.SI 5
void set_outside_types(string types)
.EI

.SI 3
* %^BOLD%^setup_room%^RESET%^
.EI
.SI 5
void setup_room()
.EI
.SP 7 5

This needs to be called by the upper inherit to set us to womble.

.EP

.SI 3
* %^BOLD%^setup_room_chat%^RESET%^
.EI
.SI 5
varargs void setup_room_chat(mixed * args, object chatobj)
.EI
.SP 7 5

Setup the feature chats and basic room chats.

.EP


