.DT
store_close.c
Disk World autodoc help
store_close.c

.SH Description
.SP 5 5

The inherit file for store closings. This should be inherited along with 
 /std/shop or /std/item-shop. Or any other room of your choice, really.
 @author Rodion

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h, /include/climate.h and /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^close_up%^RESET%^
.EI
.SI 5
void close_up()
.EI
.SP 7 5

This method closes the shop! It sends the shopkeeper home, changes the 
incoming exits to be closed and locked, and moves all players in the store 
out.

.EP

.SI 3
* %^BOLD%^open_up%^RESET%^
.EI
.SI 5
void open_up()
.EI
.SP 7 5

This method opens the store, summoning the shopkeeper from the void and
unlocking and opening the various exits.

.EP

.SI 3
* %^BOLD%^set_night_close%^RESET%^
.EI
.SI 5
void set_night_close(int night, string shopkeeper, string * outside, string burglar_func)
.EI
.SP 7 5

This function sets up the closing code for stores.  This allows you to 
 close and lock up the place, update external rooms, send the shopkeepers 
 home, and catch burglars who try to sneak in.
 

If the "night" flag is 1, the store will close at night. If it's 0, the store will close during the day. 

The shopkeeper name should be the string filename of the shopkeeper. This can be set to "none" for no shopkeeper (regular event_person_say stuff will be used to simulate it.) 

*outside is an array of exit, room path pairs that connect to the shop. That is, if you moved east to get into the shop, the exit string will be "east", "/path/of/room/we/got/here/from". THIS IS REQUIRED, so that players will be moved to the right place. 

The burglar_func parameter is the name of a function that will be called when someone enters the shop after it is closed. It will be called with the argument object ob, where ob is the person who entered. For no burglar handling, this argument can be 0. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
night - Whether the store closes during the night or day.
.EP
.SP 9 5
shopkeeper - The file name of the shopkeeper for the store.
.EP
.SP 9 5
outside - The exit, room pairs corresponding to outside rooms.
.EP
.SP 9 5
burglar_func - The function called when store is burgled.
.EP


