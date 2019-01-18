.DT
water_inside.c
Disk World autodoc help
water_inside.c

.SH Description
.SP 5 5

Standard inside water room inheritable.  This is mainly just a mux
for the functions that are defined in both /std/room and
/std/room/basic/water.
.EP
.SP 10 5


Written by Presto, Jeremy, Bakhtosh
.EP



.SH See also
.SP 5 5
/std/room.c and /std/room/basic/water.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/room/basic/water.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^attack_speed%^RESET%^
.EI
.SI 5
int attack_speed()
.EI
.SP 7 5

This method sets the default attack speed for the room.
This defaults to 5 for water rooms.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default attack speed

.EP

.SI 3
* %^BOLD%^set_water_light%^RESET%^
.EI
.SI 5
int set_water_light(int number)
.EI
.SP 7 5

This function is used to set the light in a water room without overriding
the use of set_use_surface_light(1).  There should be no reason to use it
outside of the water inheritable.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new light level
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new light level
.EP


