.DT
project_management.h
Disk World autodoc help
project_management.h

.SH Classes

.SI 3
* %^BOLD%^file%^RESET%^
class file {
string file;
int access;
string task;
}

.EI

.SI 3
* %^BOLD%^note%^RESET%^
class note {
string id;
int date_added;
string added_by;
string note;
}

.EI

.SI 3
* %^BOLD%^project%^RESET%^
class project {
string id;
string name;
string leader;
string added_by;
string description;
string pt_notes;
string what_touched;
string touched_by;
int date_added;
int date_completed;
int status;
int percent_complete;
int estimated_completion;
int last_touched;
int last_status_change;
string * twiki;
string * playtesters;
string * domains;
string * creators;
string * sub_projects;
class file * files;
class task * tasks;
class note * notes;
int last_rotation;
int size;
int complexity;
int current_rotation;
string * last_pt_cohort;
string * all_pts;
string * guild_specific;
}

.EI

.SI 3
* %^BOLD%^project_tmp%^RESET%^
class project_tmp {
string id;
string name;
string leader;
string added_by;
string description;
string pt_notes;
string * domains;
string * creators;
string * sub_projects;
class file * files;
string * twiki;
string * playtesters;
string what_touched;
string touched_by;
int date_added;
int date_completed;
int status;
int percent_complete;
int estimated_completion;
int last_touched;
int last_status_change;
class task * tasks;
class note * notes;
}

.EI

.SI 3
* %^BOLD%^task%^RESET%^
class task {
string id;
string task_name;
string comments;
string * assigned_to;
string added_by;
string aims;
int percent_complete;
int completion_date;
int date_added;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^CALLBACK%^RESET%^
.EI

.SI 3
* %^BOLD%^COMPLEXITY%^RESET%^
.EI

.SI 3
* %^BOLD%^DAY%^RESET%^
.EI

.SI 3
* %^BOLD%^FILE%^RESET%^
.EI

.SI 3
* %^BOLD%^HOUR%^RESET%^
.EI

.SI 3
* %^BOLD%^MINUTE%^RESET%^
.EI

.SI 3
* %^BOLD%^NOTE%^RESET%^
.EI

.SI 3
* %^BOLD%^PLAYTESTING%^RESET%^
.EI

.SI 3
* %^BOLD%^PROJECT%^RESET%^
.EI

.SI 3
* %^BOLD%^PROJECT_HANDLER%^RESET%^
.EI

.SI 3
* %^BOLD%^PROJECT_LOCK%^RESET%^
.EI

.SI 3
* %^BOLD%^PROJECT_LOG%^RESET%^
.EI

.SI 3
* %^BOLD%^PROJECT_SAVE%^RESET%^
.EI

.SI 3
* %^BOLD%^SIZE%^RESET%^
.EI

.SI 3
* %^BOLD%^STATUS%^RESET%^
.EI

.SI 3
* %^BOLD%^TASK%^RESET%^
.EI

.SI 3
* %^BOLD%^TIME_BETWEEN_POSTINGS%^RESET%^
.EI

.SI 3
* %^BOLD%^TMP_SAVE%^RESET%^
.EI

.SI 3
* %^BOLD%^TWIKI_BASE%^RESET%^
.EI

.SI 3
* %^BOLD%^WEEK%^RESET%^
.EI

.SI 3
* %^BOLD%^WHATWHAT%^RESET%^
.EI

