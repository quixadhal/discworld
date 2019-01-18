.DT
print_shop.c
Disk World autodoc help
print_shop.c

.SH Description
.SP 5 5

A printing workshop so that players can create their own books runs.


This can be divided up into two rooms, an office (where you pay for the printing to be done) and a collection room (where you do the collection). To enable this behaviour the collection room needs to know about the office. The office stores all the information, so you call set_office_room(room_path); on the collection room and set_no_collection() on the office. 
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jan 29 17:55:47 CST 1998
.EP



.SH See also
.SP 5 5
set_office_room() and set_no_collection();

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/language.h, /include/book_handler.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_print_run%^RESET%^
.EI
.SI 5
void add_print_run(string name, object ob, int number, string language)
.EI
.SP 7 5

This adds an object into the current set to be collected.  This handles
figuring out the nice number to use for the static and dynamic save
numbers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the person adding the run
.EP
.SP 9 5
ob - the object being added
.EP
.SP 9 5
cost - the cost of the object
.EP
.SP 9 5
number - the number to be printer
.EP

.SI 3
* %^BOLD%^add_translation_cost%^RESET%^
.EI
.SI 5
void add_translation_cost(string lang, int cost)
.EI
.SP 7 5

This method will add the cost to translate from one language to
another.  The default is translating to the common language and
this cost will be used in both directions.  ie: the cost of translating
from djelian to common is the same as translating from common to
djelian.  If translating between two languages that are not the
default language, say the default is common and we want to translate
from djelian to wombat, then the cost from djelian->common and then
common->wombat will be used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to set the translation cost of
.EP
.SP 9 5
cost - the cost is in percentage, 200 means twice the price
.EP

.SI 3
* %^BOLD%^check_for_magic_writing%^RESET%^
.EI
.SI 5
int check_for_magic_writing(object ob)
.EI
.SP 7 5

Checks to see if the object has any magic writing on it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is magic writing, 0 if not.
.EP

.SI 3
* %^BOLD%^check_for_untranslatable_language%^RESET%^
.EI
.SI 5
int check_for_untranslatable_language(object ob)
.EI
.SP 7 5

Checks to see if the book contains a language we cannot translate.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the book to check for languages
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is a language we cannot deal with
.EP

.SI 3
* %^BOLD%^do_print%^RESET%^
.EI
.SI 5
int do_print(object * obs, string language)
.EI
.SP 7 5

This method will be called by the print add_command.  It will print out
the pricing schedule and how long the item will take to make.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects which have been matched
.EP
.SP 9 5
language - the language to print the book in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_print_copies%^RESET%^
.EI
.SI 5
int do_print_copies(object * obs, int number, string language)
.EI
.SP 7 5

This method will be called when the player tries to print a certain
number of copies of a book.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects which have been matched
.EP
.SP 9 5
number - the number of prints to do
.EP
.SP 9 5
language - the language to print the book in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_print_run%^RESET%^
.EI
.SI 5
void do_print_run(object ob, int number, int cost, string language)
.EI
.SP 7 5

This method does the actual print run.  It creates all the objects
and makes the player pay for them all.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to print
.EP
.SP 9 5
number - the number to print
.EP
.SP 9 5
cost - the cost of the object
.EP
.SP 9 5
the - language to translate things to
.EP

.SI 3
* %^BOLD%^find_book_or_paper%^RESET%^
.EI
.SI 5
object find_book_or_paper(object * obs)
.EI
.SP 7 5

This method finds a single book or piece of paper to work on.  It will
return 0 if multiple objects are found, or none are found.  It will
set the error message appropriately.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to check for a book or paper
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the book or paper found
.EP

.SI 3
* %^BOLD%^load_it%^RESET%^
.EI
.SI 5
void load_it()
.EI
.SP 7 5

Loads the saved information off the disc.
.EP

.SI 3
* %^BOLD%^price_object%^RESET%^
.EI
.SI 5
int price_object(object ob, int num, string to_language)
.EI
.SP 7 5

