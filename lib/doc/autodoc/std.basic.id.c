.DT
id.c
Disk World autodoc help
id.c

.SH Description
.SP 5 5

This class keeps track of all the aliases, names and adjectives
which are used to match the object.
.EP
.SP 10 5

.EP



.SH See also
.SP 5 5
/global/player->add_command() and /std/basic/desc.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/id_match.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/parse_command.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^Id%^RESET%^
class Id {
string name;
string * alias;
string * faux_alias;
string * unique_faux_alias;
string * adjectives;
string * faux_adjectives;
string * unique_faux_adjectives;
string * plural_adjectives;
string * plurals;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_adjective%^RESET%^
.EI
.SI 5
void add_adjective(mixed str)
.EI
.SP 7 5

This method adds an adjective to the object.   The adjectives are used
for the object matching routines.    For example, if you have a "green
bath tun" then "green" and "bath" are adjectives.
.EP

.SI 3
* %^BOLD%^add_alias%^RESET%^
.EI
.SI 5
void add_alias(mixed str)
.EI
.SP 7 5

This method adds an alias to the object.   An alias for the object is the
part used as the main noun for the object. ie in the case of a "green
bath tub", "bathtub" might be an alias for the object. and "green"
and "bath" would be adjectives. The value set with add_alias is also
used with present. This means you can make up an alias for an object
that players cannot normaly access but you can
get a handle on with present. For an exmple of this look at the money
object.


The function takes an array or a string as an argument. If an array is passed in each element of the array is added to the alias array, if it is a string then the string elemnt is added to the array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the alias(s) to add
.EP

.SI 3
* %^BOLD%^add_faux_adjective%^RESET%^
.EI
.SI 5
void add_faux_adjective(mixed str)
.EI
.SP 7 5

This method adds a faux adjective to the object.   Faux adjectives are
ignored by objects that ignore identifiers.


The function takes an array or a string as an argument. If an array is passed in each element of the array is added to the faux adjective array, if it is a string then each word in the string element is added to the array. Elements that are already real adjectives of the object are ignored.
.EP

.SI 3
* %^BOLD%^add_faux_alias%^RESET%^
.EI
.SI 5
void add_faux_alias(mixed str)
.EI
.SP 7 5

This method adds a faux alias to the object.   Faux aliases are
ignored by objects that ignore identifiers.


The function takes an array or a string as an argument. If an array is passed in each element of the array is added to the faux alias array, if it is a string then the string element is added to the array. Elements that are already real aliases of the object are ignored. Duplication in this list is allowed; a separate list without duplicates is maintained to improve performance.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the alias(s) to add
.EP

.SI 3
* %^BOLD%^add_plural%^RESET%^
.EI
.SI 5
void add_plural(mixed str)
.EI
.SP 7 5

This method adds a plural onto the object.   The plurals will be
used in plural name matching by thje find_match simul_efun
.EP

.SI 3
* %^BOLD%^add_plural_adjective%^RESET%^
.EI
.SI 5
void add_plural_adjective(mixed str)
.EI
.SP 7 5

This method adds an plural adjective to the object.   The plural
adjectives are used for the
object matching routines.  Plural adjectives should be used very rarely,
they are used in cases where the plural of a name is determined by the
adjective rather than it's alias.  For example: pair of shoes, pairs
of shoes.
.EP

.SI 3
* %^BOLD%^add_plurals%^RESET%^
.EI
.SI 5
void add_plurals(string * str)
.EI
.SP 7 5

This method adds multiple plurals to the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the array of plurals to add
.EP

.SI 3
* %^BOLD%^faux_id_allowed%^RESET%^
.EI
.SI 5
int faux_id_allowed()
.EI
.SP 7 5

This method is used by the id function.   It determines whether or
not an object in the calling list ignores identifiers.
If none do, then faux aliases may be used.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if faux ids may be used
.EP

.SI 3
* %^BOLD%^id%^RESET%^
.EI
.SI 5
int id(string str)
.EI
.SP 7 5

This method is used by the present efun.   It determines if the
passed in string is an alias or the name of the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the value to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the name matches

.EP

.SI 3
* %^BOLD%^id_adjective%^RESET%^
.EI
.SI 5
int id_adjective(string word)
.EI
.SP 7 5

This method is similar to the id function, except this will check the
adjectives instead of the aliases and the name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the adjective to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the name matches one of the adjectives, 0 otherwise
.EP

.SI 3
* %^BOLD%^id_plural%^RESET%^
.EI
.SI 5
int id_plural(string str)
.EI
.SP 7 5

This method is similar to the id function, except this will check the
plural names instead of the aliases and the name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the name matches one of the plurals, 0 otherwise
.EP

.SI 3
* %^BOLD%^id_plural_adjective%^RESET%^
.EI
.SI 5
int id_plural_adjective(string word)
.EI
.SP 7 5

This method is similar to the id function, except this will check the
plural adjectives instead of the aliases and the name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the name matches one of the plurals, 0 otherwise
.EP

.SI 3
* %^BOLD%^query_adjectives%^RESET%^
.EI
.SI 5
varargs string * query_adjectives(int no_faux)
.EI
.SP 7 5

This method returns the current list of adjectives associated with
this object.  If faux ids are allowed, then faux adjectives are
included if the optional parameter is not 1.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no_faux - 1 to ignore faux adjectives
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the complete list of adjectives

.EP

.SI 3
* %^BOLD%^query_alias%^RESET%^
.EI
.SI 5
varargs string * query_alias(int no_faux)
.EI
.SP 7 5

This method returns the current list of aliases.  If faux aliases are
allowed, they are included, unless the optional parameter is passed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no_faux - 1 to ignore faux aliases (optional)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of aliases (array of strings)

.EP

.SI 3
* %^BOLD%^query_cap_name%^RESET%^
.EI
.SI 5
string query_cap_name()
.EI
.SP 7 5

This method returns the capitalized name.   It calls the function
capitalize() on the name
.EP

.SI 3
* %^BOLD%^query_faux_adjectives%^RESET%^
.EI
.SI 5
string * query_faux_adjectives()
.EI
.SP 7 5

This method returns the current list of faux adjectives, including
duplicates.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of faux adjectives (array of strings)

.EP

.SI 3
* %^BOLD%^query_faux_alias%^RESET%^
.EI
.SI 5
string * query_faux_alias()
.EI
.SP 7 5

This method returns the current list of faux aliases, with duplicates
included.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of faux aliases (array of strings)

.EP

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string query_name()
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
* %^BOLD%^query_plural_adjectives%^RESET%^
.EI
.SI 5
string * query_plural_adjectives()
.EI
.SP 7 5

This method returns the current list of plural adjectives associated with
this object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the complete list of plural adjectives

.EP

.SI 3
* %^BOLD%^query_plurals%^RESET%^
.EI
.SI 5
string * query_plurals()
.EI
.SP 7 5

This method returns the complete list of plurals for the
object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the complete array of plurals

.EP

.SI 3
* %^BOLD%^query_unique_faux_adjectives%^RESET%^
.EI
.SI 5
string * query_unique_faux_adjectives()
.EI
.SP 7 5

This method returns a unique list of faux adjectives.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of faux adjectives (array of strings)

.EP

.SI 3
* %^BOLD%^query_unique_faux_alias%^RESET%^
.EI
.SI 5
string * query_unique_faux_alias()
.EI
.SP 7 5

This method returns the unique list of faux aliases.  It is this list
that id() uses.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of faux aliases (array of strings) without duplication

.EP

.SI 3
* %^BOLD%^remove_adjective%^RESET%^
.EI
.SI 5
void remove_adjective(mixed str)
.EI
.SP 7 5

This method will remove an adjective from the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the adective(s) to remove

.EP

.SI 3
* %^BOLD%^remove_alias%^RESET%^
.EI
.SI 5
int remove_alias(string str)
.EI
.SP 7 5

This method removes an alias from the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the alias to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the alias is found, 0 if not

.EP

.SI 3
* %^BOLD%^remove_faux_adjective%^RESET%^
.EI
.SI 5
void remove_faux_adjective(mixed str)
.EI
.SP 7 5

This method will remove a faux adjective from the object.  Note that,
if the adjective has been added more than once, only one instance is
removed from the array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the adective(s) to remove

.EP

.SI 3
* %^BOLD%^remove_faux_alias%^RESET%^
.EI
.SI 5
int remove_faux_alias(string str)
.EI
.SP 7 5

This method removes a faux alias from the object.  Note that, if the
alias has been added more than once, only one instance is removed
from the array.  If the last instance of an alias is removed from the
list, it is also removed from the unique list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the faux alias to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the alias is found, 0 if not

.EP

.SI 3
* %^BOLD%^remove_plural%^RESET%^
.EI
.SI 5
void remove_plural(string str)
.EI
.SP 7 5

This method removes a plural from the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the plural to remove
.EP

.SI 3
* %^BOLD%^remove_plural_adjective%^RESET%^
.EI
.SI 5
void remove_plural_adjective(mixed str)
.EI
.SP 7 5

This method will remove an plural adjective from the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the adective(s) to remove

.EP

.SI 3
* %^BOLD%^set_adjectives%^RESET%^
.EI
.SI 5
void set_adjectives(string * str)
.EI
.SP 7 5

This method sets all the adjectives for the object.   This method
should not be used, please use add_adjective instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the array of adjectives

.EP

.SI 3
* %^BOLD%^set_aliases%^RESET%^
.EI
.SI 5
void set_aliases(string * str)
.EI
.SP 7 5

This method sets the entire alias array.   It overrides any current alias
definitions and sets the alias arry.   This should be used carefully
if at all.   Use add_alias instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new alias array

.EP

.SI 3
* %^BOLD%^set_name%^RESET%^
.EI
.SI 5
void set_name(string str)
.EI
.SP 7 5

This method sets the name of the object.   The name is the basic handle
used to reference the object.   It should not have any spaces
in it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name
.EP

.SI 3
* %^BOLD%^set_plural_adjectives%^RESET%^
.EI
.SI 5
void set_plural_adjectives(string * str)
.EI
.SP 7 5

This method sets all the plural adjectives for the object.   This method
should not be used, please use add_plural_adjective instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the array of plural adjectives

.EP

.SI 3
* %^BOLD%^set_plurals%^RESET%^
.EI
.SI 5
void set_plurals(string * str)
.EI
.SP 7 5

This method sets the compete array of plurals.   This shod
not be used, use add_plural and remove_plural instead.
.EP


