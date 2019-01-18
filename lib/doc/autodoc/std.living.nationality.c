.DT
nationality.c
Disk World autodoc help
nationality.c

.SH Description
.SP 5 5

The details for handling nationality.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Jun 26 13:22:26 PDT 2002

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h and /include/living.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_nationality%^RESET%^
.EI
.SI 5
string query_nationality()
.EI
.SP 7 5

This method returns the nationality of the living.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nationality of the living object

.EP

.SI 3
* %^BOLD%^query_nationality_accent_ob%^RESET%^
.EI
.SI 5
string query_nationality_accent_ob()
.EI
.SP 7 5

This method returns the accent to use for this nationality/region
setup on the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the accent object to use

.EP

.SI 3
* %^BOLD%^query_nationality_data%^RESET%^
.EI
.SI 5
mixed query_nationality_data()
.EI
.SP 7 5

This method returns the nationality data of the living.  The
data is any free form data needed by the nationality.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nationality data of the living object

.EP

.SI 3
* %^BOLD%^query_nationality_description%^RESET%^
.EI
.SI 5
string query_nationality_description(object looker)
.EI
.SP 7 5

This method returns the description of the nationality.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
looker - the person looking at the them
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the nationality

.EP

.SI 3
* %^BOLD%^query_nationality_name%^RESET%^
.EI
.SI 5
string query_nationality_name()
.EI
.SP 7 5

This method returns the name of the nationality.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the nationality

.EP

.SI 3
* %^BOLD%^query_nationality_region%^RESET%^
.EI
.SI 5
string query_nationality_region()
.EI
.SP 7 5

This method returns the nationality region of the living.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nationality region of the living object

.EP

.SI 3
* %^BOLD%^query_nationality_start_location%^RESET%^
.EI
.SI 5
string query_nationality_start_location()
.EI
.SP 7 5

This method returns the start location for this player/npc.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the start location

.EP

.SI 3
* %^BOLD%^set_nationality%^RESET%^
.EI
.SI 5
void set_nationality(string nationality)
.EI
.SP 7 5

This method sets the nationality of the living.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
nationaltiy - the new nationality

.EP

.SI 3
* %^BOLD%^set_nationality_data%^RESET%^
.EI
.SI 5
void set_nationality_data(mixed data)
.EI
.SP 7 5

This method sets the nationality data of the person.  The
data is any extre free form data needed by the nationality.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
data - the new data for the living

.EP

.SI 3
* %^BOLD%^set_nationality_region%^RESET%^
.EI
.SI 5
void set_nationality_region(string region)
.EI
.SP 7 5

This method sets the nationality region of the person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
region - the new region of the person

.EP


