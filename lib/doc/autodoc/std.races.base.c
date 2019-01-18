.DT
base.c
Disk World autodoc help
base.c

.SH Description
.SP 5 5

The base object which all races will be inherited from.
.EP
.SP 10 5


Written by Pinkfish

Started 1992 sometime

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
* %^BOLD%^add_ac%^RESET%^
.EI
.SI 5
void add_ac(string ac_name, string ac_type, mixed ac_amount)
.EI
.SP 7 5

Adds a default ac to the npc for when they fight unarmed.  Please look
add the armour object for more details on this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ac_name - the name of the ac
.EP
.SP 9 5
ac_type - the type of the ac
.EP
.SP 9 5
ac_amount - the damage/roll stuff to take off
.EP

.SI 3
* %^BOLD%^add_attack%^RESET%^
.EI
.SI 5
void add_attack(string attack_name, int attack_chance, int * attack_data)
.EI
.SP 7 5

This method adds a default attack to the npc.  Please see the
help on the weapons for more details on this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
attack_name - the name of the attack
.EP
.SP 9 5
attack_chance - the chance of the attack occuring
.EP
.SP 9 5
attack_data - the damage roll for the attack
.EP

.SI 3
* %^BOLD%^add_bit%^RESET%^
.EI
.SI 5
void add_bit(string bit_short, string bit_name, mixed * bit_array)
.EI
.SP 7 5

This method adds a bit to the current bit setup.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bit_short - the short of the bit
.EP
.SP 9 5
bit_name - the name of the bit
.EP
.SP 9 5
bit_array - the array of children bits
.EP

.SI 3
* %^BOLD%^find_limbs%^RESET%^
.EI
.SI 5
void find_limbs()
.EI
.SP 7 5

This method figures out the current set of limbs on the race.  A creature
is assigned one "limb" for every "hand" found in the body part array.  The
number of limbs is the number of weapons that can be held.
.EP

.SI 3
* %^BOLD%^modifier%^RESET%^
.EI
.SI 5
int modifier(int number)
.EI
.SP 7 5

This method figures out a modifier for the height based on the base
weight and height.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the height/weight to modify
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the modified heigh/weight

.EP

.SI 3
* %^BOLD%^query_acs%^RESET%^
.EI
.SI 5
mixed * query_acs()
.EI
.SP 7 5

The defaults acs for npcs when they are fighting unarmed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default acs
.EP

.SI 3
* %^BOLD%^query_attacks%^RESET%^
.EI
.SI 5
mixed * query_attacks()
.EI
.SP 7 5

The default attacks for the npc when they are fighting unarmed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default attacks
.EP

.SI 3
* %^BOLD%^query_bit%^RESET%^
.EI
.SI 5
mixed * query_bit(string word)
.EI
.SP 7 5

This method returns the bits which are children of the specified bit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the bit to look for the children of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the children bits
.EP

.SI 3
* %^BOLD%^query_bits%^RESET%^
.EI
.SI 5
mixed * query_bits()
.EI
.SP 7 5

This method returns all the bits for the npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the complete array of bits
.EP

.SI 3
* %^BOLD%^query_dark%^RESET%^
.EI
.SI 5
int query_dark(int light)
.EI
.SP 7 5

Determines if the race thinks it is dark or light.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 for normal, -1 for dark, -2 for pitch dark, 1 for bright,
2 for toobright
.EP

.SI 3
* %^BOLD%^query_desc%^RESET%^
.EI
.SI 5
string query_desc(object thing)
.EI
.SP 7 5

This method returns the current description of the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object which is being described
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the description of the npc
.EP

.SI 3
* %^BOLD%^query_height%^RESET%^
.EI
.SI 5
int query_height()
.EI
.SP 7 5

This method returns the base height of the npc type.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current base height
.EP

.SI 3
* %^BOLD%^query_limbs%^RESET%^
.EI
.SI 5
string * query_limbs()
.EI
.SP 7 5

This method returns the current limbs of the race.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the limbs on the race
.EP

.SI 3
* %^BOLD%^query_number_worn%^RESET%^
.EI
.SI 5
int query_number_worn(string type)
.EI
.SP 7 5

This method figures out how many of each sort of thing 
can be worn.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to check for numbers worn
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of type to worn

