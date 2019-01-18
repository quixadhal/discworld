.DT
player_craft_shop.c
Disk World autodoc help
player_craft_shop.c

.SH Description
.SP 5 5

This is just like a normal craft shop, except it to be used by players.
It handles things like messing with the descriptions and so on, it is
mostly designed to be used in places like club rooms and things.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Sep 20 19:28:22 PDT 1999

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room, /std/room/player_housing and /std/shops/inherit/player_craft_shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^is_allowed%^RESET%^
.EI
.SI 5
int is_allowed(string name)
.EI
.SP 7 5

This method tells us if the person is allowed to use this shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the person

.EP


