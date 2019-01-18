.DT
cont_medium.c
Disk World autodoc help
cont_medium.c

.SH Description
.SP 5 5

Continuous Medium Object.

amount: current quantity of medium in arbitrary units

continuous: 1 for continuous (default), reset to 0 for normal
            object behaviour

collective: 1 for collective, 0 for normal behaviour.

weight_unit: ({ a, b }) means that b units have weight a

value_scale (float): if set & continuous, value is ::query_value() * amount
                     * value_scale; else value is ::query_value()

medium_alias: used for identification - choose something with
              capitals (e.g. "ChalkDust") so players can't
              use it as a valid alias by accident

pile_name: "pile", "collection", "puddle", etc.

amount_types: ([ "pinch": ({ 1, "pinches" }),
                 "handful": ({ 50, "handfuls" }) ])

pile_types: ({ 50, "small", 100, "medium", 150, "large", "huge" });

Added by Jeremy, 4-Nov-95:

ground, longs, shorts, medium_aliases

The above were removed by Jeremy, 18-Jun-97.  Hopefully no one will
miss them.
.EP
.SP 10 5


Written by Jeremy

Additions for collective objects:
Since "a" and "an" will be replaced with the number of objects, 
"$a" and "$an" can be used if there should be a non-changing "a" or "an"
in the long.
For verbs you can use $V<singular-verb>$<plural-verb>. Example: $Vglows$glow
If there are any words which should be pluralized, place a $P in front of it,
like $Pleaf or $Pword
Finally you can use $A<word> if you want "a word" in for one object and
"words" for more than one.

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/obj_parser.h, /include/parse_command.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_amount%^RESET%^
.EI
.SI 5
void adjust_amount(int number)
.EI
.SP 7 5

This method changes the amount of stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the amount to change it by

.EP

.SI 3
* %^BOLD%^amount_size%^RESET%^
.EI
.SI 5
varargs string amount_size(int exact)
.EI
.SP 7 5

This method returns a string which tells us how much stuff we
have.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string representing the amount of stuff we have

.EP

.SI 3
* %^BOLD%^amount_size_short%^RESET%^
.EI
.SI 5
string amount_size_short()
.EI
.SP 7 5

This method returns a string which tells us how much stuff we
have.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string representing the amount of stuff we have

.EP

.SI 3
* %^BOLD%^dest_old_env%^RESET%^
.EI
.SI 5
void dest_old_env(object place)
.EI
.SP 7 5

This method destructs the temporary container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
place - the object to destroy

.EP

.SI 3
* %^BOLD%^make_medium_clone%^RESET%^
.EI
.SI 5
object make_medium_clone(int number)
.EI
.SP 7 5

This method makes a duplicate of ourselves at a different size.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new size of the object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new object

.EP

.SI 3
* %^BOLD%^merge%^RESET%^
.EI
.SI 5
void merge()
.EI

.SI 3
* %^BOLD%^merge_criteria%^RESET%^
.EI
.SI 5
int merge_criteria(object ob)
.EI

.SI 3
* %^BOLD%^new_collective%^RESET%^
.EI
.SI 5
object new_collective(int number)
.EI

.SI 3
* %^BOLD%^no_env_move%^RESET%^
.EI
.SI 5
void no_env_move(object dest, object place)
.EI
.SP 7 5

This method checks to see if we are still in our silly little container
and if we are, moves us back.  We always dest the old container
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - where we are going to
.EP
.SP 9 5
place - the temporary container

.EP

.SI 3
* %^BOLD%^pile_size%^RESET%^
.EI
.SI 5
string pile_size()
.EI
.SP 7 5

This method returns a string describing how big the pile is.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the size of this pile of stuff

.EP

.SI 3
* %^BOLD%^query_amount%^RESET%^
.EI
.SI 5
int query_amount()
.EI
.SP 7 5

This method returns the amount of the stuff there is.  This is a
volume measurement.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of stuff

.EP

.SI 3
* %^BOLD%^query_amount_types%^RESET%^
.EI
.SI 5
mapping query_amount_types()
.EI
.SP 7 5

This method returns the types of the different amounts.
argument is a mapping which contains the name of the pile and
value is an array, the first element is the amount needed for
the name and the second element is the plural of the pile name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount types mapping

.EP

.SI 3
* %^BOLD%^query_collective%^RESET%^
.EI
.SI 5
int query_collective()
.EI
.SP 7 5

This method tests to see if this object is a collective or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is continuous, 0 if not.

.EP

.SI 3
* %^BOLD%^query_continuous%^RESET%^
.EI
.SI 5
int query_continuous()
.EI
.SP 7 5

This method tests to see if this object is continuous or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is continuous, 0 if not

.EP

.SI 3
* %^BOLD%^query_how_many%^RESET%^
.EI
.SI 5
int query_how_many()
.EI
.SP 7 5

This returns as an integer 'how many' we have of a particular
type.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int representing the amount of a particular pile.

.EP

.SI 3
* %^BOLD%^query_max_seen%^RESET%^
.EI
.SI 5
int query_max_seen()
.EI
.SP 7 5

This method returns the maximum number of piles of the
object that can be seen.  Any more than this turns into 'many'.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum number of piles that can be seen

.EP

.SI 3
* %^BOLD%^query_medium_alias%^RESET%^
.EI
.SI 5
string query_medium_alias()
.EI
.SP 7 5

This method returns the medium alias.  This should be a unique alias
for the medium, it is used when joining to see if we can join or not
It it used for identification - choose something with
capitals (e.g. "ChalkDust") so players can't
use it as a valid alias by accident.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the medium alias

.EP

.SI 3
* %^BOLD%^query_merger%^RESET%^
.EI
.SI 5
int query_merger()
.EI
.SP 7 5

