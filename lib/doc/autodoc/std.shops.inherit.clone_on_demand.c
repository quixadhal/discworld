.DT
clone_on_demand.c
Disk World autodoc help
clone_on_demand.c

.SH Description
.SP 5 5

This is a inheritable that provides an efficient way of storing
lots of items. Only one copy of the item is effectively in storage
and everytime one is removed, an exact copy is duplicated and
put back in the store. Objects are added to the store via
add_object(). The actual storage container object
can be returned by query_cont() and this container should be
searched when you want to find what objects the store contains.


Any object can inherit this, and methods should be put in place in the inheriting file that end up calling create_real_object() which will sort out duplicating the item and returning an object pointer to the one you can deal with.
.EP
.SP 10 5


Written by Pinkfish Aquilo
.EP



.SH See also
.SP 5 5
add_object

.EP
.SH Example
.SI 5
inherit "clone_on_demand";
int do_buy( objects *obs );

void setup(){
  set_name("shop");
  set_short("widget shop");
  add_object( "sprocket" );
}

object create_object( string arg ){
  if( arg == "sprocket" )
    return clone_object( "/path/of/sprocket" );
}

void init(){
  add_command("buy", "<indirect:object:" +
                     base_name( query_cont() ) + ">");
}

int do_buy( object *obs ){
  object ob;
  foreach(ob in obs){
    widget = create_real_object(ob);
    widget->move( this_player() );
  }
  add_succeeded_mess( "$N buy$s $I.\n", obs );
  return 1;
}

.EI
.SH Includes
.SP 5 5
This class includes the following files /include/armoury.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_object%^RESET%^
.EI
.SI 5
varargs int add_object(string name, int max_per_reset, string display_as)
.EI
.SP 7 5

This method is used to add an item to the storage.
When this method is called, create_object() is called
(with the object name as an arg) in the inheriting file.
If no object is returned by that function,
the name is cloned with clone_object(), and failing that
request_item() is called in the armoury against the name.

This method makes add_weapon() and add_armour() obsolete.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the object to add.
.EP
.SP 9 5
max_per_reset - the maximum number of items to be available at any one time
.EP
.SP 9 5
display_as - the name that this item is displayed as in shops
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the item was added successfully to the store, 0 if it was not.

.EP

.SI 3
* %^BOLD%^query_cont%^RESET%^
.EI
.SI 5
object query_cont()
.EI
.SP 7 5

This method returns the container which is used to keep one copy of
each items in storage.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object container

.EP

.SI 3
* %^BOLD%^query_items_left%^RESET%^
.EI
.SI 5
int * query_items_left(string * names)
.EI
.SP 7 5

This function can be used to check the quantity left of an array
of items.  It returns a parallel array of integers.  In other words
the array it returns contains the numbers of stock in array positions
corresponding to the array positions of the objects it was passed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
names - an array of the short names of the items you wish to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of integers, each one returning like query_number_left
would for the object in that position of the object array.
.EP

.SI 3
* %^BOLD%^query_num_items_left%^RESET%^
.EI
.SI 5
int query_num_items_left(object ob)
.EI
.SP 7 5

Returns how more times object ob can be duplicated
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
how many more times

.EP

.SI 3
* %^BOLD%^query_number_left%^RESET%^
.EI
.SI 5
int query_number_left(string name)
.EI
.SP 7 5

This function returns the quantity of particular object available
to be cloned on demand. In matching which object is the one in
question it uses the short name of the object, which is passed as
an argument to the function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - is the short name of the object you wish to query.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number left, returns INFINITE_LEFT if the shop has an infinite
number, returns -1 if the item isn't stocked.

.EP

.SI 3
* %^BOLD%^query_object_domain%^RESET%^
.EI
.SI 5
string query_object_domain()
.EI
.SP 7 5

This method returns the domain the objects will be created from.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the domain the objects are created from

.EP

.SI 3
* %^BOLD%^set_object_domain%^RESET%^
.EI
.SI 5
void set_object_domain(string domain)
.EI
.SP 7 5

This method sets the domain the objects will be created from.
The default objects will always be matched as well.  So setting
this will allow objects from the default of the specified armoury
domain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
domain - the domain to try and create from
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^create_real_object%^RESET%^
.EI
.SI 5
object create_real_object(object thing)
.EI
.SP 7 5

The main point of entry. 'thing' should be an object already placed
in the clone_on_demand store container via 'add_object'. This
method then duplicates that object, replaces the original copy in
the container with this new one, and returns the original which can
be delt with as normal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - an object in the store
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the original object
.EP


