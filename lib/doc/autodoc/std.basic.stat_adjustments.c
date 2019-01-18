.DT
stat_adjustments.c
Disk World autodoc help
stat_adjustments.c

.SH Description
.SP 5 5

This is the stat adjustment inheritable will add stat
adjustments to the player.  It will also keep track of who they are added to
so they can be removed correctly.
.EP
.SP 10 5


Written by Pinkfish

Started Sun May  7 14:41:09 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/effects/stat_adjustment.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_stat_adjustments%^RESET%^
.EI
.SI 5
mapping query_stat_adjustments(object play)
.EI
.SP 7 5

This method returns the current stat adjustments on the specified
player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
play - the player to find the stat adjustments for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the stat adjustments

.EP

.SI 3
* %^BOLD%^remove_all_stat_adjustments%^RESET%^
.EI
.SI 5
void remove_all_stat_adjustments()
.EI
.SP 7 5

This method removes all the stat adjusments currently setup on
the object.

.EP

.SI 3
* %^BOLD%^remove_stat_adjustment%^RESET%^
.EI
.SI 5
void remove_stat_adjustment(object player)
.EI
.SP 7 5

This method removes the stat adjustments from the specified player
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to remove the stat adjustments from

.EP

.SI 3
* %^BOLD%^set_stat_adjustment%^RESET%^
.EI
.SI 5
void set_stat_adjustment(object player, mapping adjs)
.EI
.SP 7 5

This method adds a set of stat adjustments to a specified player

.EP
.SP 7 5
 ([
.EP
.SP 7 5
    "str" : 1,
.EP
.SP 7 5
    "dex" : -1,
.EP
.SP 7 5
    "wis" : 3
.EP
.SP 7 5
 ])
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to add the adjustments too
.EP
.SP 9 5
adjs - the adjustments to add

.EP


