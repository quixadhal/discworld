.DT
project_management.c
Disk World autodoc help
project_management.c

.SH Description
.SP 5 5

 Project Management System
 @author Drakkos
 @started 19/08/2001


.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/project_management.h, /include/cmds/twiki.h, /include/board.h, /include/permissions.h, /include/mail.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^file_access_cache%^RESET%^
class file_access_cache {
string id;
mapping accesses;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_cache_file%^RESET%^
.EI
.SI 5
int add_cache_file(string id)
.EI

.SI 3
* %^BOLD%^add_creator%^RESET%^
.EI
.SI 5
int add_creator(string id, string creator)
.EI

.SI 3
* %^BOLD%^add_domain%^RESET%^
.EI
.SI 5
int add_domain(string id, string domain)
.EI

.SI 3
* %^BOLD%^add_file%^RESET%^
.EI
.SI 5
int add_file(string id, string file, int access, string name, string task)
.EI

.SI 3
* %^BOLD%^add_note_to_project%^RESET%^
.EI
.SI 5
int add_note_to_project(string id, string note_id, string note, string name)
.EI

.SI 3
* %^BOLD%^add_playtester%^RESET%^
.EI
.SI 5
string * add_playtester(string id, string * pt)
.EI

.SI 3
* %^BOLD%^add_project%^RESET%^
.EI
.SI 5
int add_project(string id, string nname, string lead, string desc, string * dom, string * cre, string add_name, int size, int complexity, string * guilds)
.EI
.SP 7 5

 Add a project to the database.
 @param id the project to add.
 @param nname the name of the project
 @param lead the project leader.
 @param desc the description of the project
 @param dom the domains the project belongs to
 @param cre the creators assigned to the project
 @param add_name the person adding the project.
 @returns the index of the added project, or -1 if the project already 
   exists


.EP

.SI 3
* %^BOLD%^add_task_creator%^RESET%^
.EI
.SI 5
int add_task_creator(string id, string task_id, string creator)
.EI

.SI 3
* %^BOLD%^add_task_to_project%^RESET%^
.EI
.SI 5
int add_task_to_project(string id, string task_id, string name, string aim)
.EI

.SI 3
* %^BOLD%^add_twiki%^RESET%^
.EI
.SI 5
int add_twiki(string id, string twiki)
.EI

.SI 3
* %^BOLD%^assign_playtesters%^RESET%^
.EI
.SI 5
int assign_playtesters(string id, string * pt, string name)
.EI

.SI 3
* %^BOLD%^assign_project_task_creator%^RESET%^
.EI
.SI 5
int assign_project_task_creator(string id, string task, string creator)
.EI

.SI 3
* %^BOLD%^assign_pts_to_project%^RESET%^
.EI
.SI 5
string * assign_pts_to_project(mixed proj)
.EI

.SI 3
* %^BOLD%^catch_status_change%^RESET%^
.EI
.SI 5
void catch_status_change(string id, int status)
.EI

.SI 3
* %^BOLD%^clear%^RESET%^
.EI
.SI 5
void clear()
.EI

.SI 3
* %^BOLD%^clear_file_cache%^RESET%^
.EI
.SI 5
int clear_file_cache(string nfile)
.EI

.SI 3
* %^BOLD%^clear_playtesting_projects%^RESET%^
.EI
.SI 5
int clear_playtesting_projects(string * names)
.EI

.SI 3
* %^BOLD%^data_dump%^RESET%^
.EI
.SI 5
class project * data_dump()
.EI

.SI 3
* %^BOLD%^delete_note%^RESET%^
.EI
.SI 5
int delete_note(string id, string note_id, string name)
.EI
.SP 7 5

 Delete a note from a project.
 @param id the project to search for.
 @param note_id the note to search for
 @param name the name of the person deleting.
 @returns 1.


.EP

.SI 3
* %^BOLD%^delete_project%^RESET%^
.EI
.SI 5
int delete_project(string id, string name)
.EI
.SP 7 5

 Delete a project from the array.
 @param id the note to search for.
 @param name the name of the person deleting.
 @returns 1.


.EP

.SI 3
* %^BOLD%^delete_task%^RESET%^
.EI
.SI 5
int delete_task(string id, string task_id, string name)
.EI
.SP 7 5

