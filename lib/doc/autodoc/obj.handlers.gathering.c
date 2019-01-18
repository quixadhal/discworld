.DT
gathering.c
Disk World autodoc help
gathering.c

.SH Description
.SP 5 5

Handler to handle the gathering of implicitly existing
objects that require some skill to find, such as herbs.
Recognition (ie, knowing what it is once you've found it) may be
added later.

The data used for gathering comes from two places: the handler's database of gatherable items, and the room's local information.

The handler's database contains a class entry for each gatherable:
.EP
.SP 5 5

.EP
.SP 5 5
skill (string) 
.EP
.SP 8 5
The skill needed to find the item. 
.EP
.SP 5 5
difficulty (int) 
.EP
.SP 8 5
The taskmaster difficulty. 
.EP
.SP 5 5
upper (int) 
.EP
.SP 8 5
The taskmaster "upper" parameter. 
.EP
.SP 5 5
extra (mixed) 
.EP
.SP 8 5
The taskmaster "extra" parameter. 
.EP
.SP 5 5
season (string *) 
.EP
.SP 8 5
The seasons during which the gatherable is available (eg, plants). If this is unset, the default is all seasons. Note that, currently, it is the same season everywhere on the Disc. 
.EP
.SP 5 5
quant (int or function pointer returning an int) 
.EP
.SP 8 5
The quantity that will be gathered. The function pointer is passed three parameters: the room, the player, and the name of the gathered item. This allows, for example, a function to be called on the room to calculate the quantity, such as: 
.EP
.SP 8 5

.EP
.SP 8 5
    (: $1->room_func($2, $3) :)
.EP
.SP 8 5

.EP
.SP 5 5
ob (string) 
.EP
.SP 8 5
The pathname of the object to be created. If the object is continuous, the %^CYAN%^amount%^RESET%^of ob is set to %^CYAN%^quant%^RESET%^; otherwise, %^CYAN%^quant%^RESET%^copies of the object are cloned. 
.EP
.SP 5 5


The room's local information is set when the gatherable is added via add_item(), and consists of:


.EP
.SP 8 5
item name (string or function pointer returning a string) 
.EP
.SP 11 5
The name that will be used to look up the gatherable in the handler's database. The function pointer is passed two parameters when evaluated: the room, and the player. 
.EP
.SP 8 5
scarcity (int or function pointer returning an int) 
.EP
.SP 11 5
The percent chance of finding any quantity of the gatherable. This is an additional constraint on finding the gatherable, in addition to any skill requirements and quantity calculations. A scarcity of 100 (the default) indicates no scarcity constraint. A scarcity of 0 means that the gatherable will never be found. 
.EP
.SP 5 5

.EP
.SP 10 5


Written by Jeremy

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h and /include/weather.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^item_data%^RESET%^
class item_data {
string skill;
int difficulty;
string * season;
mixed quant;
string ob;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_item%^RESET%^
.EI
.SI 5
void add_item(string name, string skill, int diff, string * season, mixed quant, string ob)
.EI
.SP 7 5

This method adds an item into the current list of gatherable items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the item to add
.EP
.SP 9 5
skill - skill needed to find this item
.EP
.SP 9 5
diff - taskmaster difficulty
.EP
.SP 9 5
season - available season(s)
.EP
.SP 9 5
quant - quantity (int or function pointer returning an int)
.EP
.SP 9 5
ob - filename of object to be created
.EP

.SI 3
* %^BOLD%^gather_item%^RESET%^
.EI
.SI 5
object * gather_item(string word, object player)
.EI
.SP 7 5

This method attempts to gather some items in the environment of
the specified player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the item to try and gather
.EP
.SP 9 5
player - the player doing the gathering
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of gathered objects
.EP

.SI 3
* %^BOLD%^query_item%^RESET%^
.EI
.SI 5
mixed query_item(string name)
.EI
.SP 7 5

This method will return an array of the information associated with
the item.  The array consists of:

.EP
.SP 7 5
  ({
.EP
.SP 7 5
     skill,       // skill needed to find this item   (string)
.EP
.SP 7 5
     difficulty,  // taskmaster difficulty            (int)
.EP
.SP 7 5
     season,      // available season(s)              (string *)
.EP
.SP 7 5
     quant,       // quantity (int or (int)(:\<code\>:)) (mixed)
.EP
.SP 7 5
     ob           // filename of object to be created (string)
.EP
.SP 7 5
   })
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the item to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the item array as detailed above.
.EP

.SI 3
* %^BOLD%^query_items%^RESET%^
.EI
.SI 5
mapping query_items()
.EI
.SP 7 5

This method returns all of the current gatherable items.  It returns
this as a mapping of a class, so probably not overly useful except
for debugging.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mapping of a locally defined class
.EP


