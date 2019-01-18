.DT
skills.h
Disk World autodoc help
skills.h

.SH Description
.SP 5 5

The main skills header file.  This contains the skill tree and any bits
relating to the skill tree.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/std/skills.c and /std/living/skills.c

.EP
.SH Defines
.SI 3
* %^BOLD%^SKILL_ARR_SIZE%^RESET%^
.EI
.SP 7 5
The size of the skill array. 
.EP

.SI 3
* %^BOLD%^SKILL_BIT%^RESET%^
.EI
.SP 7 5
The index corresponding to the array of sub skills in the skill array. 
.EP

.SI 3
* %^BOLD%^SKILL_EXP%^RESET%^
.EI
.SP 7 5
The index corresponding to the skill exp in the skill array. 
.EP

.SI 3
* %^BOLD%^SKILL_LVL%^RESET%^
.EI
.SP 7 5
The index corresponding to the skill level in the skill array. 
.EP

.SI 3
* %^BOLD%^SKILL_MAX_TEACH%^RESET%^
.EI
.SP 7 5
The maximum level that you can teach someone else too. 
.EP

.SI 3
* %^BOLD%^SKILL_NAM%^RESET%^
.EI
.SP 7 5
The index corresponding to the skill name in the skill array. 
.EP

.SI 3
* %^BOLD%^SKILL_OB%^RESET%^
.EI
.SP 7 5
The skill handling object. 
.EP

.SI 3
* %^BOLD%^STD_SKILLS%^RESET%^
.EI
.SP 7 5

The complete standard skill tree.  This should be avoided being used
if possible.
.EP