.EP

.SI 3
* %^BOLD%^query_possible_bits%^RESET%^
.EI
.SI 5
string * query_possible_bits(string word)
.EI
.SP 7 5

This method returns the names of all the possible bits for the
npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the bits to look for the children under
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of possible bit names
.EP

.SI 3
* %^BOLD%^query_possible_plural_bits%^RESET%^
.EI
.SI 5
string * query_possible_plural_bits(string word)
.EI
.SP 7 5

This method returns the plural names for all the bits.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the bit to find the children of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the plural names of all the bits
.EP

.SI 3
* %^BOLD%^query_sight%^RESET%^
.EI
.SI 5
int * query_sight()
.EI
.SP 7 5

Thie method returns the values of the race which determine visibility.
It returns an array of the format:
.EP
.SP 7 5
({
.EP
.SP 7 5
   dark
.EP
.SP 7 5
   pitch dark,
.EP
.SP 7 5
   bright light,
.EP
.SP 7 5
   bright
.EP
.SP 7 5
})
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sight array as detailed above
.EP

.SI 3
* %^BOLD%^query_skin%^RESET%^
.EI
.SI 5
string query_skin()
.EI
.SP 7 5

This method returns the current skin of the race.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current skin of the race
.EP

.SI 3
* %^BOLD%^query_stats%^RESET%^
.EI
.SI 5
int * query_stats()
.EI
.SP 7 5

This method queries the default bonus stat values for the race.
.EP
.SP 7 5
({
.EP
.SP 7 5
  con,
.EP
.SP 7 5
  dex,
.EP
.SP 7 5
  int,
.EP
.SP 7 5
  str,
.EP
.SP 7 5
  wiz
.EP
.SP 7 5
})
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current stat offsets
.EP

.SI 3
* %^BOLD%^remove_bit%^RESET%^
.EI
.SI 5
void remove_bit(string word)
.EI
.SP 7 5

This method removes a bit from the current bit array
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the bit to remove
.EP

.SI 3
* %^BOLD%^set_desc%^RESET%^
.EI
.SI 5
void set_desc(string words)
.EI
.SP 7 5

This method sets the description for the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the description of the npc
.EP

.SI 3
* %^BOLD%^set_height%^RESET%^
.EI
.SI 5
void set_height(int number)
.EI
.SP 7 5

This method sets the base height of the npc type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new height
.EP

.SI 3
* %^BOLD%^set_sight%^RESET%^
.EI
.SI 5
void set_sight(int * numbers)
.EI
.SP 7 5

Thie method sets the current sight array for the npc.  This sets the
levels at which the race things things are too dark/too bright.
.EP
.SP 7 5
({
.EP
.SP 7 5
   pitch dark,
.EP
.SP 7 5
   dark,
.EP
.SP 7 5
   bright,
.EP
.SP 7 5
   bright light
.EP
.SP 7 5
})
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
numbers - as detailed above
.EP

.SI 3
* %^BOLD%^set_skin%^RESET%^
.EI
.SI 5
void set_skin(string word)
.EI
.SP 7 5

This method sets the skin of the race.  This is what is given back
when the npc's corpse is skinned.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the skin of the race
.EP

.SI 3
* %^BOLD%^set_stats%^RESET%^
.EI
.SI 5
void set_stats(int * numbers)
.EI
.SP 7 5

This method sets the current default bonuses of the stats.  The parameter
is an array of numbers, with the numbers corresponding to the stats as
shown below.  It is highly inadvisable to set any stat bonus to less than
-6, since in conjunction with the guild stat settings this could combine
to give a negative total for that stat.  Negative stats can cause
permanent unconsciousness.
({
.EP
.SP 7 5
  con,
.EP
.SP 7 5
  dex,
.EP
.SP 7 5
  int,
.EP
.SP 7 5
  str,
.EP
.SP 7 5
  wis
.EP
.SP 7 5
})
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
numbers - sets the default bonuses for the stats
.EP

.SI 3
* %^BOLD%^start_player%^RESET%^
.EI
.SI 5
void start_player(object thing)
.EI
.SP 7 5

This method is called when the npc is first created.  This sets up
the weight/height/stats etc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the npc being created

.EP


