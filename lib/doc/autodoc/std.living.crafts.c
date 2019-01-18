.DT
crafts.c
Disk World autodoc help
crafts.c

.SH Description
.SP 5 5

This method returns the current bunch of processes for the object.
.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_process%^RESET%^
.EI
.SI 5
int add_process(string name, string ob)
.EI
.SP 7 5

This method adds a process into the current process list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the process to add
.EP
.SP 9 5
ob - the object the process is on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^query_process%^RESET%^
.EI
.SI 5
string query_process(string word)
.EI
.SP 7 5

This method finds the data for the spell of the specific name
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the spell to find data on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the data for the spell

.EP

.SI 3
* %^BOLD%^query_processes%^RESET%^
.EI
.SI 5
mapping query_processes()
.EI

.SI 3
* %^BOLD%^query_processes_nocase%^RESET%^
.EI
.SI 5
mapping query_processes_nocase()
.EI
.SP 7 5

This method returns the list of processes without any case issues.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of processes without any case

.EP

.SI 3
* %^BOLD%^remove_process%^RESET%^
.EI
.SI 5
int remove_process(string name)
.EI
.SP 7 5

This method removes the spell of the given name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the spell to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^reset_processes%^RESET%^
.EI
.SI 5
void reset_processes()
.EI
.SP 7 5

This method resets all of the process data.

.EP


