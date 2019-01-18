.DT
applications.h
Disk World autodoc help
applications.h

.SH Description
.SP 5 5

 Applications 
 @author Drakkos
 @started 31/01/2003


.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^application%^RESET%^
class application {
string type;
string name;
string text;
mapping vouches;
int time;
string domain;
}

.EI

.SI 3
* %^BOLD%^application_type%^RESET%^
class application_type {
string type;
string board;
string * questions;
int vouches_required;
int gl_requirement;
int qp_requirement;
int age_requirement;
int timeout;
string intro_text;
string responsibility;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^APPLICATIONS_HANDLER%^RESET%^
.EI

.SI 3
* %^BOLD%^APPLY_COMMAND%^RESET%^
.EI

.SI 3
* %^BOLD%^APPLY_SAVE_FILE%^RESET%^
.EI

.SI 3
* %^BOLD%^DAY%^RESET%^
.EI

.SI 3
* %^BOLD%^HOUR%^RESET%^
.EI

.SI 3
* %^BOLD%^MINUTE%^RESET%^
.EI

