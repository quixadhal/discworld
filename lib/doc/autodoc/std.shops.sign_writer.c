.DT
sign_writer.c
Disk World autodoc help
sign_writer.c

.SH Description
.SP 5 5

This method checks to see if the shop is open.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^sign_making%^RESET%^
class sign_making {
int type;
string mess;
string colour;
int pick_up;
}

.EI

.SI 3
* %^BOLD%^sign_type%^RESET%^
class sign_type {
string short;
string long;
int cost;
int weight;
int max_letters;
int max_lines;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_paint_colour%^RESET%^
.EI
.SI 5
void add_paint_colour(string colour, int cost)
.EI
.SP 7 5

This method adds a paint colour to the shop and how much that 
colour costs to use.  The cost for the colour is the cost per
letter.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
colour - the colour of the paint
.EP
.SP 9 5
cost - the cost of the paint

.EP

.SI 3
* %^BOLD%^add_sign_making%^RESET%^
.EI
.SI 5
void add_sign_making(string owner, int type, string mess, string colour, int pick_up)
.EI
.SP 7 5

This method adds a sign into the current list of signs to be
made.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
owner - the person who owns the sign
.EP
.SP 9 5
type - the type of the sign
.EP
.SP 9 5
mess - the message on the sign
.EP
.SP 9 5
colour - the colour of the sign

.EP

.SI 3
* %^BOLD%^add_sign_type%^RESET%^
.EI
.SI 5
void add_sign_type(string short, string long, int cost, int weight, int max_letters, int max_lines)
.EI
.SP 7 5

This method adds the types of signs that are available.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short - the short description of the sign
.EP
.SP 9 5
cost - the base cost of the sign
.EP
.SP 9 5
weight - how heavy the sign is
.EP
.SP 9 5
max_letters - most letters
.EP
.SP 9 5
max_lines - the most lines

.EP

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(string type)
.EI
.SP 7 5

This method browses one of the sign types.  THis shows detailed
information about the sign type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to browse

.EP

.SI 3
* %^BOLD%^do_collect%^RESET%^
.EI
.SI 5
int do_collect()
.EI
.SP 7 5

This method is called when the player comes back to pick up a sign
(or signs) that are being made.

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

This method lists all the current sign types.

.EP

.SI 3
* %^BOLD%^do_order%^RESET%^
.EI
.SI 5
int do_order(string type)
.EI
.SP 7 5

This method is called when the player orders a sign.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of sign to order
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success 0 on failure

.EP

.SI 3
* %^BOLD%^is_open%^RESET%^
.EI
.SI 5
int is_open()
.EI

.SI 3
* %^BOLD%^query_language%^RESET%^
.EI
.SI 5
string query_language()
.EI
.SP 7 5

This method returns the language the shop works in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the language the shop works in

.EP

.SI 3
* %^BOLD%^query_open_func%^RESET%^
.EI
.SI 5
function query_open_func()
.EI
.SP 7 5

This method returns the current function used to test to see
if the shop is open or not.

.EP

.SI 3
* %^BOLD%^query_save_file_name%^RESET%^
.EI
.SI 5
string query_save_file_name()
.EI
.SP 7 5

THis method returns the current save file name of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save file name of the shop

.EP

.SI 3
* %^BOLD%^set_language%^RESET%^
.EI
.SI 5
void set_language(string lang)
.EI
.SP 7 5

This method sets the language the shop works in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
language - the language the shop uses

.EP

.SI 3
* %^BOLD%^set_open_func%^RESET%^
.EI
.SI 5
void set_open_func(function func)
.EI
.SP 7 5

This method sets a function to call to test to see if the shop
is open.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
open_func - the function to call

.EP

.SI 3
* %^BOLD%^set_save_file_name%^RESET%^
.EI
.SI 5
void set_save_file_name(string save)
.EI
.SP 7 5

This method sets the save file for the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
save_file - the save file name

.EP


