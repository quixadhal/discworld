.DT
glass.c
Disk World autodoc help
glass.c

.SH Description
.SP 5 5

A nice little inheritable and constructible glass object thing.

.EP
.SP 10 5


Written by Taffyd

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/vessel.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping map)
.EI

.SI 3
* %^BOLD%^make_glass%^RESET%^
.EI
.SI 5
void make_glass(string drink, string glass_description, string glass_size, int amount, int has_contents)
.EI
.SP 7 5

This function is used to transform the glass into a new type of glass, changing the 
short descriptions and adding aliases and adjectives.

If the 'contents' parameter is 1, then the glass is automatically filled
up with a drink made from /obj/reagents/generic_liquid.ob

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
drink - The name of the drink to be created
.EP
.SP 9 5
glass_description - The description given when there is something in the glass.
.EP
.SP 9 5
glass_size - The size of the glass, (in words), for example "large glass",
.EP
.SP 9 5
amount - The amount of liquid the glass should hold.
.EP
.SP 9 5
has_contents - Fill the glass up with 'drink'.
.EP


