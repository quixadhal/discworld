.DT
philosophies.c
Disk World autodoc help
philosophies.c

.SH Description
.SP 5 5

This handler contains all the information on the philosophies used
in philosphical debates.
.EP
.SP 10 5


Written by Deutha

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_known%^RESET%^
.EI
.SI 5
void add_known(string person, string argument)
.EI
.SP 7 5

This method adds an argument to the specified person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to add the argument to
.EP
.SP 9 5
argument - the argument to add to the person
.EP

.SI 3
* %^BOLD%^add_philosophy%^RESET%^
.EI
.SI 5
void add_philosophy(string name, string type, int bonus, string needed, string effect)
.EI
.SP 7 5

This method adds a philosphy into the current list of philosphies.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy
.EP
.SP 9 5
type - the type of the philosophy
.EP
.SP 9 5
bonus - the bonus of the philosophy
.EP
.SP 9 5
needed - the string describing what you need
.EP
.SP 9 5
effect - the effect of the philosphy
.EP

.SI 3
* %^BOLD%^add_philosophy_argument%^RESET%^
.EI
.SI 5
void add_philosophy_argument(string name, string argument, mixed args)
.EI
.SP 7 5

This method adds in a philosophy argument to the specified philosophy.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy to add
.EP
.SP 9 5
argument - the name of the argument to add
.EP
.SP 9 5
args - the parameters for the argument
.EP

.SI 3
* %^BOLD%^query_all_known%^RESET%^
.EI
.SI 5
string * query_all_known(string person)
.EI
.SP 7 5

This method returns all the arguments known to the person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to get the arguments for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of all the known arguments
.EP

.SI 3
* %^BOLD%^query_known%^RESET%^
.EI
.SI 5
int query_known(string person, string argument)
.EI
.SP 7 5

This method checks to see if the person knows the argument.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check the argument for
.EP
.SP 9 5
argument - the argument to check to see if it is known
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is known, 0 if not.
.EP

.SI 3
* %^BOLD%^query_philosophies%^RESET%^
.EI
.SI 5
mapping query_philosophies()
.EI
.SP 7 5

This method returns the current set of philosphies available.
The mapping consists of the value being the name of the philosphy
and the value being an array consisting of 5 elements.  You should
be using this call for anything except debugging purposes.  It
would create a bad dependancy otherwise.

.EP
.SP 7 5
name : ({
.EP
.SP 7 5
      [string]  type (i.e. type.points is used),
.EP
.SP 7 5
      [int]     bonus (i.e. bonus in type.points that is needed),
.EP
.SP 7 5
      [string]  what is needed/increased to start,
.EP
.SP 7 5
      [string]  debate effect file name,
.EP
.SP 7 5
      [mapping] arguments
.EP
.SP 7 5
    })
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping of values
.EP

.SI 3
* %^BOLD%^query_philosophy%^RESET%^
.EI
.SI 5
int query_philosophy(string name)
.EI
.SP 7 5

This method determines if the named philosphy exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosphy to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists, 0 if it does not
.EP

.SI 3
* %^BOLD%^query_philosophy_all_arguments%^RESET%^
.EI
.SI 5
mapping query_philosophy_all_arguments(string name)
.EI
.SP 7 5

This method returns the arguments of the philosophy.  This returns
a mapping containing the argument values.  This should not be
used except for debuggin purposes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy to get the arguments for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the arguments for the philosophy
.EP

.SI 3
* %^BOLD%^query_philosophy_argument%^RESET%^
.EI
.SI 5
mixed query_philosophy_argument(string name, string argument)
.EI
.SP 7 5

This method returns the values for the specific argument in
philosophy.   The returned array consists of three values, they
are ({ skill, something, something }).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosphy to get the arguments for
.EP
.SP 9 5
argument - the name of the argument to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of the arguments values
.EP

.SI 3
* %^BOLD%^query_philosophy_bonus%^RESET%^
.EI
.SI 5
int query_philosophy_bonus(string name)
.EI
.SP 7 5

This method returns the bonus of the philosophy.  An example of the
return value from this is 50.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy to get the bonus of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bonus of the philosophy
.EP

.SI 3
* %^BOLD%^query_philosophy_effect%^RESET%^
.EI
.SI 5
string query_philosophy_effect(string name)
.EI
.SP 7 5

This method returns the effect of the philosophy.  An example of the
return value from this is "/std/effects/religious/theological_debate".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy to get the effect of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the effect of the philosophy
.EP

.SI 3
* %^BOLD%^query_philosophy_names%^RESET%^
.EI
.SI 5
string * query_philosophy_names()
.EI
.SP 7 5

This method returns the names of all the current philosophies.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of strings of the names of philosophies
.EP

.SI 3
* %^BOLD%^query_philosophy_needed%^RESET%^
.EI
.SI 5
string query_philosophy_needed(string name)
.EI
.SP 7 5

This method returns the needed string of the philosophy.  An example of the
return value from this is "the power of your faith".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy to get the needes string for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the needed string for the philosophy
.EP

.SI 3
* %^BOLD%^query_philosophy_type%^RESET%^
.EI
.SI 5
string query_philosophy_type(string name)
.EI
.SP 7 5

This method returns the type of the philosophy.  An example of the
return value from this is "faith".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosophy to get the type of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the type of the philosophy
.EP

.SI 3
* %^BOLD%^remove_known%^RESET%^
.EI
.SI 5
int remove_known(string person, string argument)
.EI
.SP 7 5

This method removes an argument from the specified person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove the argument from
.EP
.SP 9 5
argument - the argument to remove from the person
.EP

.SI 3
* %^BOLD%^remove_philosophy%^RESET%^
.EI
.SI 5
void remove_philosophy(string name)
.EI
.SP 7 5

This method removes the specified philosophy from the current list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosphy to remove
.EP

.SI 3
* %^BOLD%^remove_philosophy_argument%^RESET%^
.EI
.SI 5
void remove_philosophy_argument(string name, string argument)
.EI
.SP 7 5

This method removes a specified argument from the philosophy.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the philosphy to remove the argument form
.EP
.SP 9 5
argument - the argument name to remove
.EP

.SI 3
* %^BOLD%^reset_known%^RESET%^
.EI
.SI 5
int reset_known(string person)
.EI
.SP 7 5

This method resets the argument list for the specified person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove the argument from
.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

This method saves the current state of the object.

.EP


