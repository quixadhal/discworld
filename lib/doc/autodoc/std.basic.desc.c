.DT
desc.c
Disk World autodoc help
desc.c

.SH Description
.SP 5 5

The main description handling class.  Keeps track of the longs short
etc.
.EP
.SP 10 5


Written by Pinkfish
.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/hide_invis.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^a_short%^RESET%^
.EI
.SI 5
varargs string a_short(int flag)
.EI
.SP 7 5

This method returns the "indeterminate" short.  This function returns a
string that the message system replaces, when messages are printed, by the
determinate of the object and its short as given by pretty_short. The
string should not be stored, since the object to which it refers may
not later exist; it can be processed with convert_message.


The return of this function needs more processing before the player sees it. In general you will not need to worry about this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - If this flag is set, do not bring the player out of hiding.
.EP

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string str, int dark)
.EI
.SP 7 5

This method returns the current long description.  This is
often overriden to display other information.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string which matched us
.EP
.SP 9 5
dark - is it dark when viewing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current long descriptions
.EP

.SI 3
* %^BOLD%^one_short%^RESET%^
.EI
.SI 5
varargs string one_short(int flag)
.EI
.SP 7 5

This method returns an "out-of-a-group" short.
This function returns a string that the message system replaces, when
messages are printed, by a string dependent on the determinate of
the object and its short as given by pretty_short. If the viewer
is not in the same environment as the object or if the determinate is
defined and is not "a " or "an ", that string is the determinate. If
there is more than one object with the same plural as that object in its
environment, then that string is "one of the "; otherwise the
string is "the ". The output from from one_short should not be
stored, since the object to which it refers may not later exist; it can be
processed with convert_message. One_short is mostly used for
referring to a living object when it performs an action independent
of any previous actions. 


The return of this function needs more processing before the player sees it. In general you will not need to worry about this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - If this flag is set, do not bring the player out of hiding.
.EP

.SI 3
* %^BOLD%^poss_short%^RESET%^
.EI
.SI 5
varargs string poss_short(int flag)
.EI
.SP 7 5

This method returns the "possessed" short.  This function returns a
string that the message system replaces, when messages are printed, by a
string dependent on the possessor of the object and its short as
given by pretty_short. If the object has no environment or is not in a
living object or corpse, that string is the determinate. If the
environment of the object is the viewer of the message, that string
is "your". If the determinate is defined and is not "a " or "an ",
or, if it is not defined or is "a " or "an " but it is the only thing in its
environment with its plural, that string is the possessive of the owner, if
the owner has already been mentioned in constructing a sentence, or the
owner's the_short plus "'s "; otherwise, the string is "one
of the " plus the possessive bit as described in the previous part of
this sentence. Got all that? Good :) The string should not be stored,
since the object to which it refers may not later exist; it can be
processed with convert_message. Poss_short is mostly used for referring
to an object when a person has just used it to do something.


The return of this function needs more processing before the player sees it. In general you will not need to worry about this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - If this flag is set, do not bring the player out of hiding.
.EP

.SI 3
* %^BOLD%^pretty_plural%^RESET%^
.EI
.SI 5
varargs string pretty_plural(object thing)
.EI
.SP 7 5

The observer dependant plural.  It was specifical written to
handle money that needed to be displayed over several lines but
only be one object. The way this was achived was if pretty_plural
returns an array then each of the elements of the array is considered
to be a seperate object and displayed accordingly. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the observer
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the plural string

.EP

.SI 3
* %^BOLD%^pretty_short%^RESET%^
.EI
.SI 5
varargs string pretty_short(object thing)
.EI
.SP 7 5

This method does weird stuff.  Pretty much it just calls the
short() function with the dark parameter...  This should not be
used to generate output.  Use a_short(), the_short(), or
one_short or poss_short() instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the viewer
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string being the short
.EP

.SI 3
* %^BOLD%^query_determinate%^RESET%^
.EI
.SI 5
string query_determinate(object thing)
.EI
.SP 7 5

This returns the determinate for the object.  The determinate is
something like 'the'.  You set the derterminate by setting
the "determinate" property.  The viewer is check to see if
they can see the object as well, if it cannot be seen
"' is returned.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to check for visibility

.EP

