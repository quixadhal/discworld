.DT
base_master.c
Disk World autodoc help
base_master.c

.SH Description
.SP 5 5

Called to check for rcsforcing of a file.  By default we allow the deputies
to rcsforce things.
.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/error_handler.h, /include/project_management.h and /include/player_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_deputy%^RESET%^
.EI
.SI 5
int add_deputy(string name)
.EI

.SI 3
* %^BOLD%^add_member%^RESET%^
.EI
.SI 5
int add_member(string name)
.EI

.SI 3
* %^BOLD%^add_permission%^RESET%^
.EI
.SI 5
int add_permission(string euid, string path, int mask)
.EI

.SI 3
* %^BOLD%^add_rcsforce_allowed%^RESET%^
.EI
.SI 5
int add_rcsforce_allowed(string creator, string directory)
.EI

.SI 3
* %^BOLD%^assign_people_to_directory%^RESET%^
.EI
.SI 5
int assign_people_to_directory(string dir, string * people)
.EI
.SP 7 5

This method allows a specific person to be assigned to the given
directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to assign someone too
.EP
.SP 9 5
people - the people who are assigned to the directory

.EP

.SI 3
* %^BOLD%^can_rcsforce%^RESET%^
.EI
.SI 5
int can_rcsforce(string file, string person, string locker)
.EI

.SI 3
* %^BOLD%^check_call_tree%^RESET%^
.EI
.SI 5
int check_call_tree(int only_lord)
.EI

.SI 3
* %^BOLD%^check_permission%^RESET%^
.EI
.SI 5
int check_permission(string euid, string * path, int mask)
.EI

.SI 3
* %^BOLD%^nuke_creator_project_cache%^RESET%^
.EI
.SI 5
int nuke_creator_project_cache(string * creators)
.EI

.SI 3
* %^BOLD%^query_access%^RESET%^
.EI
.SI 5
mapping query_access()
.EI

.SI 3
* %^BOLD%^query_assigned_to_directory%^RESET%^
.EI
.SI 5
string * query_assigned_to_directory(string * dir)
.EI
.SP 7 5

Finds the person assigned to the specific directory.  It will climb
up the directory tree looking for who to assign the directory too.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to find assignments in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people assigned to the directory

.EP

.SI 3
* %^BOLD%^query_changelog%^RESET%^
.EI
.SI 5
string query_changelog(string filename)
.EI

.SI 3
* %^BOLD%^query_deputies%^RESET%^
.EI
.SI 5
string * query_deputies()
.EI

.SI 3
* %^BOLD%^query_deputy%^RESET%^
.EI
.SI 5
int query_deputy(string name)
.EI

.SI 3
* %^BOLD%^query_directory_assignments%^RESET%^
.EI
.SI 5
mapping query_directory_assignments()
.EI
.SP 7 5

This method returns all the current assignments from directory to the
people who are looking after them.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the directory assignment mapping

.EP

.SI 3
* %^BOLD%^query_domain%^RESET%^
.EI
.SI 5
string query_domain()
.EI

.SI 3
* %^BOLD%^query_idle_mess%^RESET%^
.EI
.SI 5
string query_idle_mess()
.EI

.SI 3
* %^BOLD%^query_lord%^RESET%^
.EI
.SI 5
string query_lord()
.EI

.SI 3
* %^BOLD%^query_members%^RESET%^
.EI
.SI 5
string * query_members()
.EI

.SI 3
* %^BOLD%^query_prevent_shadow%^RESET%^
.EI
.SI 5
int query_prevent_shadow()
.EI

.SI 3
* %^BOLD%^query_project%^RESET%^
.EI
.SI 5
string query_project(string name)
.EI

.SI 3
* %^BOLD%^query_use_tracker%^RESET%^
.EI
.SI 5
int query_use_tracker()
.EI

.SI 3
* %^BOLD%^remove_deputy%^RESET%^
.EI
.SI 5
int remove_deputy(string name)
.EI

.SI 3
* %^BOLD%^remove_member%^RESET%^
.EI
.SI 5
int remove_member(string name)
.EI

.SI 3
* %^BOLD%^remove_permission%^RESET%^
.EI
.SI 5
int remove_permission(string euid, string path, int mask)
.EI

.SI 3
* %^BOLD%^remove_rcsforce_allowed%^RESET%^
.EI
.SI 5
int remove_rcsforce_allowed(string creator, string directory)
.EI

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI

.SI 3
* %^BOLD%^set_idle_mess%^RESET%^
.EI
.SI 5
int set_idle_mess(string txt)
.EI

.SI 3
* %^BOLD%^set_project%^RESET%^
.EI
.SI 5
int set_project(string name, string pro)
.EI

.SI 3
* %^BOLD%^set_use_tracker%^RESET%^
.EI
.SI 5
void set_use_tracker(int i)
.EI

.SI 3
* %^BOLD%^valid_read%^RESET%^
.EI
.SI 5
int valid_read( string, string euid,  string)
.EI

.SI 3
* %^BOLD%^valid_write%^RESET%^
.EI
.SI 5
int valid_write(string * path, string euid,  string)
.EI


