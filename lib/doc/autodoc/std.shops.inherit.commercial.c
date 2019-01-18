.DT
commercial.c
Disk World autodoc help
commercial.c

.SH Description
.SP 5 5

This is the file that is the basic inheritable for commercial
properties.  The only different between commercial properties and
player housing properties is that commerical ones can deal with
signs and commercial furniture.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Apr 21 13:37:44 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/person_expressions, /std/shops/inherit/expression_util and /std/room/player_housing.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/move_failures.h, /include/player.h, /include/shops/bank.h, /include/mail.h, /include/expressions.h, /include/money.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^float_withdrawls%^RESET%^
class float_withdrawls {
string person;
int amount;
int withdrawl_time;
}

.EI

.SI 3
* %^BOLD%^royalty_data%^RESET%^
class royalty_data {
int value;
int last_withdrawl;
}

.EI

.SI 3
* %^BOLD%^weekly_status%^RESET%^
class weekly_status {
int total_incoming;
int total_outgoing;
class float_withdrawls * withdrawls;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_commercial_thing%^RESET%^
.EI
.SI 5
void add_commercial_thing(object ob)
.EI
.SP 7 5

This method adds a commercial thing into the list of current
commercial things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to add

.EP

.SI 3
* %^BOLD%^adjust_royalty%^RESET%^
.EI
.SI 5
void adjust_royalty(string person, int amount)
.EI
.SP 7 5

This method adds money into the specified person's royalty
float.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to add the royalty to
.EP
.SP 9 5
amount - the amount to add

.EP

.SI 3
* %^BOLD%^do_add_royalties%^RESET%^
.EI
.SI 5
int do_add_royalties(string amount)
.EI
.SP 7 5

Adds money into the float for the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount to add to the float

.EP

.SI 3
* %^BOLD%^do_black_list_add%^RESET%^
.EI
.SI 5
int do_black_list_add(string name)
.EI
.SP 7 5

This method adds the person to the black list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to add to the black list

.EP

.SI 3
* %^BOLD%^do_black_list_add_ignore%^RESET%^
.EI
.SI 5
int do_black_list_add_ignore()
.EI
.SP 7 5

This method adds their ignore list to the black list

.EP

.SI 3
* %^BOLD%^do_black_list_remove%^RESET%^
.EI
.SI 5
int do_black_list_remove(string name)
.EI
.SP 7 5

This method removes the person from the black list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove from the black list

.EP

.SI 3
* %^BOLD%^do_collect%^RESET%^
.EI
.SI 5
int do_collect()
.EI
.SP 7 5

This method shows the current royalties you can pick up.

.EP

.SI 3
* %^BOLD%^do_collect_partial_royalties%^RESET%^
.EI
.SI 5
int do_collect_partial_royalties(string amount)
.EI
.SP 7 5

This method collects royalities up to a certain amount.  This is
only usable by the owner of the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount to collect
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_deposit%^RESET%^
.EI
.SI 5
int do_deposit(object * obs)
.EI
.SP 7 5

This method deposits money into the float
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects which could be money

.EP

.SI 3
* %^BOLD%^do_measure%^RESET%^
.EI
.SI 5
int do_measure()
.EI

.SI 3
* %^BOLD%^do_name_property%^RESET%^
.EI
.SI 5
int do_name_property(string name)
.EI
.SP 7 5

This method gives the property a name.

.EP

.SI 3
* %^BOLD%^do_pay_entry%^RESET%^
.EI
.SI 5
int do_pay_entry()
.EI
.SP 7 5

This method lets the person pay for entry into the premesis.

.EP

.SI 3
* %^BOLD%^do_purchase%^RESET%^
.EI
.SI 5
int do_purchase()
.EI
.SP 7 5

This methof calls ups the estate person to purchase stuff off.

.EP

.SI 3
* %^BOLD%^do_remove_sign%^RESET%^
.EI
.SI 5
int do_remove_sign()
.EI
.SP 7 5

This method removes the sign from the property.

.EP

.SI 3
* %^BOLD%^do_royalties%^RESET%^
.EI
.SI 5
int do_royalties()
.EI
.SP 7 5

This method shows any royalties that you might be able to collect.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP

.SI 3
* %^BOLD%^do_royalty_list%^RESET%^
.EI
.SI 5
int do_royalty_list()
.EI
.SP 7 5

This method shows any royalties that you might be able to collect.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP

.SI 3
* %^BOLD%^do_set_sign%^RESET%^
.EI
.SI 5
int do_set_sign(object * obs)
.EI
.SP 7 5

This method sets the sign you wish to use for your property.
The sign will be displayed outside the property for passer bys.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the sign

.EP

.SI 3
* %^BOLD%^do_use_expression%^RESET%^
.EI
.SI 5
int do_use_expression(string str)
.EI
.SP 7 5

This method sets up the expression to use when calculating the
ability to use the shop.

.EP

.SI 3
* %^BOLD%^do_use_expression_type%^RESET%^
.EI
.SI 5
int do_use_expression_type(string type, string str)
.EI
.SP 7 5

This method sets up the expression to use when calculating the
ability to use the shop.

.EP

.SI 3
* %^BOLD%^find_commercial_items%^RESET%^
.EI
.SI 5
object * find_commercial_items(string type)
.EI
.SP 7 5

This function will try and track down a specific type of commercial
object in the current location.  It will look into 'partitioned'
rooms for the item as well.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to look for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object found

.EP

.SI 3
* %^BOLD%^find_next_commercial_id%^RESET%^
.EI
.SI 5
int find_next_commercial_id()
.EI
.SP 7 5

This method finds a unique commercial id in the room to use as new
save file.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a unique commercial id for the room

.EP

.SI 3
* %^BOLD%^is_open_for%^RESET%^
.EI
.SI 5
int is_open_for(string type, string person)
.EI
.SP 7 5

This method is called to try and see if the property is open for
the specific type of thingy attempted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of thing

.EP

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string word, int dark)
.EI
.SP 7 5

