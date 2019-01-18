.DT
wallpaper.c
Disk World autodoc help
wallpaper.c

.SH Description
.SP 5 5

 *  A wallpaper object for playerhousing
 *
 *  To create a roll with a pattern, just call make_roll( <design> ) * on one. * * To apply this wallpaper to a playerhousing wall, you also need * a pot of glue and a paint brush. * * @author Una * @27th August, 2002 * *
.EP
.SP 10 5

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
* %^BOLD%^do_paper%^RESET%^
.EI
.SI 5
varargs int do_paper(string indirect, object pot)
.EI

.SI 3
* %^BOLD%^make_roll%^RESET%^
.EI
.SI 5
void make_roll(string design)
.EI

.SI 3
* %^BOLD%^query_paper_mess%^RESET%^
.EI
.SI 5
string query_paper_mess()
.EI