Figure out how much this object will cost to make.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to price
.EP
.SP 9 5
num - the number of them to print
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost of the item

.EP

.SI 3
* %^BOLD%^print_pricing%^RESET%^
.EI
.SI 5
void print_pricing(object ob, string language)
.EI
.SP 7 5

This method will print out the pricing schedule for the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to get a pricing schedule of.

.EP

.SI 3
* %^BOLD%^query_all_translantion_costs%^RESET%^
.EI
.SI 5
mapping query_all_translantion_costs()
.EI
.SP 7 5

This method will return a mapping of all the translation costs.
The keys are the languages and the values are the costs in terms
of percentages.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current translation cost mapping
.EP

.SI 3
* %^BOLD%^query_default_language%^RESET%^
.EI
.SI 5
string query_default_language()
.EI
.SP 7 5

This method will return the current default language for the
print shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current default language
.EP

.SI 3
* %^BOLD%^query_discount_schedule%^RESET%^
.EI
.SI 5
mixed * query_discount_schedule()
.EI
.SP 7 5

This method returns the discount schedule.  There is a default discount
schedule setup for printing shops.  The discount schedule tells how much
of a discount you get for certain numbers of a run.  It returns
an array of the form:

.EP
.SP 7 5
({ ({ numn1, dis1 }), ({ num2, dis2}), ... })
.EP
.SP 7 5
Where each number is the discount amount for printing less than that number of items. The array must be sorted from the lowest number to the highest number.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the discount schedule
.EP

.SI 3
* %^BOLD%^query_dynamic_auto_load_info%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load_info(int num)
.EI
.SP 7 5

This method returns dynamic the autoload information associated with the
file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number associated with the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the dynamic auto load string, 0 if none

.EP

.SI 3
* %^BOLD%^query_letter_cost%^RESET%^
.EI
.SI 5
int query_letter_cost()
.EI
.SP 7 5

This queries the cost per letter.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost per letter
.EP

.SI 3
* %^BOLD%^query_no_collection%^RESET%^
.EI
.SI 5
int query_no_collection()
.EI
.SP 7 5

This returns the no collection flag used by the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the no collection flag
.EP

.SI 3
* %^BOLD%^query_office_room%^RESET%^
.EI
.SI 5
string query_office_room()
.EI
.SP 7 5

Returns the current office room associated with this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current office room
.EP

.SI 3
* %^BOLD%^query_open_func%^RESET%^
.EI
.SI 5
function query_open_func()
.EI
.SP 7 5

This method returns the current function which is used to check for
the openness of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current open function

.EP

.SI 3
* %^BOLD%^query_print_run%^RESET%^
.EI
.SI 5
mixed * query_print_run(string name)
.EI
.SP 7 5

This method returns the array of documents awaiting collection.  If there
are not documents this returns 0.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing the player information, or 0 if none
.EP

.SI 3
* %^BOLD%^query_print_shop_name%^RESET%^
.EI
.SI 5
string query_print_shop_name()
.EI
.SP 7 5

This method will return the current name of the print shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the print shop

.EP

.SI 3
* %^BOLD%^query_save_dir%^RESET%^
.EI
.SI 5
string query_save_dir()
.EI
.SP 7 5

Returns the save file name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
save file name
.EP

.SI 3
* %^BOLD%^query_static_auto_load_info%^RESET%^
.EI
.SI 5
string query_static_auto_load_info(int num)
.EI
.SP 7 5

This method returns static the autoload information associated with the
file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number associated with the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the static auto load string, 0 if none

.EP

.SI 3
* %^BOLD%^remove_print_run%^RESET%^
.EI
.SI 5
void remove_print_run(string name, mixed * info)
.EI
.SP 7 5

This method will remove a players print run.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player whose print run is to be removed.
.EP

.SI 3
* %^BOLD%^remove_translation_cost%^RESET%^
.EI
.SI 5
void remove_translation_cost(string lang)
.EI
.SP 7 5

This method will remove the translation cost for a specific language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to remove the translation cost of
.EP

