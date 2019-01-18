.DT
leaflet.c
Disk World autodoc help
leaflet.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_read_mess%^RESET%^
.EI
.SI 5
mixed add_read_mess(mixed mess, string type, string lang, int size)
.EI

.SI 3
* %^BOLD%^do_open%^RESET%^
.EI
.SI 5
int do_open(int page_no)
.EI

.SI 3
* %^BOLD%^do_turn%^RESET%^
.EI
.SI 5
int do_turn()
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_leaflet%^RESET%^
.EI
.SI 5
int query_leaflet()
.EI

.SI 3
* %^BOLD%^query_open_page%^RESET%^
.EI
.SI 5
int query_open_page()
.EI

.SI 3
* %^BOLD%^query_page%^RESET%^
.EI
.SI 5
int query_page()
.EI

.SI 3
* %^BOLD%^query_pages%^RESET%^
.EI
.SI 5
mixed * query_pages()
.EI

.SI 3
* %^BOLD%^query_read_short%^RESET%^
.EI
.SI 5
string query_read_short(object player, int ignore_labels)
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^set_no_pages%^RESET%^
.EI
.SI 5
void set_no_pages(int number)
.EI

.SI 3
* %^BOLD%^set_open_page%^RESET%^
.EI
.SI 5
void set_open_page(int number)
.EI

.SI 3
* %^BOLD%^set_read_mess%^RESET%^
.EI
.SI 5
varargs void set_read_mess(mixed mess, string lang, int size)
.EI