This method returns 1 if the object is available for mergin.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we can merge

.EP

.SI 3
* %^BOLD%^query_no_join%^RESET%^
.EI
.SI 5
int query_no_join()
.EI
.SP 7 5

This method checks to see if we are allowed to join with other
objects when we encounter them.  A join will occur if we end up in
same environment and the medium aliases match.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they cannot join, 0 if they can

.EP

.SI 3
* %^BOLD%^query_parse_id%^RESET%^
.EI
.SI 5
object query_parse_id(mixed * args)
.EI

.SI 3
* %^BOLD%^query_pile_name%^RESET%^
.EI
.SI 5
string query_pile_name()
.EI
.SP 7 5

This method returns the name we give the piles of this stuff.  This
is the name given to piles of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the pile

.EP

.SI 3
* %^BOLD%^query_pile_types%^RESET%^
.EI
.SI 5
mixed query_pile_types()
.EI
.SP 7 5

This method returns the array of pile types.
The pile types array starts with a number, the number is the
cut off amount to get the size name in the next array element.
The numbers and strings alternate and the last element is the
the default size.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of pile types

.EP

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value()
.EI
.SP 7 5

This method returns the value of the object.  If the object is
not continuous, or the value_scale has not been set, the value is
the value set with set_value(), as usual.  Otherwise, the value is
scaled by value_scale and multiplied by the current amount.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the object
.EP

.SI 3
* %^BOLD%^query_value_scale%^RESET%^
.EI
.SI 5
float query_value_scale()
.EI
.SP 7 5

This method returns the current value scale.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value scale
.EP

.SI 3
* %^BOLD%^query_weight_unit%^RESET%^
.EI
.SI 5
int * query_weight_unit()
.EI
.SP 7 5

This method returns the weight units used for this type
of unit.  The weight unit is an array ({ a, b }) which means
that b units of stuff have weight a.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the weight array

.EP

.SI 3
* %^BOLD%^reset_collective%^RESET%^
.EI
.SI 5
void reset_collective()
.EI
.SP 7 5

This method removes the collective property from the object.

.EP

.SI 3
* %^BOLD%^reset_continuous%^RESET%^
.EI
.SI 5
void reset_continuous()
.EI
.SP 7 5

This method removes the continuous property from the object.

.EP

.SI 3
* %^BOLD%^reset_no_join%^RESET%^
.EI
.SI 5
void reset_no_join()
.EI
.SP 7 5

This method lets this object joining with other
objects when we encounter them.  A join will occur if we end up in
same environment and the medium aliases match.

.EP

.SI 3
* %^BOLD%^set_amount%^RESET%^
.EI
.SI 5
void set_amount(int number)
.EI
.SP 7 5

This method sets the amount of the stuff there is.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the amount to set it to

.EP

.SI 3
* %^BOLD%^set_amount_types%^RESET%^
.EI
.SI 5
void set_amount_types(mapping map)
.EI
.SP 7 5

This method sets the sizes of the piles and their names.  The
argument is a mapping which contains the name of the pile and
value is an array, the first element is the amount needed for
the name and the second element is the plural of the pile name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the amount types mapping

.EP

.SI 3
* %^BOLD%^set_collective%^RESET%^
.EI
.SI 5
void set_collective()
.EI

.SI 3
* %^BOLD%^set_continuous%^RESET%^
.EI
.SI 5
void set_continuous()
.EI
.SP 7 5

This method makes the object continuous.

.EP

.SI 3
* %^BOLD%^set_medium_alias%^RESET%^
.EI
.SI 5
void set_medium_alias(string word)
.EI
.SP 7 5

This method sets the medium alias for the object.
This should be a unique alias
for the medium, it is used when joining to see if we can join or not
It it used for identification - choose something with
capitals (e.g. "ChalkDust") so players can't
use it as a valid alias by accident.
& @param word the medium alias

.EP

.SI 3
* %^BOLD%^set_no_env_move%^RESET%^
.EI
.SI 5
void set_no_env_move(object dest)
.EI
.SP 7 5

This method is for dealing with the object when bits are shaved off.
If it is split up by someone referencing it we are moved into a
special environment so we do not interfer with the old pile.  The
property "medium dest" will be used as the place to go, otherwise
the environment will be used.  This method sticks it into a special
on the fly created container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - the place to move too  (this will be the "medium dest" or
the environment()

.EP

.SI 3
* %^BOLD%^set_no_join%^RESET%^
.EI
.SI 5
void set_no_join()
.EI
.SP 7 5

This method stops this object joining with other
objects when we encounter them.  A join will occur if we end up in
same environment and the medium aliases match.

.EP

.SI 3
* %^BOLD%^set_pile_name%^RESET%^
.EI
.SI 5
void set_pile_name(string word)
.EI
.SP 7 5

This method returns the name we give the piles of this stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the pile

.EP

.SI 3
* %^BOLD%^set_pile_types%^RESET%^
.EI
.SI 5
void set_pile_types(mixed * args)
.EI
.SP 7 5

This method sets the name of the piles given the different
amounts of piles available.  The last pile type in the list is
the size to use for anything larger.  The pile sizes must be in
order.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the pile types

.EP

.SI 3
* %^BOLD%^set_value_scale%^RESET%^
.EI
.SI 5
void set_value_scale(float f)
.EI
.SP 7 5

This method sets the amount by which to scale the value of a
continuous object.  If not set, the value will be that set with
set_value().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
f - the value scale
.EP

.SI 3
* %^BOLD%^set_weight_unit%^RESET%^
.EI
.SI 5
void set_weight_unit(int * numbers)
.EI
.SP 7 5

This method sets the weight units used for this type
of stuff.  The weight unit is an array ({ a, b }) which means
that b units of stuff have weight a.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
numbers - the new weight unit

.EP


