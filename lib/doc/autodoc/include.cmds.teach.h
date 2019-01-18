.DT
teach.h
Disk World autodoc help
teach.h

.SH Description
.SP 5 5

This file contains all the stuff to do with the teaching respondable
command.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Aug 16 23:39:28 PDT 1998

.EP

.SH Classes

.SI 3
* %^BOLD%^teaching_skill%^RESET%^
class teaching_skill {
string skill;
int lvl;
int num;
int xp;
object teacher;
object taught;
object room;
}

.EI
.SP 7 5

The class to keep track of everything to do with teaching a skill.

.EP


.SH Defines
.SI 3
* %^BOLD%^TEACH_COMMAND_AUTO_PROPERTY%^RESET%^
.EI
.SP 7 5

This is the property to use to test to see if the player has
automatic teaching turned on or not.

.EP

.SI 3
* %^BOLD%^TEACH_COMMAND_TYPE%^RESET%^
.EI
.SP 7 5

This is the type used for teaching commands.

.EP

.SI 3
* %^BOLD%^TEACH_SKILL_TYPE%^RESET%^
.EI
.SP 7 5

This is the type used for teaching skills.

.EP

