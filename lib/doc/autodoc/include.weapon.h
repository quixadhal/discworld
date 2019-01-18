.DT
weapon.h
Disk World autodoc help
weapon.h

.SH Description
.SP 5 5

These macros are used to index the array returned by query_attack_data() in
/std/weapon_logic.c

.EP
.SP 10 5

.EP

.SH Defines
.SI 3
* %^BOLD%^A_AC%^RESET%^
.EI

.SI 3
* %^BOLD%^A_ARRAY_SIZE%^RESET%^
.EI
.SP 7 5

These macros index the armour_types data stored in /std/armour_logic.c
Quite why they're in weapon.h is beyond me -- Ceres.

.EP

.SI 3
* %^BOLD%^A_NAME%^RESET%^
.EI

.SI 3
* %^BOLD%^A_TYPE%^RESET%^
.EI

.SI 3
* %^BOLD%^AT_ARRAY_SIZE%^RESET%^
.EI
.SP 7 5

These macros are used to index the array returned by weapon_attacks() in
/std/weapon_logic.c

.EP

.SI 3
* %^BOLD%^AT_DAMAGE%^RESET%^
.EI

.SI 3
* %^BOLD%^AT_NAME%^RESET%^
.EI

.SI 3
* %^BOLD%^AT_SKILL%^RESET%^
.EI

.SI 3
* %^BOLD%^AT_TYPE%^RESET%^
.EI

.SI 3
* %^BOLD%^F_DIE%^RESET%^
.EI

.SI 3
* %^BOLD%^F_FIXED%^RESET%^
.EI
.SP 7 5

The W_DAMAGE index of query_attack_data() can be an integer or a three
index array. These macros index that array.

.EP

.SI 3
* %^BOLD%^F_NUM%^RESET%^
.EI

.SI 3
* %^BOLD%^W_ARRAY_SIZE%^RESET%^
.EI

.SI 3
* %^BOLD%^W_CHANCE%^RESET%^
.EI

.SI 3
* %^BOLD%^W_DAMAGE%^RESET%^
.EI

.SI 3
* %^BOLD%^W_FUNCTION%^RESET%^
.EI

.SI 3
* %^BOLD%^W_SKILL%^RESET%^
.EI

.SI 3
* %^BOLD%^W_TYPE%^RESET%^
.EI

