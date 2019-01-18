.DT
surface_decorator.c
Disk World autodoc help
surface_decorator.c

.SH Description
.SP 5 5

 This is a inheritable for playerhousing surface decorating npcs.
 
 With this you can add carpets, wood panelling, beams or anything your 
 little heart desires to npcs that will then go out and make the world a    
 prettier place.

 If you want to add a surface to an npc that he will be able to 
 decorate, use eg:
 
 add_allowed_surface( "floor" );    

 If you want to add a type of decoration that the npc can install, use eg:
 
 add_fitted_surface( "plush blue carpet", "covered with plush "
 "blue carpet", <cost> ); @September 3rd, 2002 @authors Una and Dasquian 
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/monster.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_allowed_domain%^RESET%^
.EI
.SI 5
void add_allowed_domain(string domain)
.EI
.SP 7 5

Adds a string to the list of domains to be checked by the npc when it 
moves to see if it's allowed to be in that room, eg. "/d/guilds" or 
"Bes_Pelargic"

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
domain - the name of the domain to be added

.EP

.SI 3
* %^BOLD%^add_allowed_surface%^RESET%^
.EI
.SI 5
void add_allowed_surface(string surface)
.EI
.SP 7 5

Allows the npc to work on a specific surface in a room, eg. floor, ceiling
If the argument passed is "wall", they will be able to work on all walls,
eg. north wall, east wall, south wall, etc.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
surface - the name of the surface to be added

.EP

.SI 3
* %^BOLD%^add_fitted_surface%^RESET%^
.EI
.SI 5
void add_fitted_surface(string short, string long, int cost)
.EI
.SP 7 5

Adds a new decoration that the npc can use to his stock list

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short - the short description used in the stock list and for ordering
.EP
.SP 9 5
long - the long description actually used for the new decoration in 
the room
.EP
.SP 9 5
cost - the cost of installing this decoration

.EP

.SI 3
* %^BOLD%^check_domain%^RESET%^
.EI
.SI 5
void check_domain()
.EI
.SP 7 5

Called every time the npc moves.  This function checks that the file name 
of its new environment contains at least one of the allowed domains.  If
it does, it will store the current room as being its last good location.  
Otherwise, it will move it to its last good location.

.EP

.SI 3
* %^BOLD%^do_ask%^RESET%^
.EI
.SI 5
int do_ask(string surface, string decor)
.EI
.SP 7 5

Does all the necessary checks to see if he can decorate the requested 
surface with the requested decor, handles payment and starts the work. 

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
surface - the surface to be decorated 
.EP
.SP 9 5
decor - the short name of the decor to be used, eg. green carpet

.EP

.SI 3
* %^BOLD%^do_hire%^RESET%^
.EI
.SI 5
int do_hire()
.EI
.SP 7 5

Sets him to follow the player and marks that player as his current 
employer

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

Causes the surface decorator to give a list of the surfaces he decorates
and what he can decorate them with, or says that he has no stock.

.EP

.SI 3
* %^BOLD%^finish_work%^RESET%^
.EI
.SI 5
void finish_work(string surface, string long, object where)
.EI
.SP 7 5

Called by do_ask(), this finishes decorating the surface

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
surface - the surface to be decorated
.EP
.SP 9 5
long - the long description of the decorated surface, eg. carpeted   
with plush green carpet

.EP

.SI 3
* %^BOLD%^query_allowed_domains%^RESET%^
.EI
.SI 5
string * query_allowed_domains()
.EI

.SI 3
* %^BOLD%^query_allowed_surfaces%^RESET%^
.EI
.SI 5
string * query_allowed_surfaces()
.EI

.SI 3
* %^BOLD%^query_employer%^RESET%^
.EI
.SI 5
object query_employer()
.EI

.SI 3
* %^BOLD%^query_fitted_surfaces%^RESET%^
.EI
.SI 5
mapping query_fitted_surfaces()
.EI

.SI 3
* %^BOLD%^query_responses%^RESET%^
.EI
.SI 5
string * query_responses()
.EI

.SI 3
* %^BOLD%^query_under_construction_mess%^RESET%^
.EI
.SI 5
string query_under_construction_mess()
.EI

.SI 3
* %^BOLD%^query_work_time%^RESET%^
.EI
.SI 5
int query_work_time()
.EI

.SI 3
* %^BOLD%^remove_all_fitted_surfaces%^RESET%^
.EI
.SI 5
void remove_all_fitted_surfaces()
.EI
.SP 7 5

Removes all decorations from the npc's stock list

.EP

.SI 3
* %^BOLD%^remove_fitted_surface%^RESET%^
.EI
.SI 5
void remove_fitted_surface(string short)
.EI
.SP 7 5

Removes a decoration whose short matches the argument given

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short - the short description of the surface to remove

.EP

.SI 3
* %^BOLD%^set_employer%^RESET%^
.EI
.SI 5
void set_employer(object player)
.EI
.SP 7 5

Sets the current player employer

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player object currently using the npc

.EP

.SI 3
* %^BOLD%^set_responses%^RESET%^
.EI
.SI 5
void set_responses(string * the_responses)
.EI
.SP 7 5

Sets custom responses.  The array size must match the existing one or the  
call will be disregarded.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the_responses - the array of responses to be used

.EP

.SI 3
* %^BOLD%^set_under_construction_mess%^RESET%^
.EI
.SI 5
void set_under_construction_mess(string mess)
.EI
.SP 7 5

Sets what a surface looks like while the npc is working on it.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the description to replace the surface's appearance in the 
room while it is being worked on

.EP

.SI 3
* %^BOLD%^set_work_time%^RESET%^
.EI
.SI 5
void set_work_time(int time)
.EI
.SP 7 5

Sets the time it takes to finish the work.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
time - the time (in seconds) taken to complete the work

.EP


