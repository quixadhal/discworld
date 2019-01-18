.DT
potion_space.c
Disk World autodoc help
potion_space.c

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_effect%^RESET%^
.EI
.SI 5
int add_effect(string ob, int x1, int x2, int y1, int y2)
.EI

.SI 3
* %^BOLD%^neutral_coordinate%^RESET%^
.EI
.SI 5
int * neutral_coordinate()
.EI

.SI 3
* %^BOLD%^potion_create%^RESET%^
.EI
.SI 5
void potion_create(mixed ob, int * coord, int quantity)
.EI

.SI 3
* %^BOLD%^potion_drunk%^RESET%^
.EI
.SI 5
void potion_drunk(mixed ob, int * coord, int quantity)
.EI

.SI 3
* %^BOLD%^potion_smell%^RESET%^
.EI
.SI 5
void potion_smell(mixed ob, int * coord, int quantity)
.EI

.SI 3
* %^BOLD%^potion_touch%^RESET%^
.EI
.SI 5
void potion_touch(mixed ob, int * coord, int quantity)
.EI

.SI 3
* %^BOLD%^query_attrs_at%^RESET%^
.EI
.SI 5
mixed * query_attrs_at(int * coord)
.EI

.SI 3
* %^BOLD%^query_effect_at%^RESET%^
.EI
.SI 5
string * query_effect_at(int * coord)
.EI

.SI 3
* %^BOLD%^query_effect_vols%^RESET%^
.EI
.SI 5
mixed * query_effect_vols()
.EI

.SI 3
* %^BOLD%^remove_effect%^RESET%^
.EI
.SI 5
int remove_effect(object ob)
.EI