 Delete a task from a project.
 @param id the project to search for.
 @param task_id the task to search for
 @param name the name of the person deleting.
 @returns 1.


.EP

.SI 3
* %^BOLD%^do_inform%^RESET%^
.EI
.SI 5
void do_inform()
.EI
.SP 7 5

 Compile the information for the creator inform.


.EP

.SI 3
* %^BOLD%^do_load%^RESET%^
.EI
.SI 5
void do_load()
.EI
.SP 7 5

 Load the file.
 @ignore yes


.EP

.SI 3
* %^BOLD%^do_load_tmp%^RESET%^
.EI
.SI 5
void do_load_tmp()
.EI

.SI 3
* %^BOLD%^do_log%^RESET%^
.EI
.SI 5
void do_log(string text)
.EI
.SP 7 5

 Log text to the project management logfile.
 @ignore yes


.EP

.SI 3
* %^BOLD%^do_mail%^RESET%^
.EI
.SI 5
void do_mail(string * playtesters, mixed proj)
.EI

.SI 3
* %^BOLD%^do_save%^RESET%^
.EI
.SI 5
void do_save()
.EI
.SP 7 5

 Save the file.
 @ignore yes


.EP

.SI 3
* %^BOLD%^do_setup%^RESET%^
.EI
.SI 5
void do_setup()
.EI

.SI 3
* %^BOLD%^filter_by_query%^RESET%^
.EI
.SI 5
class project * filter_by_query(mapping query)
.EI

.SI 3
* %^BOLD%^filter_tasks_by_query%^RESET%^
.EI
.SI 5
class task * filter_tasks_by_query(string id, mapping query)
.EI

.SI 3
* %^BOLD%^find_cache_file%^RESET%^
.EI
.SI 5
int find_cache_file(string file)
.EI
.SP 7 5

 Find the cache entry for a particular file.
 @param id the project to search for.
 @returns the index of the project, or -1 if none is found.


.EP

.SI 3
* %^BOLD%^find_file%^RESET%^
.EI
.SI 5
class file * find_file(mixed i, string nfile, string task)
.EI
.SP 7 5

 Find the occurance of a particular file for a project.
 @param i the project to search for
 @param file the file to search for.
 @returns the index of the task, or -1 if none is found.


.EP

.SI 3
* %^BOLD%^find_note%^RESET%^
.EI
.SI 5
int find_note(mixed i, string id)
.EI
.SP 7 5

 Find the index of a particular note for a project.
 @param i the project to search for
 @param id the note to search for.
 @returns the index of the note, or -1 if none is found.


.EP

.SI 3
* %^BOLD%^find_project%^RESET%^
.EI
.SI 5
int find_project(string id)
.EI
.SP 7 5

 Find the index of a particular project.
 @param id the project to search for.
 @returns the index of the project, or -1 if none is found.


.EP

.SI 3
* %^BOLD%^find_task%^RESET%^
.EI
.SI 5
int find_task(mixed i, string id)
.EI
.SP 7 5

 Find the index of a particular task for a project.
 @param i the project to search for
 @param id the task to search for.
 @returns the index of the task, or -1 if none is found.


.EP

.SI 3
* %^BOLD%^generate_pt_report%^RESET%^
.EI
.SI 5
string generate_pt_report()
.EI

.SI 3
* %^BOLD%^get_note_file%^RESET%^
.EI
.SI 5
class note get_note_file(string id, string t)
.EI

.SI 3
* %^BOLD%^get_playtesting_projects%^RESET%^
.EI
.SI 5
class project * get_playtesting_projects()
.EI

.SI 3
* %^BOLD%^get_project_file%^RESET%^
.EI
.SI 5
class project get_project_file(string id)
.EI

.SI 3
* %^BOLD%^get_task_file%^RESET%^
.EI
.SI 5
class task get_task_file(string id, string t)
.EI

.SI 3
* %^BOLD%^insert_into_cache%^RESET%^
.EI
.SI 5
int insert_into_cache(mixed temp)
.EI

.SI 3
* %^BOLD%^insert_into_notes%^RESET%^
.EI
.SI 5
int insert_into_notes(int i, mixed temp)
.EI
.SP 7 5

