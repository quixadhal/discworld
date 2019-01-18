.DT
pawn_receipt.c
Disk World autodoc help
pawn_receipt.c

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/misc/paper.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h, /include/player.h, /include/move_failures.h, /include/shops/bank.h, /include/am_time.h, /include/armoury.h, /include/playtesters.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_reclaim%^RESET%^
.EI
.SI 5
int do_reclaim()
.EI
.SP 7 5

This handles all the reclaiming of objects that have been pawned.

.EP

.SI 3
* %^BOLD%^pluralize_sentence%^RESET%^
.EI
.SI 5
string pluralize_sentence(string text, int plural)
.EI
.SP 7 5

This takes a sentence and pluralises it appropriately.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
plural - Whether or not the sentence should be in plural form.

.EP

.SI 3
* %^BOLD%^query_correct_short%^RESET%^
.EI
.SI 5
string query_correct_short(object ob)
.EI
.SP 7 5

This returns a short which is accurate for both ordinary and
collective objects.

.EP

.SI 3
* %^BOLD%^query_expiry_time%^RESET%^
.EI
.SI 5
int query_expiry_time()
.EI
.SP 7 5

This returns the Unix time in seconds at which this receipt will expire.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[int] The time of death in seconds.

.EP

.SI 3
* %^BOLD%^query_item_value%^RESET%^
.EI
.SI 5
int query_item_value()
.EI
.SP 7 5

This returns the value of the item this receipt was based on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[int] the value of the original item.

.EP

.SI 3
* %^BOLD%^query_object_plural%^RESET%^
.EI
.SI 5
int query_object_plural()
.EI
.SP 7 5

This lets us check whether or not our object is a plural object.

.EP

.SI 3
* %^BOLD%^query_obname%^RESET%^
.EI
.SI 5
string query_obname()
.EI
.SP 7 5

This method returns the name of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the object

.EP

.SI 3
* %^BOLD%^query_obpath%^RESET%^
.EI
.SI 5
string query_obpath()
.EI
.SP 7 5

This method returns the path of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the object

.EP

.SI 3
* %^BOLD%^query_obshort%^RESET%^
.EI
.SI 5
string query_obshort()
.EI
.SP 7 5

This returns the short of the pawned object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[string] The short of the pawned object.

.EP

.SI 3
* %^BOLD%^query_pawned_item_receipt%^RESET%^
.EI
.SI 5
int query_pawned_item_receipt()
.EI
.SP 7 5

This allows us to check if the object is a pawned item receipt.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the object is a pawned item receipt.

.EP

.SI 3
* %^BOLD%^query_read_mess%^RESET%^
.EI
.SI 5
mixed * query_read_mess()
.EI
.SP 7 5

This sets up a message for us to get when we read the receipt.
Because we're using a function pointer, add_read_mess doesn't work.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - The language to produce the message in.

.EP

.SI 3
* %^BOLD%^query_shop_path%^RESET%^
.EI
.SI 5
string query_shop_path()
.EI
.SP 7 5

This returns the path to the shop that this receipt was spawned by.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[string] the path to the cloning shop.

.EP

.SI 3
* %^BOLD%^query_static_save%^RESET%^
.EI
.SI 5
mixed query_static_save()
.EI
.SP 7 5

This method returns the save information for the object.  THis is not
just the static save information, it is an array of two elements.  The
first is the static save information and the second is the dynamic
save information.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save information

.EP

.SI 3
* %^BOLD%^query_timestamp%^RESET%^
.EI
.SI 5
int query_timestamp()
.EI
.SP 7 5

This method returns the timestamp of the reciept.  This is the time the
receipt was created.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the timestamp

.EP

.SI 3
* %^BOLD%^query_virt_obname%^RESET%^
.EI
.SI 5
string query_virt_obname()
.EI
.SP 7 5

This method returns the virtual object path name of the item.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the virtual object name

.EP

.SI 3
* %^BOLD%^set_expiry_time%^RESET%^
.EI
.SI 5
void set_expiry_time(int time)
.EI
.SP 7 5

This sets the time at which this receipt will die.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
time - The time in seconds when this receipt will be killed.

.EP

.SI 3
* %^BOLD%^set_item_value%^RESET%^
.EI
.SI 5
void set_item_value(int value)
.EI
.SP 7 5

This sets the value of the item this receipt is based on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - The value of the original item.

.EP

.SI 3
* %^BOLD%^set_object%^RESET%^
.EI
.SI 5
void set_object(string path)
.EI
.SP 7 5

Sets the path to the object.  The path of the object for the recipt to
work with.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the object path

.EP

.SI 3
* %^BOLD%^set_object_plural%^RESET%^
.EI
.SI 5
void set_object_plural(object ob)
.EI
.SP 7 5

This sets whether or not our object is actually a collective,
and so more than one object.

.EP

.SI 3
* %^BOLD%^set_obname%^RESET%^
.EI
.SI 5
void set_obname(string name)
.EI
.SP 7 5

Sets the name of the object.  This will be used so the item recipt
can be identified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the object name

.EP

.SI 3
* %^BOLD%^set_obshort%^RESET%^
.EI
.SI 5
void set_obshort(string s)
.EI
.SP 7 5

This sets the short of the pawned object.

.EP

.SI 3
* %^BOLD%^set_shop_path%^RESET%^
.EI
.SI 5
void set_shop_path(mixed shop)
.EI
.SP 7 5

This allows us to set the path of the shop that spawned us, so that this
receipt can only be redeemed at that shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
shop - - either a shop object or a string path.

.EP

.SI 3
* %^BOLD%^set_static_save%^RESET%^
.EI
.SI 5
void set_static_save(mixed args)
.EI
.SP 7 5

This method sets the save information for the file.  It actually sets
both the static and dynamic information.  This assumes it is passed
an array of two elements, the first is the static save information and
the second is the dynamic save information.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the data as defined above

.EP

.SI 3
* %^BOLD%^set_virtobname%^RESET%^
.EI
.SI 5
void set_virtobname(string name)
.EI
.SP 7 5

This method sets the virtual object name of the item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the virtual object name

.EP

.SI 3
* %^BOLD%^setup_receipt%^RESET%^
.EI
.SI 5
void setup_receipt(object ob)
.EI
.SP 7 5

This method sets up the receipt for the specified object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to setup the reciept for

.EP