This gets round the problems with setting the property name before
we have a save file.

.EP

.SI 3
* %^BOLD%^query_commercial_property%^RESET%^
.EI
.SI 5
int query_commercial_property()
.EI
.SP 7 5

This method returns true for any commercial property.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for commercial property

.EP

.SI 3
* %^BOLD%^query_commercial_things%^RESET%^
.EI
.SI 5
object * query_commercial_things()
.EI
.SP 7 5

This method returns the current list of commercial things in
the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of commercial things

.EP

.SI 3
* %^BOLD%^query_commercial_type%^RESET%^
.EI
.SI 5
string query_commercial_type()
.EI
.SP 7 5

This method returns the commercial type of the property.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the commercial type of the property

.EP

.SI 3
* %^BOLD%^query_float%^RESET%^
.EI
.SI 5
int query_float()
.EI
.SP 7 5

This method returns the float of the property.  The float is the
owner's current royalty balance.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the float of the property

.EP

.SI 3
* %^BOLD%^query_language%^RESET%^
.EI
.SI 5
string query_language()
.EI
.SP 7 5

This method returns the language items in the shop should use if they
need to use a language.

.EP

.SI 3
* %^BOLD%^query_main_room%^RESET%^
.EI
.SI 5
object query_main_room()
.EI
.SP 7 5

This method checks returns the main room.  THis is used when the
property can have multiple sub-rooms.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the main room

.EP

.SI 3
* %^BOLD%^query_owners_money%^RESET%^
.EI
.SI 5
int query_owners_money()
.EI
.SP 7 5

This method figures out how much float the owner has in the current
property.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current float

.EP

.SI 3
* %^BOLD%^query_parcel_post%^RESET%^
.EI
.SI 5
string query_parcel_post()
.EI
.SP 7 5

This method tells us the current parcel post used for rejecting items.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current parcel post

.EP

.SI 3
* %^BOLD%^query_property_name%^RESET%^
.EI
.SI 5
string query_property_name()
.EI
.SP 7 5

