.DT
storeroom.c
Disk World autodoc help
storeroom.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
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
* %^BOLD%^add_thing%^RESET%^
.EI
.SI 5
void add_thing(object ob)
.EI

.SI 3
* %^BOLD%^clean_up%^RESET%^
.EI
.SI 5
int clean_up( int)
.EI

.SI 3
* %^BOLD%^do_dest%^RESET%^
.EI
.SI 5
void do_dest(object thing)
.EI

.SI 3
* %^BOLD%^event_enter%^RESET%^
.EI
.SI 5
void event_enter(object ob,  string,  object)
.EI

.SI 3
* %^BOLD%^event_exit%^RESET%^
.EI
.SI 5
void event_exit(object ob, string mesage, object to)
.EI

.SI 3
* %^BOLD%^query_max_num%^RESET%^
.EI
.SI 5
int query_max_num()
.EI

.SI 3
* %^BOLD%^query_room_cont%^RESET%^
.EI
.SI 5
mapping query_room_cont()
.EI

.SI 3
* %^BOLD%^query_shop_type_mapping%^RESET%^
.EI
.SI 5
mapping query_shop_type_mapping()
.EI

.SI 3
* %^BOLD%^remove_thing%^RESET%^
.EI
.SI 5
void remove_thing(object ob)
.EI

.SI 3
* %^BOLD%^set_max_num%^RESET%^
.EI
.SI 5
void set_max_num(int i)
.EI

.SI 3
* %^BOLD%^test_cont%^RESET%^
.EI
.SI 5
void test_cont()
.EI