.SI 3
* %^BOLD%^save_it%^RESET%^
.EI
.SI 5
void save_it()
.EI
.SP 7 5

Saves the information to the disc.
.EP

.SI 3
* %^BOLD%^set_default_language%^RESET%^
.EI
.SI 5
void set_default_language(string lang)
.EI
.SP 7 5

This method sets the default language to use in the print shop.
Anything written in this language will cost nothing extra to
duplicate.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the new default language
.EP

.SI 3
* %^BOLD%^set_discount_schedule%^RESET%^
.EI
.SI 5
void set_discount_schedule(mixed * value)
.EI
.SP 7 5

Sets the discount schedule for this object.  See the query_discount_schedule
help for the format of the array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - the new discount schedule.
.EP

.SI 3
* %^BOLD%^set_letter_cost%^RESET%^
.EI
.SI 5
void set_letter_cost(int value)
.EI
.SP 7 5

This queries the cost per letter.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - the new cost per letter
.EP

.SI 3
* %^BOLD%^set_no_collection%^RESET%^
.EI
.SI 5
void set_no_collection(int collect)
.EI
.SP 7 5

This sets the no collection flag.  It makes it so this room cannot be
used for collections.  If this is set then there should be an associated
collection room used to pick up the books from.  This room should
use the set_office_room() function to set the office room to use
for the collections.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
collect - the new value of no_collections flag
.EP

.SI 3
* %^BOLD%^set_office_room%^RESET%^
.EI
.SI 5
void set_office_room(string room)
.EI
.SP 7 5

Sets the office room to which this collection room is associated.  If
this is set on a room, you cannot setup print jobs in the room and this
room specified here is queried for the information about the jobs to
collect.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the new office room
.EP

.SI 3
* %^BOLD%^set_open_func%^RESET%^
.EI
.SI 5
void set_open_func(function func)
.EI
.SP 7 5

This method sets the function to evaluate to check to see if the shop
is open or not.  This should set any error messages which are needed.
The objects are passed in as an argument in the do_print functions
and nothing is passed in for the collection check.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the open function
.EP

.SI 3
* %^BOLD%^set_print_shop_name%^RESET%^
.EI
.SI 5
void set_print_shop_name(string name)
.EI
.SP 7 5

This method sets the name of the print shop which will be used on
books when they are printed.  This is only needed in the collection
room, this is the name that is placed on the fly leaf.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the print shop

.EP

.SI 3
* %^BOLD%^set_save_dir%^RESET%^
.EI
.SI 5
void set_save_dir(string name)
.EI
.SP 7 5

Sets the save file name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the save file
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_auto_load_info%^RESET%^
.EI
.SI 5
int add_auto_load_info(string static, string dynamic)
.EI
.SP 7 5

This method saves the autoload information out to a file and returns
the number associated with the file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
static_arg - the static auto load information
.EP
.SP 9 5
dynamic_arg - the dynamic auto load information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number associated with the save files

.EP

.SI 3
* %^BOLD%^copyright_holder%^RESET%^
.EI
.SI 5
string copyright_holder(object book)
.EI
.SP 7 5

This method returns the current copyright holder of the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book - the book to get the copyright of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the copyright holder, 0 if none

.EP

.SI 3
* %^BOLD%^do_claim%^RESET%^
.EI
.SI 5
int do_claim(object * obs)
.EI
.SP 7 5

This method sets up the copyright for the objects.  This will check for
existing copyright status and then check with the book handler to see
if it still fits the criteria.  If it does not then it will add itself
as a new book.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to claim copyright on

.EP

.SI 3
* %^BOLD%^do_collect%^RESET%^
.EI
.SI 5
int do_collect()
.EI
.SP 7 5

This is when the player comes back to collect the book they have
printed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^is_copyright%^RESET%^
.EI
.SI 5
int is_copyright(object book)
.EI
.SP 7 5

This method checks to make sure that the player is the correct holder
of the copyright and that the book is still copyright.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
book - the book to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is copyright

.EP