 Insert an element into the notes array.  It will do this 
 alphabetically to ensure that the binary search algorithim
 can locate projects.
 @param i the index of the project to insert into.
 @param temp the element to insert
 @returns the index of the inserted element.

.EP

.SI 3
* %^BOLD%^insert_into_projects%^RESET%^
.EI
.SI 5
int insert_into_projects(mixed temp)
.EI
.SP 7 5

 Insert an element into the projects array.  It will do this 
 alphabetically to ensure that the binary search algorithim
 can locate projects.
 @param temp the element to insert
 @returns the index of the inserted element.

.EP

.SI 3
* %^BOLD%^insert_into_tasks%^RESET%^
.EI
.SI 5
int insert_into_tasks(int i, mixed temp)
.EI
.SP 7 5

 Insert an element into the tasks array.  It will do this 
 alphabetically to ensure that the binary search algorithim
 can locate projects.
 @param i the index of the project to insert into.
 @param temp the element to insert
 @returns the index of the inserted element.

.EP

.SI 3
* %^BOLD%^is_creator_on_project%^RESET%^
.EI
.SI 5
int is_creator_on_project(string name, string id)
.EI

.SI 3
* %^BOLD%^make_post%^RESET%^
.EI
.SI 5
int make_post()
.EI

.SI 3
* %^BOLD%^next_rotation_time%^RESET%^
.EI
.SI 5
int next_rotation_time(mixed proj)
.EI

.SI 3
* %^BOLD%^nuke_creator_project_cache%^RESET%^
.EI
.SI 5
int nuke_creator_project_cache(string * creators)
.EI

.SI 3
* %^BOLD%^parse_details%^RESET%^
.EI
.SI 5
string parse_details(class project p)
.EI
.SP 7 5

 Return a brief summary of the pertinent details of a particular project.
 @param p the project to summarise.
 @returns the string summary.


.EP

.SI 3
* %^BOLD%^project_info%^RESET%^
.EI
.SI 5
string project_info(string id)
.EI

.SI 3
* %^BOLD%^project_note_data%^RESET%^
.EI
.SI 5
class note * project_note_data(string id)
.EI

.SI 3
* %^BOLD%^project_task_data%^RESET%^
.EI
.SI 5
class task * project_task_data(string id)
.EI

.SI 3
* %^BOLD%^query_all_projects_info%^RESET%^
.EI
.SI 5
string query_all_projects_info()
.EI

.SI 3
* %^BOLD%^query_description%^RESET%^
.EI
.SI 5
string query_description(string id)
.EI

.SI 3
* %^BOLD%^query_file_permission%^RESET%^
.EI
.SI 5
int query_file_permission(string name, string nfile, int access)
.EI

.SI 3
* %^BOLD%^query_files%^RESET%^
.EI
.SI 5
class file * query_files(string id)
.EI

.SI 3
* %^BOLD%^query_guild_specific%^RESET%^
.EI
.SI 5
string * query_guild_specific(mixed proj)
.EI

.SI 3
* %^BOLD%^query_last_summary%^RESET%^
.EI
.SI 5
int query_last_summary()
.EI

.SI 3
* %^BOLD%^query_number_of_pts%^RESET%^
.EI
.SI 5
int query_number_of_pts(mixed proj)
.EI

.SI 3
* %^BOLD%^query_number_rotations%^RESET%^
.EI
.SI 5
int query_number_rotations(mixed proj)
.EI

.SI 3
* %^BOLD%^query_permission%^RESET%^
.EI
.SI 5
int query_permission(string project, string name, string nfile, int access)
.EI

.SI 3
* %^BOLD%^query_playtesters%^RESET%^
.EI
.SI 5
string * query_playtesters(mixed proj)
.EI

.SI 3
* %^BOLD%^query_project_completeness%^RESET%^
.EI
.SI 5
int query_project_completeness(string id)
.EI

.SI 3
* %^BOLD%^query_project_domains%^RESET%^
.EI
.SI 5
string * query_project_domains(string id)
.EI

.SI 3
* %^BOLD%^query_project_leader%^RESET%^
.EI
.SI 5
string query_project_leader(string id)
.EI

.SI 3
* %^BOLD%^query_project_lock%^RESET%^
.EI
.SI 5
int query_project_lock(string project)
.EI

.SI 3
* %^BOLD%^query_project_name%^RESET%^
.EI
.SI 5
string query_project_name(string id)
.EI

.SI 3
* %^BOLD%^query_project_notes%^RESET%^
.EI
.SI 5
string * query_project_notes(string id)
.EI

.SI 3
* %^BOLD%^query_project_subprojects%^RESET%^
.EI
.SI 5
string * query_project_subprojects(string id)
.EI

.SI 3
* %^BOLD%^query_project_tasks%^RESET%^
.EI
.SI 5
string * query_project_tasks(string id)
.EI

.SI 3
* %^BOLD%^query_projects%^RESET%^
.EI
.SI 5
string * query_projects()
.EI

.SI 3
* %^BOLD%^query_projects_for_creator%^RESET%^
.EI
.SI 5
string * query_projects_for_creator(string c)
.EI

.SI 3
* %^BOLD%^query_projects_for_creator_in_domain%^RESET%^
.EI
.SI 5
string * query_projects_for_creator_in_domain(string creator, string domain)
.EI

.SI 3
* %^BOLD%^query_pt_cache%^RESET%^
.EI
.SI 5
string * query_pt_cache()
.EI

.SI 3
* %^BOLD%^query_pt_projects%^RESET%^
.EI
.SI 5
string * query_pt_projects(string name)
.EI

.SI 3
* %^BOLD%^query_rotation%^RESET%^
.EI
.SI 5
int query_rotation(mixed proj)
.EI

.SI 3
* %^BOLD%^query_size%^RESET%^
.EI
.SI 5
int query_size(mixed proj)
.EI

.SI 3
* %^BOLD%^query_status%^RESET%^
.EI
.SI 5
int query_status(string id)
.EI

.SI 3
* %^BOLD%^query_summary_mapping%^RESET%^
.EI
.SI 5
mapping query_summary_mapping()
.EI

.SI 3
* %^BOLD%^query_summary_page%^RESET%^
.EI
.SI 5
string query_summary_page()
.EI
.SP 7 5

