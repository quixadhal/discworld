.DT
bit.c
Disk World autodoc help
bit.c

.SH Description
.SP 5 5

The bit object is the object created to handle bits of corpses, like
fingers and eyes and so on.
.EP
.SP 10 5


Written by Ember

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/corpse.h and /include/bit.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_bit_gone%^RESET%^
.EI
.SI 5
mixed * add_bit_gone(string bit)
.EI

.SI 3
* %^BOLD%^do_cure%^RESET%^
.EI
.SI 5
void do_cure()
.EI
.SP 7 5

This method cures the corpse.  A cured bit no longer decays.

.EP

.SI 3
* %^BOLD%^do_decay%^RESET%^
.EI
.SI 5
int do_decay()
.EI
.SP 7 5

This method is called by the bits controller to do the actual
decaying of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it still exists, 0 if it has finished decaying

.EP

.SI 3
* %^BOLD%^do_eat%^RESET%^
.EI
.SI 5
int do_eat()
.EI
.SP 7 5

The main eat entry point.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look()
.EI

.SI 3
* %^BOLD%^make_bit%^RESET%^
.EI
.SI 5
object make_bit(string which_bit)
.EI
.SP 7 5

This method creates a bit from the current bit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
which_bit - the name of the bit to create
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object that is the new bit

.EP

.SI 3
* %^BOLD%^make_bits%^RESET%^
.EI
.SI 5
object * make_bits(string * what_bits)
.EI
.SP 7 5

This method makes a bunch of bits at once.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
what_bits - the list of bits to create
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of made bits

.EP

.SI 3
* %^BOLD%^no_decay%^RESET%^
.EI
.SI 5
int no_decay()
.EI
.SP 7 5

THis method checks to see if the bit can inherently not decay.
Like trolls teeth and other such things.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it cannot decay, 0 if it can

.EP

.SI 3
* %^BOLD%^query_bit_data%^RESET%^
.EI
.SI 5
mixed query_bit_data()
.EI
.SP 7 5

This method returns the bit data for the bit.  The return array is of
the format same format as used in the races.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bit data
.EP

.SI 3
* %^BOLD%^query_bit_left%^RESET%^
.EI
.SI 5
string query_bit_left(string s)
.EI
.SP 7 5

This method figures out of the specified bit is left.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the bit to check if it is left

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the bit if it is left
.EP

.SI 3
* %^BOLD%^query_bit_left_pl%^RESET%^
.EI
.SI 5
string * query_bit_left_pl(string s)
.EI
.SP 7 5

This method figures out of the specified bit is left using the plural name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the bit to check if it is left

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the bit if it is left
.EP

.SI 3
* %^BOLD%^query_bits_gone%^RESET%^
.EI
.SI 5
string * query_bits_gone()
.EI
.SP 7 5

This method returns the current array of bits gone from this
bit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of missing bits

.EP

.SI 3
* %^BOLD%^query_bits_left%^RESET%^
.EI
.SI 5
string * query_bits_left()
.EI
.SP 7 5

This method returns the current array of bits left.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of bits left

.EP

.SI 3
* %^BOLD%^query_collective%^RESET%^
.EI
.SI 5
int query_collective()
.EI

.SI 3
* %^BOLD%^query_cured%^RESET%^
.EI
.SI 5
int query_cured()
.EI
.SP 7 5

This returns the cured state of the bit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is cured, 0 if not

.EP

.SI 3
* %^BOLD%^query_decay%^RESET%^
.EI
.SI 5
int query_decay()
.EI
.SP 7 5

This method returns the current decay level of the corpse
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current decay level

.EP

.SI 3
* %^BOLD%^query_determinate%^RESET%^
.EI
.SI 5
string query_determinate(object caller)
.EI

.SI 3
* %^BOLD%^query_edible%^RESET%^
.EI
.SI 5
int query_edible()
.EI
.SP 7 5

This method checks to see if the bit is edible or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is edible, 0 if not

.EP

.SI 3
* %^BOLD%^query_medium_alias%^RESET%^
.EI
.SI 5
string query_medium_alias()
.EI

.SI 3
* %^BOLD%^query_possible_bits%^RESET%^
.EI
.SI 5
string * query_possible_bits(string word)
.EI
.SP 7 5

This method returns the possible bits that can be removed
from this bit.  If the bits are restricted then the
possible bits are the ones that are contained by that bit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - restrict the possible bits
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of possible bits

.EP

.SI 3
* %^BOLD%^query_possible_plural_bits%^RESET%^
.EI
.SI 5
string * query_possible_plural_bits(string word)
.EI
.SP 7 5

This returns the plural of all the possible bit names.
If the bits are restricted then the
possible bits are the ones that are contained by that bit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - restrict the possible bits
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of possible bit names

.EP

.SI 3
* %^BOLD%^query_race_name%^RESET%^
.EI
.SI 5
string query_race_name()
.EI
.SP 7 5

This method returns the name of the race associated with this
bit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the race

.EP

.SI 3
* %^BOLD%^query_race_ob%^RESET%^
.EI
.SI 5
object query_race_ob()
.EI
.SP 7 5

This method returns the race object associated with the bit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the race object

.EP

.SI 3
* %^BOLD%^query_vect%^RESET%^
.EI
.SI 5
int * query_vect()
.EI
.SP 7 5

This method returns the vector used with the potion space.  I 
think this call is now obsolete and not used.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the vectorof the bit

.EP

.SI 3
* %^BOLD%^set_bit%^RESET%^
.EI
.SI 5
void set_bit(string s, int dec)
.EI
.SP 7 5

This method sets the bit that this actually is.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the name of the bit
.EP
.SP 9 5
dec - the current decay numbe

.EP

.SI 3
* %^BOLD%^set_bits%^RESET%^
.EI
.SI 5
void set_bits()
.EI
.SP 7 5

This method sets all the basic bits for the bit when it is first created.

.EP

.SI 3
* %^BOLD%^set_bits_gone%^RESET%^
.EI
.SI 5
void set_bits_gone(string * bits)
.EI
.SP 7 5

This method sets the array of gone bits on the bit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bits - the set of bits gone to set

.EP

.SI 3
* %^BOLD%^set_corpse_weight%^RESET%^
.EI
.SI 5
void set_corpse_weight(int i)
.EI
.SP 7 5

This method sets the weight of the bit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the weight of the bit

.EP

.SI 3
* %^BOLD%^set_race_name%^RESET%^
.EI
.SI 5
void set_race_name(string s)
.EI
.SP 7 5

This method sets the name of the race associated with the corpse.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the race name to set

.EP

.SI 3
* %^BOLD%^set_race_ob%^RESET%^
.EI
.SI 5
void set_race_ob(object s)
.EI
.SP 7 5

This method sets the race object associated with the corpse.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the race object to set

.EP

.SI 3
* %^BOLD%^setup_long%^RESET%^
.EI
.SI 5
void setup_long()
.EI
.SP 7 5

This method setsup the long and short based on the current decay
levels of the corpse.

.EP


