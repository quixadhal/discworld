.DT
cloner.c
Disk World autodoc help
cloner.c

.SH Description
.SP 5 5

Used for mapping deleted files to new ones when things are changed.
Updated to a more tidy mechanism by Ceres 14/3/96


Usage: call add_mapping(from, to) /global/cloner 
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_illegal%^RESET%^
.EI
.SI 5
int add_illegal(string basename, string short, string replacement)
.EI
.SP 7 5

This method is used to add objects that we don't want to be reloaded.
This is for those objects that are clones of /std/object or /obj/armour
or something but which are wrongly configured.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
basename - the base filename of the object
.EP
.SP 9 5
short - the short name of the object
.EP
.SP 9 5
replacement - the replacement object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^add_mapping%^RESET%^
.EI
.SI 5
int add_mapping(string from, string to)
.EI
.SP 7 5

Use this function to add a new mapping of one filename to another.
Means that if an object of type 'from' is attempted to be cloned
an object of type 'to' will be cloned instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the old object name
.EP
.SP 9 5
to - the new object name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^clone%^RESET%^
.EI
.SI 5
object clone(string word)
.EI
.SP 7 5

Try and clone the passed in object.  If the name exists in the changes
array then the new file name is cloned instead of the old one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the file to clone
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cloned object
.EP

.SI 3
* %^BOLD%^illegal_thing%^RESET%^
.EI
.SI 5
string illegal_thing(string basename, string short)
.EI
.SP 7 5

Determine if a given object is illegal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
basename - the basename of the object.
.EP
.SP 9 5
short - the shortname of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^list_mappings%^RESET%^
.EI
.SI 5
string list_mappings(string str)
.EI
.SP 7 5

This method returns a list of all the current mappings.  If a
pattern is given, then only files which contain that string will
be returned.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the pattern to search for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of mappings
.EP

.SI 3
* %^BOLD%^load_file%^RESET%^
.EI
.SI 5
void load_file()
.EI
.SP 7 5

Loads the current configuration.

.EP

.SI 3
* %^BOLD%^other_file%^RESET%^
.EI
.SI 5
string other_file(string word)
.EI
.SP 7 5

This method returns the mapped name of the object.  If no mapping
exists the passed in value is returned directly.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the object name to get a mapping for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file to use
.EP

.SI 3
* %^BOLD%^query_changes%^RESET%^
.EI
.SI 5
mapping query_changes()
.EI
.SP 7 5

This method returns the list of currently moved objects.

.EP

.SI 3
* %^BOLD%^query_illegal%^RESET%^
.EI
.SI 5
mapping query_illegal()
.EI
.SP 7 5

List the illegal objects.

.EP

.SI 3
* %^BOLD%^remove_illegal%^RESET%^
.EI
.SI 5
int remove_illegal(string basename, string short)
.EI
.SP 7 5

Remove an item from the illegal array
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
basename - the basename of the object.
.EP
.SP 9 5
short - the shortname of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^remove_mapping%^RESET%^
.EI
.SI 5
int remove_mapping(string from)
.EI
.SP 7 5

Removes a mapping from the object.  This looks to see if an mapping
exists for the object and removes it if it does.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the object to remove the mapping from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success and 0 on failure
.EP

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI
.SP 7 5

Saves the current configuration.

.EP


