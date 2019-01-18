.DT
corpse.c
Disk World autodoc help
corpse.c

.SH Description
.SP 5 5

This method will allow all sorts of modifications to a corpse once it has
been created, based on the weapon or attack used for the killing stroke.
.EP
.SP 10 5

.EP



.SH See also
.SP 5 5
death_helper(), make_corpse() and do_death()

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/player.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^alter_corpse%^RESET%^
.EI
.SI 5
int alter_corpse(object corpse, object weapon, string attack)
.EI

.SI 3
* %^BOLD%^death_helper%^RESET%^
.EI
.SI 5
mixed * death_helper(object thing, int death)
.EI
.SP 7 5

This method deals with any cute messages you want to print when
something dies or, cute things you want to do (like not dieng
at all).  It handles the alignment shift due to the killing of
this npc as well.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing which killed us
.EP

.SI 3
* %^BOLD%^do_death%^RESET%^
.EI
.SI 5
varargs object do_death(object thing, object weapon, string attack)
.EI
.SP 7 5

This does the actual death and co ordinates the death into a well
ordered feeding frenzy.  This method creates the actual corpse itself.
If the property "dead" is set on the object no corpse will be
created, or if the second_life() function called on the object
returns a non-zero value the corpse will not be created.


This method calls the second_life() function on the current object, if this returns 1 it must handle all the the death code itself. This is used in the player code to override the death code.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing which killed us
.EP
.SP 9 5
weapon - the weapon (sword,claw,etc) object that attacked and killed us
.EP
.SP 9 5
attack - the actual attack that killed us
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the corpse, or 0 if no more action is to be taken
.EP

.SI 3
* %^BOLD%^make_corpse%^RESET%^
.EI
.SI 5
object make_corpse()
.EI
.SP 7 5

This method will create a corpse for the living object when it
eventually dies.  Of old age of course, no one would die of having
sword rammed through them
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the created corpse object

.EP