This method returns the name of the property.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the property

.EP

.SI 3
* %^BOLD%^query_property_size%^RESET%^
.EI
.SI 5
int query_property_size()
.EI
.SP 7 5

This method returns the size of the property.  The property size ranges
from 0 (no size) to 100 (big shop).
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the property size

.EP

.SI 3
* %^BOLD%^query_royalty%^RESET%^
.EI
.SI 5
int query_royalty(string person)
.EI
.SP 7 5

This method returns the amount of money in the specified person's
royalty float.

.EP

.SI 3
* %^BOLD%^query_save_dir%^RESET%^
.EI
.SI 5
string query_save_dir()
.EI
.SP 7 5

This method returns the save directory for the room.  This should be
used as a reference point for other save stuff.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save directory

.EP

.SI 3
* %^BOLD%^query_sign_object%^RESET%^
.EI
.SI 5
object query_sign_object()
.EI
.SP 7 5

This method returns the current sign object for the property.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current sign object

.EP

.SI 3
* %^BOLD%^query_weekly_status_string%^RESET%^
.EI
.SI 5
string query_weekly_status_string()
.EI
.SP 7 5

This method returns the string that will be sent to the owners at the
end of the week.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the weekly status

.EP

.SI 3
* %^BOLD%^register_use_types%^RESET%^
.EI
.SI 5
void register_use_types(string * types)
.EI
.SP 7 5

This registers a set of use types so they can be seen and controlled.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
types - the list of types to register

.EP

.SI 3
* %^BOLD%^remove_commercial_thing%^RESET%^
.EI
.SI 5
void remove_commercial_thing(object thing)
.EI
.SP 7 5

This method removes a commercil thing from the list of current
commercial things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to remove

.EP

.SI 3
* %^BOLD%^reset_weekly_status%^RESET%^
.EI
.SI 5
void reset_weekly_status()
.EI
.SP 7 5

This method resets the weekly status when the stats are mailed out.
It will also reset all the commercial objects.

.EP

.SI 3
* %^BOLD%^set_commercial_type%^RESET%^
.EI
.SI 5
void set_commercial_type(string name)
.EI
.SP 7 5

This method sets the commercial type of the property.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the commecial type

.EP

.SI 3
* %^BOLD%^set_language%^RESET%^
.EI
.SI 5
void set_language(string language)
.EI
.SP 7 5

This sets the language items in the shop should be displayed in,
if a language is required.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
language - the language to use

.EP

.SI 3
* %^BOLD%^set_parcel_post%^RESET%^
.EI
.SI 5
void set_parcel_post(string parcel)
.EI
.SP 7 5

This method sets the parcel post to use for rejecting items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
parcel - the parcel post office

.EP

.SI 3
* %^BOLD%^set_property_name%^RESET%^
.EI
.SI 5
void set_property_name(string name)
.EI
.SP 7 5

This method sets the name of the property.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the property

.EP

.SI 3
* %^BOLD%^set_property_size%^RESET%^
.EI
.SI 5
void set_property_size(int size)
.EI
.SP 7 5

This method sets the size of the property.  The property size ranges
from 0 (no size) to 100 (big shop).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
size - the new size of the property

.EP

.SI 3
* %^BOLD%^set_save_dir%^RESET%^
.EI
.SI 5
void set_save_dir(string dir)
.EI
.SP 7 5

This method sets the save directory for the the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to save stuff in

.EP

.SI 3
* %^BOLD%^set_sign_object%^RESET%^
.EI
.SI 5
void set_sign_object(object ob)
.EI
.SP 7 5

This method sets the current sign object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sign - the new sign object

.EP

.SI 3
* %^BOLD%^short%^RESET%^
.EI
.SI 5
string short()
.EI
.SP 7 5

This gets round the problems with setting the property name before
we have a save file.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^mail_weekly_status%^RESET%^
.EI
.SI 5
void mail_weekly_status()
.EI
.SP 7 5

This method will mail out the weekly status for the shop.

.EP