 Return the details of projects that have been touched since the
 last summary.
 @returns the recent project changes.


.EP

.SI 3
* %^BOLD%^query_task_aims%^RESET%^
.EI
.SI 5
string query_task_aims(string id, string task_id)
.EI

.SI 3
* %^BOLD%^query_task_comments%^RESET%^
.EI
.SI 5
string query_task_comments(string id, string task_id)
.EI

.SI 3
* %^BOLD%^query_task_competion%^RESET%^
.EI
.SI 5
int query_task_competion(string id, string task_id)
.EI

.SI 3
* %^BOLD%^query_task_completion_date%^RESET%^
.EI
.SI 5
int query_task_completion_date(string id, string task_id)
.EI

.SI 3
* %^BOLD%^query_task_creators%^RESET%^
.EI
.SI 5
string * query_task_creators(string id, string task_id)
.EI

.SI 3
* %^BOLD%^query_task_name%^RESET%^
.EI
.SI 5
string query_task_name(string id, string task_id)
.EI

.SI 3
* %^BOLD%^query_time_to_rotate%^RESET%^
.EI
.SI 5
int query_time_to_rotate(mixed proj)
.EI

.SI 3
* %^BOLD%^query_touched%^RESET%^
.EI
.SI 5
string query_touched(string id)
.EI
.SP 7 5

 Return a formatted string saying when the project was last touched.
 @param id the project ID.
 @returns the last touched string.


.EP

.SI 3
* %^BOLD%^query_twiki%^RESET%^
.EI
.SI 5
string * query_twiki(string id)
.EI

.SI 3
* %^BOLD%^remove_file%^RESET%^
.EI
.SI 5
int remove_file(string id, string file, string task, string name)
.EI

.SI 3
* %^BOLD%^remove_playtester%^RESET%^
.EI
.SI 5
int remove_playtester(string id, string * pt)
.EI

.SI 3
* %^BOLD%^reset_files%^RESET%^
.EI
.SI 5
int reset_files(string id)
.EI

.SI 3
* %^BOLD%^rotate_project%^RESET%^
.EI
.SI 5
void rotate_project(mixed tmp)
.EI

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI
.SP 7 5

