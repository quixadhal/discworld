.DT
master.c
Disk World autodoc help
master.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/dom/base_master.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^author_file%^RESET%^
.EI
.SI 5
string author_file(string * path)
.EI

.SI 3
* %^BOLD%^check_permission%^RESET%^
.EI
.SI 5
int check_permission(string euid, string * path, int mask)
.EI

.SI 3
* %^BOLD%^log_who%^RESET%^
.EI
.SI 5
string log_who(string where)
.EI

.SI 3
* %^BOLD%^query_info%^RESET%^
.EI
.SI 5
string query_info()
.EI

.SI 3
* %^BOLD%^query_lord%^RESET%^
.EI
.SI 5
string query_lord()
.EI

.SI 3
* %^BOLD%^query_member%^RESET%^
.EI
.SI 5
int query_member(string name)
.EI


