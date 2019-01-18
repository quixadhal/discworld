.DT
parser.c
Disk World autodoc help
parser.c

.SH Includes
.SP 5 5
This class includes the following files /include/tokenise.h, /include/parser.h and /include/creator.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^eval%^RESET%^
.EI
.SI 5
mixed eval()
.EI

.SI 3
* %^BOLD%^expr%^RESET%^
.EI
.SI 5
mixed expr()
.EI

.SI 3
* %^BOLD%^finish_expr%^RESET%^
.EI
.SI 5
void finish_expr()
.EI

.SI 3
* %^BOLD%^inform_of_call%^RESET%^
.EI
.SI 5
void inform_of_call(object ob, mixed * argv)
.EI

.SI 3
* %^BOLD%^init_expr%^RESET%^
.EI
.SI 5
void init_expr(string expr)
.EI

.SI 3
* %^BOLD%^parse_args%^RESET%^
.EI
.SI 5
mixed * parse_args(string str, string close)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^bit2%^RESET%^
.EI
.SI 5
mixed bit2()
.EI

.SI 3
* %^BOLD%^bit3%^RESET%^
.EI
.SI 5
mixed bit3()
.EI

.SI 3
* %^BOLD%^bit4%^RESET%^
.EI
.SI 5
mixed bit4()
.EI

.SI 3
* %^BOLD%^do_function_call%^RESET%^
.EI
.SI 5
mixed do_function_call(object ob, string name, mixed stuff)
.EI

.SI 3
* %^BOLD%^mapped_call%^RESET%^
.EI
.SI 5
mixed mapped_call(object ob, string func, mixed * argv)
.EI

.SI 3
* %^BOLD%^parse_frogs%^RESET%^
.EI
.SI 5
int parse_frogs(string str)
.EI


