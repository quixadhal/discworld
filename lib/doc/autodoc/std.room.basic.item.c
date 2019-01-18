.DT
item.c
Disk World autodoc help
item.c

.SH Includes
.SP 5 5
This class includes the following files /include/obj_parser.h and /include/parse_command.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^modify_item_by_index%^RESET%^
.EI
.SI 5
int modify_item_by_index(int index, mixed long)
.EI
.SP 7 5

This method non-destructively modifies the items values.  It will not
remove the value for this item, remember this!  If you add again
and again you will end up with multiple of the same object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name to reference the object by
.EP
.SP 9 5
long - the long bits to change on the item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^modify_item_by_name%^RESET%^
.EI
.SI 5
int modify_item_by_name(string str, mixed long)
.EI
.SP 7 5

This method non-destructively modifies the items values.  It will not
remove the value for this item, remember this!  If you add again
and again you will end up with multiple of the same object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name to reference the object by
.EP
.SP 9 5
long - the long bits to change on the item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^print_stuff%^RESET%^
.EI
.SI 5
void print_stuff()
.EI

.SI 3
* %^BOLD%^query_count%^RESET%^
.EI
.SI 5
int query_count()
.EI

.SI 3
* %^BOLD%^query_creator_room%^RESET%^
.EI
.SI 5
string query_creator_room()
.EI

.SI 3
* %^BOLD%^query_cur_desc%^RESET%^
.EI
.SI 5
int * query_cur_desc()
.EI
.SP 7 5

This returns the currently matching indexes for the item object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of currently matchig indexes

.EP

.SI 3
* %^BOLD%^query_gather%^RESET%^
.EI
.SI 5
mixed * query_gather()
.EI
.SP 7 5

This method figures out what gatherables are available for the
current description set.  The array contains the values passed
into the add_item stuff with the "gather" index.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current gatherables

.EP

.SI 3
* %^BOLD%^query_lng%^RESET%^
.EI
.SI 5
string * query_lng()
.EI
.SP 7 5

This method returns all the long descriptions for the item object.  The
positions in the array correspond to the index used to reference the
items.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of long descriptions

.EP

.SI 3
* %^BOLD%^query_plurals%^RESET%^
.EI
.SI 5
mapping query_plurals()
.EI
.SP 7 5

This method returns the plurals mapping used in the item object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the plurals in the item object

.EP

.SI 3
* %^BOLD%^query_short_array%^RESET%^
.EI
.SI 5
string * query_short_array()
.EI
.SP 7 5

This method returns the base array of shorts to be processed in the
other short methods.  This is the short without the leading
'the', 'a' whatever...
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of shorts

.EP

.SI 3
* %^BOLD%^query_shrt%^RESET%^
.EI
.SI 5
string * query_shrt()
.EI
.SP 7 5

This method returns all the short descriptions for the item object.  The
positions in the array correspond to the index used to reference the
items.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of long descriptions

.EP

.SI 3
* %^BOLD%^query_verbs%^RESET%^
.EI
.SI 5
mapping query_verbs()
.EI
.SP 7 5

This method returns the verbs mapping used in the item object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the verbs in the item object

.EP

.SI 3
* %^BOLD%^remove_item_by_index%^RESET%^
.EI
.SI 5
int remove_item_by_index(int index)
.EI
.SP 7 5

This method removes an item by the returned index number from the
add_item method.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
index - the index number to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^remove_item_by_name%^RESET%^
.EI
.SI 5
int remove_item_by_name(string str)
.EI
.SP 7 5

This method removes an item by the short description.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
index - the index number to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^setup_item%^RESET%^
.EI
.SI 5
int setup_item(mixed nam, mixed long, int no_plural)
.EI
.SP 7 5

This method setups the item initially.   This is called by the
add_item method in the room itself.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
nam - the name(s) to add the item for
.EP
.SP 9 5
long - the long description of the item
.EP
.SP 9 5
no_plural - do not add a plural flag
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the index associated with the item

.EP


