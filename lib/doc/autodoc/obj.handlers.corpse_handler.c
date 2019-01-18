.DT
corpse_handler.c
Disk World autodoc help
corpse_handler.c

.SH Description
.SP 5 5

A small handler to keep track of corpses.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jun 13 17:22:06 PDT 2002

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^corpse_data%^RESET%^
class corpse_data {
int id;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_corpses%^RESET%^
.EI
.SI 5
void check_corpses()
.EI
.SP 7 5

This goes through the current list of corpses and tries to find them,
if not it creates them.

.EP

.SI 3
* %^BOLD%^create_corpse%^RESET%^
.EI
.SI 5
object create_corpse(string name, int id)
.EI
.SP 7 5

This method creates the specific corpse.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player name to create it for
.EP
.SP 9 5
id - the id to create
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new corpse

.EP

.SI 3
* %^BOLD%^deregister_corpse%^RESET%^
.EI
.SI 5
void deregister_corpse(object ob)
.EI
.SP 7 5

This method deregisters the corpse from us.  We forget about it and
it wombles off into the ether.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
corpse - the corpse to deregister

.EP

.SI 3
* %^BOLD%^find_player_corpses%^RESET%^
.EI
.SI 5
object * find_player_corpses(string player)
.EI
.SP 7 5

Finds all of the corpses for the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the player to find the corpses of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of player corpses

.EP

.SI 3
* %^BOLD%^query_next_corpse_id%^RESET%^
.EI
.SI 5
int query_next_corpse_id()
.EI
.SP 7 5

This method returns the next corpse id.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the next corpse id

.EP

.SI 3
* %^BOLD%^register_corpse%^RESET%^
.EI
.SI 5
void register_corpse(object ob)
.EI
.SP 7 5

This method registers the corpse with us.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
corpse - the corpse to register

.EP

.SI 3
* %^BOLD%^save_corpse%^RESET%^
.EI
.SI 5
void save_corpse(object ob)
.EI
.SP 7 5

This method will save the corpse data out to a nice file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to save

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


