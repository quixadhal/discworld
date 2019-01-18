.DT
quill.c
Disk World autodoc help
quill.c

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/weapon.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/language.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^begin_writing%^RESET%^
.EI
.SI 5
void begin_writing(object writer)
.EI

.SI 3
* %^BOLD%^do_copy%^RESET%^
.EI
.SI 5
int do_copy(mixed * in_dir, string direct, string indirect, mixed * args)
.EI

.SI 3
* %^BOLD%^do_scribble%^RESET%^
.EI
.SI 5
int do_scribble(object * things, string mess)
.EI

.SI 3
* %^BOLD%^do_sign%^RESET%^
.EI
.SI 5
int do_sign(object * things)
.EI

.SI 3
* %^BOLD%^do_write%^RESET%^
.EI
.SI 5
int do_write(object * things)
.EI

.SI 3
* %^BOLD%^end_writing%^RESET%^
.EI
.SI 5
void end_writing(string words)
.EI