.SI 3
* %^BOLD%^query_long%^RESET%^
.EI
.SI 5
varargs mixed query_long(string str, int dark)
.EI
.SP 7 5

This method returns the unadulterated long description.  The other
long calls might modify the output to display some status information.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the long description
.EP

.SI 3
* %^BOLD%^query_long_details%^RESET%^
.EI
.SI 5
string query_long_details(string arg, int dark, object looker)
.EI
.SP 7 5

This method returns extra details about the object that should be
known above the basic long.  This is printed to people who do not
own the object in question (long() is used if you own it).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arg - the argument the look was called with
.EP
.SP 9 5
dark - how dark the place is
.EP
.SP 9 5
looker - the person looking at the object

.EP

.SI 3
* %^BOLD%^query_main_plural%^RESET%^
.EI
.SI 5
mixed query_main_plural()
.EI
.SP 7 5

This method returns the current value of the main plural.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the main plural

.EP

.SI 3
* %^BOLD%^query_plural%^RESET%^
.EI
.SI 5
varargs string query_plural(int dark)
.EI
.SP 7 5

This method returns the current plural string.  This is often overridden
to display extra information about the object, like the short and
long calls.  There is always an exception to a naming scheme :)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dark - if it was dark when viewing

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the plural string
.EP

.SI 3
* %^BOLD%^query_plural_desc%^RESET%^
.EI
.SI 5
string query_plural_desc()
.EI
.SP 7 5

This method returns the pluralized version of query_short_desc(). 
.EP

.SI 3
* %^BOLD%^query_short%^RESET%^
.EI
.SI 5
mixed query_short()
.EI
.SP 7 5

This method returns the unadulterated short description.  The other
short calls might modify the output to display some status information.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short description
.EP

.SI 3
* %^BOLD%^query_short_desc%^RESET%^
.EI
.SI 5
string query_short_desc()
.EI
.SP 7 5

This method creates a simple short description for players based on their
stats, gender and guild.

.EP

.SI 3
* %^BOLD%^set_long%^RESET%^
.EI
.SI 5
void set_long(mixed str)
.EI
.SP 7 5

This method sets the long description for the object.  The long description
is what the player sees with they 'look' at the object.  Please make sure
tht you use sentances in your long description.  Something like
'The red box' is not a sentance.  You can use colour codes in your
long description to make it look cool.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the long description
.EP

.SI 3
* %^BOLD%^set_main_plural%^RESET%^
.EI
.SI 5
void set_main_plural(mixed str)
.EI
.SP 7 5

This method sets the main plural for the object.  The main plural
is like the short description for the pluralised object.  If this
is not set then the short will attempt to be pluralised to
figure it out.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the main plural
.EP

.SI 3
* %^BOLD%^set_short%^RESET%^
.EI
.SI 5
void set_short(mixed words)
.EI
.SP 7 5

This method sets the short description for the object.  Be careful
with using function pointers here, as they do not save when
players log out.


The short description is the description seen in peoples inventorys and in brief mode when you enter rooms. This should be only a few words long and should contain a fairly good description of the object. Colour processing is done on basicly all of discworlds output, so that will work in the short.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the short description
.EP

.SI 3
* %^BOLD%^short%^RESET%^
.EI
.SI 5
varargs string short(int dark)
.EI
.SP 7 5

This method returns the current short description.  This is often
overridden to return status information about the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dark - if it is dark when viewing

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short description string
.EP

.SI 3
* %^BOLD%^the_poss_short%^RESET%^
.EI
.SI 5
varargs string the_poss_short(int flag)
.EI
.SP 7 5

Just like poss_short() but returning a definite article.
.EP

.SI 3
* %^BOLD%^the_short%^RESET%^
.EI
.SI 5
varargs string the_short(int flag)
.EI
.SP 7 5

This method returns the "determinate" short.  This function returns a
string that the message system replaces, when messages are printed, by the
determinate of the object (or "the" if the determinate is "a", "an" or
undefined) and its short as given by pretty_short. The string
should not be stored, since the object to which it refers may not later
exist; it can be processed with convert_message.


The return of this function needs more processing before the player sees it. In general you will not need to worry about this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - If this flag is set, do not bring the player out of hiding.
.EP


