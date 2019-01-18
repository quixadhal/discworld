.DT
scroll.c
Disk World autodoc help
scroll.c

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
void add_read_mess(string mess, string type, string lang, string size)
.EI

.SI 3
* %^BOLD%^do_scroll%^RESET%^
.EI
.SI 5
int do_scroll(string word)
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping args)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping args)
.EI

.SI 3
* %^BOLD%^int_query_static_auto_load%^RESET%^
.EI
.SI 5
mapping int_query_static_auto_load()
.EI

.SI 3
* %^BOLD%^query_auto_dynamic_load%^RESET%^
.EI
.SI 5
mapping query_auto_dynamic_load()
.EI

.SI 3
* %^BOLD%^query_num_pages%^RESET%^
.EI
.SI 5
int query_num_pages()
.EI

.SI 3
* %^BOLD%^query_open_page%^RESET%^
.EI
.SI 5
int query_open_page()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mixed query_static_auto_load()
.EI

.SI 3
* %^BOLD%^set_num_pages%^RESET%^
.EI
.SI 5
void set_num_pages(int number)
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
void set_read_mess(string mess, string lang, int size)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


