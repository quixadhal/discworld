.DT
virtual_quit_control.c
Disk World autodoc help
virtual_quit_control.c

.SH Description
.SP 5 5

This method handles problems where players log out in places which are
virtually controlled.  It does the effort to put them back in, but does
it in a way that is recursive.  So if they are on a horse in a
terrain it will work properly.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Sep 22 18:24:40 PDT 1999
.EP

.SH Example
.SI 5
inherit "/std/basic/virtual_quit_control";

void player_quitting(object player, object place) {
   string pos;

   pos = check_start_pos(environment(place));
   // Doesn't need to last long since we save immediately.
   player->add_property(START_PLACE, pos, 10);
   ::player_quitting(player, environment(place));
} /* player_quitting() */

// Move the player to the right spot when they login.
void event_enter(object player) {
   player->move(player->query_property(START_PLACE));
} /* move_player_to_real_location() */

.EI
.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^find_start_coord%^RESET%^
.EI
.SI 5
int * find_start_coord(object player, object place)
.EI
.SP 7 5

This method is called on the quit handler when a player quits.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player
.EP
.SP 9 5
place - the place they quit from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last position (if that needs to be saved).

.EP

.SI 3
* %^BOLD%^find_start_pos%^RESET%^
.EI
.SI 5
string find_start_pos(object player, object place)
.EI
.SP 7 5

This method is called on the quit handler when a player quits.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player
.EP
.SP 9 5
place - the place they quit from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last position (if that needs to be saved).

.EP