 Call out the do_save function.  If lots of projects are being touched 
` within a short time scale, this should reduce the hard-drive grinding.
 @ignore yes


.EP

.SI 3
* %^BOLD%^set_completion_date%^RESET%^
.EI
.SI 5
int set_completion_date(string id, int date)
.EI

.SI 3
* %^BOLD%^set_complexity%^RESET%^
.EI
.SI 5
int set_complexity(mixed proj, int com)
.EI

.SI 3
* %^BOLD%^set_description%^RESET%^
.EI
.SI 5
int set_description(string id, string desc)
.EI

.SI 3
* %^BOLD%^set_guild_specific%^RESET%^
.EI
.SI 5
int set_guild_specific(mixed proj, string * guilds)
.EI

.SI 3
* %^BOLD%^set_note_file%^RESET%^
.EI
.SI 5
void set_note_file(string id, string t, class note tmp, string name)
.EI

.SI 3
* %^BOLD%^set_project_completeness%^RESET%^
.EI
.SI 5
int set_project_completeness(string id, int percent)
.EI

.SI 3
* %^BOLD%^set_project_file%^RESET%^
.EI
.SI 5
void set_project_file(string id, class project tmp, string name)
.EI

.SI 3
* %^BOLD%^set_project_leader%^RESET%^
.EI
.SI 5
int set_project_leader(string id, string leader)
.EI

.SI 3
* %^BOLD%^set_project_lock%^RESET%^
.EI
.SI 5
void set_project_lock(string project)
.EI

.SI 3
* %^BOLD%^set_project_name%^RESET%^
.EI
.SI 5
int set_project_name(string id, string name)
.EI

.SI 3
* %^BOLD%^set_rotations%^RESET%^
.EI
.SI 5
void set_rotations(string proj, int rot)
.EI

.SI 3
* %^BOLD%^set_size%^RESET%^
.EI
.SI 5
int set_size(mixed proj, int siz)
.EI

.SI 3
* %^BOLD%^set_status%^RESET%^
.EI
.SI 5
int set_status(string id, int stat)
.EI

.SI 3
* %^BOLD%^set_status_change%^RESET%^
.EI
.SI 5
int set_status_change(string id, int time)
.EI

.SI 3
* %^BOLD%^set_task_aims%^RESET%^
.EI
.SI 5
int set_task_aims(string id, string task_id, string aims)
.EI

.SI 3
* %^BOLD%^set_task_comments%^RESET%^
.EI
.SI 5
int set_task_comments(string id, string task_id, string comments)
.EI

.SI 3
* %^BOLD%^set_task_completion%^RESET%^
.EI
.SI 5
int set_task_completion(string id, string task_id, int percent)
.EI

.SI 3
* %^BOLD%^set_task_completion_date%^RESET%^
.EI
.SI 5
int set_task_completion_date(string id, string task_id, int date)
.EI

.SI 3
* %^BOLD%^set_task_file%^RESET%^
.EI
.SI 5
void set_task_file(string id, string t, class task tmp, string name)
.EI

.SI 3
* %^BOLD%^set_task_name%^RESET%^
.EI
.SI 5
int set_task_name(string id, string task_id, string name)
.EI

.SI 3
* %^BOLD%^task_info%^RESET%^
.EI
.SI 5
string task_info(string id, string task_id)
.EI

.SI 3
* %^BOLD%^test_num%^RESET%^
.EI
.SI 5
void test_num()
.EI

.SI 3
* %^BOLD%^touch_project%^RESET%^
.EI
.SI 5
void touch_project(string id, string reason, string name)
.EI
.SP 7 5

 Touch a project.  We use this project to set when a project
 was last touched, by whom and what they did.  We also save
 the data file at this point.
 @param id the project ID
 @param reason the way the project was touched
 @param name the name of the person touching the project
 

.EP

.SI 3
* %^BOLD%^update_project_task_completeness%^RESET%^
.EI
.SI 5
int update_project_task_completeness(string id, string task, int percent)
.EI

.SI 3
* %^BOLD%^valid_operation%^RESET%^
.EI
.SI 5
varargs int valid_operation(string name, string action, string project, string * dom)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^find_index%^RESET%^
.EI
.SI 5
int find_index(string id, mixed array)
.EI
.SP 7 5

 Find the occurance of a particular ID in a particular array.  This
 is done via a binary search of the array.
 @param id the ID to match on
 @array array the array to search through.
 @returns the index of the matched element, or -1 if none is found.
 @ignore yes

.EP


