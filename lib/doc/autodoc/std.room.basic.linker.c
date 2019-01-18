.DT
linker.c
Disk World autodoc help
linker.c

.SH Includes
.SP 5 5
This class includes the following files /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^area_name%^RESET%^
.EI
.SI 5
string area_name()
.EI

.SI 3
* %^BOLD%^compose_start%^RESET%^
.EI
.SI 5
string compose_start()
.EI

.SI 3
* %^BOLD%^destruct_shadow%^RESET%^
.EI
.SI 5
void destruct_shadow(object thing)
.EI

.SI 3
* %^BOLD%^event_enter%^RESET%^
.EI
.SI 5
void event_enter(object person, string message, object from)
.EI

.SI 3
* %^BOLD%^event_exit%^RESET%^
.EI
.SI 5
void event_exit(object person, string message, object to)
.EI

.SI 3
* %^BOLD%^event_person_say%^RESET%^
.EI
.SI 5
void event_person_say(object person, string start, string rest, string language, string accent)
.EI

.SI 3
* %^BOLD%^query_dynamic_preposition%^RESET%^
.EI
.SI 5
string query_dynamic_preposition()
.EI

.SI 3
* %^BOLD%^query_linked%^RESET%^
.EI
.SI 5
int query_linked()
.EI

.SI 3
* %^BOLD%^query_room_name%^RESET%^
.EI
.SI 5
string query_room_name()
.EI

.SI 3
* %^BOLD%^query_rooms%^RESET%^
.EI
.SI 5
string * query_rooms()
.EI

.SI 3
* %^BOLD%^query_static_preposition%^RESET%^
.EI
.SI 5
string query_static_preposition()
.EI

.SI 3
* %^BOLD%^setup_shadow%^RESET%^
.EI
.SI 5
void setup_shadow(object thing, string * words, string word1, string word2, string word3)
.EI


