.DT
implement.c
DW_wibble autodoc help
implement.c

.SH Description
.SP 5 5

This is a generic held implement used for cutlery, lockpicks, tissues,
basically anything you could hold in your hand but which would not be a
weapon. Now of course you could try and fight someone with a dessert
spoon but it would be pretty silly so use this instead of a weapon.

Best way to use this is via the .imp virtual object.

.EP
.SP 10 5


Written by Ceres
.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/holdable and /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/money.h, /include/virtual.h, /include/move_failures.h and /include/shops/bank.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map,  object)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping map)
.EI

.SI 3
* %^BOLD%^int_query_static_auto_load%^RESET%^
.EI
.SI 5
mapping int_query_static_auto_load()
.EI

.SI 3
* %^BOLD%^player_wield%^RESET%^
.EI
.SI 5
void player_wield(int pos)
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI


