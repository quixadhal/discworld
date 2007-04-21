 /**
 *  Project Management System
 *  @author Drakkos
 *  @started 19/08/2001
 *
 */

#include "project_management.h"
#include <playtesters.h>
#include <permissions.h>
#include <board.h>
#include <mail.h>
#include <cmds/twiki.h>

int query_project_completeness(string);
string* query_project_tasks(string);
int valid_operation (string name, string action, string project);
int find_project (string);
void do_save();
int nuke_creator_project_cache(string*); 
string* query_projects_for_creator (string);
int clear_file_cache (string nfile);
void do_mail (string*, string);
int clear_playtesting_projects (string*);
class project* filter_by_query (mapping);
void catch_status_change (string, int);
 
class file_access_cache {
  string id;
  mapping accesses;
}
 
string *pt_post_cache = ({ });
 
class project *projects = ({ });
class project_tmp *tmp_projects = ({ });
int touched;

int last_summary;
mapping summary;
mapping num_assignments;
nosave mapping inform = ([ ]); 
nosave mapping creator_project_cache = ([ ]);
nosave mapping pt_assignment_cache = ([ ]);
nosave class file_access_cache *cache = ({ });
nosave string *excluded = ({
  "bfg",
  "dionysus",
  "melian",
});    

mapping project_locks = ([ ]);
  
/**
 *  Call out the do_save function.  If lots of projects are being touched 
 *` within a short time scale, this should reduce the hard-drive grinding.
 *  @ignore yes
 *
 */
   
void save_file() {
  if (find_call_out ("do_save") == -1) {
    call_out ("do_save", 10);
  }
}

/**
 *  Log text to the project management logfile.
 *  @ignore yes
 *
 */
 
void do_log (string text) {
  log_file (PROJECT_LOG, text);
}

/**
 *  Save the file.
 *  @ignore yes
 *
 */
 
void do_save() {
  seteuid ("Root");
  touched = 1;
  unguarded ((: save_object, PROJECT_SAVE :));
//  unguarded ((: save_object, TMP_SAVE:));

  touched = 0;
}

/**
 *  Load the file.
 *  @ignore yes
 *
 */
 
void do_load() {
  unguarded ((: restore_object, PROJECT_SAVE :));

}

 
/**
 *  The create method, natch.
 *  @ignore yes
 *
 */  
 
void create() {
  string *creators = ({ });
  
  do_load();
  
  if (!last_summary) {
    last_summary = time();
  }
  
  if (!summary) {
    summary = ([ ]);
  }
  
  if (!tmp_projects) {
    tmp_projects = ({ });
  }
  
  if (!creator_project_cache) {
    creator_project_cache = ([ ]);
  }
  
  if (!cache) {
    cache = ({ });
  }
  
  if (!pt_post_cache) {
    pt_post_cache = ({ });
  }
  
  if (!num_assignments) {
    num_assignments = ([ ]);
  }
  
    
  foreach (class project p in projects) {
    creators += p->creators;
  }
  
  creators = uniq_array (creators);
  
  foreach (string c in creators) {
    query_projects_for_creator (c);    
  }
  
  
}

/**
 *  Touch a project.  We use this project to set when a project
 *  was last touched, by whom and what they did.  We also save
 *  the data file at this point.
 *  @param id the project ID
 *  @param reason the way the project was touched
 *  @param name the name of the person touching the project
 *  
 */
 
void touch_project (string id, string reason, string name) {
  int i = find_project (id);
  object ob;
  
  if (i == -1) {
    return;
  }
  
  projects[i]->last_touched = time();
  projects[i]->what_touched = reason;
  projects[i]->touched_by = name;
  save_file();
  
  if (find_call_out ("do_inform") == -1) {
    call_out ("do_inform", 120);
  }
                             
  if (undefinedp (summary[id])) {
    summary[id] = ({ name });
  }
  else {
    summary[id] += ({ name });
  }

  if (undefinedp (inform[id])) {
    inform[id] = ({ name });
  }
  else {
    inform[id] += ({ name });
  }
  
  foreach (string d in projects[i]->domains) {
    ob = load_object ("/d/" + d + "/master");    
    if (ob) {
      ob->nuke_creator_project_cache (projects[i]->creators);
    }
    nuke_creator_project_cache (projects[i]->creators);
  }
      
                                     
  do_log (sprintf ("%s:  Project %s touched by %s:  %s\n", 
    ctime (time()), id, name, reason));
}

/**
 *  Compile the information for the creator inform.
 *
 */
 
void do_inform() {
  int i;
  
  foreach (string id, string *names in inform) {    
    i = find_project (id);
    
    user_event ("inform", sprintf ("%s: Project %s touched by %s", 
      query_multiple_short(map (projects[i]->domains, (: capitalize ($1) :))), 
      id, query_multiple_short (map (uniq_array (names), 
      (: capitalize ($1) :)))), "message", this_player());
  }
  
  inform = ([ ]);
}



string* query_projects_for_creator (string c) {
  if (!creator_project_cache[c]) {
    creator_project_cache[c] = map (filter_by_query ((["creators": c])),
      (: $1->id :));
      
//    map (filter (projects, (: member_array ($(c), 
//      $1->creators) != -1 :)), (: $1->id :));
  }
  
  return creator_project_cache[c];
}


string* query_projects_for_creator_in_domain (string creator, string domain) {
  return sort_array (map (filter_by_query ((["creators": creator, "domains" : domain])),
      (: $1->name :)), 1);
}


/**
 *  Return a formatted string saying when the project was last touched.
 *  @param id the project ID.
 *  @returns the last touched string.
 *
 */
 
string query_touched (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (!projects[i]->last_touched) {
    return "This project has not been touched since it was added.";
  }
  
  return sprintf ("%s last touched project %s on %s:  %s.",
    (projects[i]->touched_by ? capitalize (projects[i]->touched_by) :
    "Someone"), id, 
    ctime (projects[i]->last_touched), projects[i]->what_touched);    
}


mapping query_summary_mapping() {
  return summary;
}

int query_last_summary() {
  return last_summary;
}


/**
 *  Return the details of projects that have been touched since the
 *  last summary.
 *  @returns the recent project changes.
 *
 */
 
string query_summary_page() {
  string ret = "Project changes since " + ctime (last_summary) + "\n\n";
  
  foreach (string proj, string *names in summary) {
    ret += sprintf ("%-30s touched by %s\n", 
      proj, query_multiple_short (uniq_array (names)));
  }

  
  return ret;
}

/**
 *  Insert an element into the projects array.  It will do this 
 *  alphabetically to ensure that the binary search algorithim
 *  can locate projects.
 *  @param temp the element to insert
 *  @returns the index of the inserted element.
 */
 
int insert_into_projects (mixed temp) {
  int index = 0;

  if (!sizeof (projects)) {
    projects = ({ temp });
    return 0;
  }
    
  while (index < sizeof (projects) && temp->id > projects[index]->id) {       
    index++;
  } 
          
  if (index == sizeof (projects)) {
    projects = projects + ({ temp });
  }
  else if (index == 0) {
    projects = ({ temp }) + projects;
  }
  else {
    projects = projects[0..index-1] + ({ temp }) + 
      projects[index..sizeof(projects)];
  }

  
  return index;
}

int insert_into_cache (mixed temp) {
  int index = 0;

  if (!sizeof (cache)) {
    cache = ({ temp });
    return 0;
  }
    
  while (index < sizeof (cache) && temp->id > cache[index]->id) {       
    index++;
  } 
          
  if (index == sizeof (cache)) {
    cache = cache + ({ temp });
  }
  else if (index == 0) {
    cache = ({ temp }) + cache ;
  }
  else {
    cache = cache[0..index-1] + ({ temp }) + 
      cache[index..sizeof(cache)];
  }

  
  return index;
}

/**
 *  Insert an element into the tasks array.  It will do this 
 *  alphabetically to ensure that the binary search algorithim
 *  can locate projects.
 *  @param i the index of the project to insert into.
 *  @param temp the element to insert
 *  @returns the index of the inserted element.
 */
 

int insert_into_tasks (int i, mixed temp) {
  int index = 0;

  if (!sizeof (projects[i]->tasks)) {
    projects[i]->tasks = ({ temp });
    return 0;
  }
    
  while (index < sizeof (projects[i]->tasks) && 
    temp->id > projects[i]->tasks[index]->id) {       
      index++;
  } 
          
  if (index == sizeof (projects[i]->tasks)) {
    projects[i]->tasks = projects[i]->tasks + ({ temp });
  }
  else if (index == 0) {
    projects[i]->tasks = ({ temp }) + projects[i]->tasks;
  }
  else {
    projects[i]->tasks = projects[i]->tasks[0..index-1] + ({ temp }) 
      + projects[i]->tasks[index..sizeof(projects[i]->tasks)];
  }
 
  return index;
}


/**
 *  Insert an element into the notes array.  It will do this 
 *  alphabetically to ensure that the binary search algorithim
 *  can locate projects.
 *  @param i the index of the project to insert into.
 *  @param temp the element to insert
 *  @returns the index of the inserted element.
 */
 
int insert_into_notes (int i, mixed temp) {
  int index = 0;

  if (!sizeof (projects[i]->notes)) {
    projects[i]->notes = ({ temp });
    return 0;
  }
    
  while (index < sizeof (projects[i]->notes) && 
    temp->id > projects[i]->notes[index]->id) {       
      index++;
  } 
          
  if (index == sizeof (projects[i]->notes)) {
    projects[i]->notes = projects[i]->notes + ({ temp });
  }
  else if (index == 0) {
    projects[i]->notes = ({ temp }) + projects[i]->notes;
  }
  else {
    projects[i]->notes = projects[i]->notes[0..index-1] + ({ temp }) 
      + projects[i]->notes[index..sizeof(projects[i]->notes)];
  }
 
  return index;
}


/**
 *  Find the occurance of a particular ID in a particular array.  This
 *  is done via a binary search of the array.
 *  @param id the ID to match on
 *  @array array the array to search through.
 *  @returns the index of the matched element, or -1 if none is found.
 *  @ignore yes
 */
 

protected int find_index (string id, mixed array) {
  int mid, right, left;
  
  if (!sizeof (array)) {
    return -1;
  }
  
  right = sizeof (array);
  mid = (left + right) / 2;
  left = 0;
  
  do {
    
    if (array[mid]->id == id) {
      return mid;
    }
    
    if (id > array[mid]->id) {  
      left = mid + 1;
      mid = (left + right) / 2;
    }
    else if (id < array[mid]->id) {
      right = mid;
      mid = (left + right) / 2;

    }
        
  } while (left < right);
  
  return -1;
} 


/**
 *  Return a brief summary of the pertinent details of a particular project.
 *  @param p the project to summarise.
 *  @returns the string summary.
 *
 */
 
string parse_details(class project p) {
  string ret = sprintf ("$I$5=The %s (%s) project belongs to the %s %s and is "
    "in %s, with assigned creators of: %s.\n", 
    p->id, p->name, query_multiple_short (p->domains), 
    (sizeof (p->domains) == 1 ? "domain" : "domains"), STATUS[p->status], 
    query_multiple_short (p->creators));      

//  if (file_name(previous_object())[0..3] == "/www" ) {
    ret = replace (ret, ({"$I$5=", "<p>"}));
//  }

  return ret;
}      

/**
 *  Find the index of a particular project.
 *  @param id the project to search for.
 *  @returns the index of the project, or -1 if none is found.
 *
 */
 
int find_project (string id) {
  int i = find_index (id, projects);
  
  return i;
}

/**
 *  Find the index of a particular note for a project.
 *  @param i the project to search for
 *  @param id the note to search for.
 *  @returns the index of the note, or -1 if none is found.
 *
 */
 
int find_note(mixed i, string id) {
  int j;
  
  if (!intp(i)) {
    i = find_project (i);
  }
  
  j = find_index (id, projects[i]->notes);
  
  return j;
}

/**
 *  Find the index of a particular task for a project.
 *  @param i the project to search for
 *  @param id the task to search for.
 *  @returns the index of the task, or -1 if none is found.
 *
 */
 
int find_task(mixed i, string id) {
  int j;
  
  if (!intp(i)) {
    i = find_project (i);
  }
  
  j = find_index (id, projects[i]->tasks);
  
  return j;
}

/**
 *  Find the occurance of a particular file for a project.
 *  @param i the project to search for
 *  @param file the file to search for.
 *  @returns the index of the task, or -1 if none is found.
 *
 */
 
class file* find_file(mixed i, string nfile, string task) {
  int j;
  class file *tmp = ({ });
  
  if (!intp(i)) {
    i = find_project (i);
  }
    
  if (!sizeof (projects[i]->files)) {
    return tmp;
  }
  
  
  for (j=0; j < sizeof (projects[i]->files); j++) {
    if (projects[i]->files[j]->file == nfile) {
      if (sizeof (task)) {
        if (task == "leader" || projects[i]->files[j]->task == task) {
          tmp += ({ projects[i]->files[j] });
        }
      }
                
      else if (!sizeof (projects[i]->files[j]->task)) {
          tmp += ({ projects[i]->files[j] });
      }
    }
  }
  
  
  return tmp;
}

/**
 *  Find the cache entry for a particular file.
 *  @param id the project to search for.
 *  @returns the index of the project, or -1 if none is found.
 *
 */
 
int find_cache_file (string file) {
  int i = find_index (file, cache);
  
  return i;
}


/**
 *  Delete a project from the array.
 *  @param id the note to search for.
 *  @param name the name of the person deleting.
 *  @returns 1.
 *
 */
 
int delete_project (string id, string name) {
  int i = find_index (id, projects);
  
  if (i == -1) {
    return 0;
  }
        
  projects -= ({ projects[i] });
  do_log (sprintf ("%s:  %s deleted project %s.\n", 
    ctime(time()), name, id));
  save_file();
  return 1;
}

/**
 *  Delete a task from a project.
 *  @param id the project to search for.
 *  @param task_id the task to search for
 *  @param name the name of the person deleting.
 *  @returns 1.
 *
 */
 
int delete_task (string id, string task_id, string name) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
      
  projects[i]->tasks -= ({ projects[i]->tasks[j] });
  touch_project (id, "Deleted task " + task_id, name);
  return 1;
}

/**
 *  Delete a note from a project.
 *  @param id the project to search for.
 *  @param note_id the note to search for
 *  @param name the name of the person deleting.
 *  @returns 1.
 *
 */
 
int delete_note (string id, string note_id, string name) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_note (i, note_id);
  
  if (j == -1) {
    return 0;
  }
  
      
  projects[i]->notes -= ({ projects[i]->notes[j] });
  touch_project (id, "Deleted note " + note_id, name);
  
  return 1;
}

/**
 *  Add a project to the database.
 *  @param id the project to add.
 *  @param nname the name of the project
 *  @param lead the project leader.
 *  @param desc the description of the project
 *  @param dom the domains the project belongs to
 *  @param cre the creators assigned to the project
 *  @param add_name the person adding the project.
 *  @returns the index of the added project, or -1 if the project already 
 *    exists
 *
 */

int add_project (string id, string nname, string lead, string desc, 
  string *dom, string *cre, string add_name, int size, int complexity,
  string *guilds) {  
  int i;
  
  class project tmp;

  if (find_project (id) != -1) {
    return -1;
  }
    
  tmp = new (class project, id: id, name: nname, leader: lead,
    date_added: time(), added_by: add_name, 
    date_completed: 0, status: 0, estimated_completion: 0, 
    description: desc, domains: dom, creators: (sizeof (cre) ? cre :
    ({})), playtesters: ({ }), tasks: ({ }),
    files: ({ }), twiki: ({ }), percent_complete: 0, sub_projects: ({ }),
    notes: ({ }), size: size, complexity: complexity, guild_specific : 
    guilds);
    
  i = insert_into_projects (tmp);

  touch_project (id, "Project added", add_name);

  return i;
}
  
int add_cache_file (string id) {  
  int i;
  
  class file_access_cache tmp;

  if (find_cache_file (id) != -1) {
    return -1;
  }
    
  tmp = new (class file_access_cache, id: id, accesses: ([ ]));
    
  i = insert_into_cache (tmp);

  return i;
}  
 
int set_completion_date (string id, int date) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->estimated_completion = date;
}
   
int add_creator (string id, string creator) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (!sizeof (projects[i]->creators)) {
    projects[i]->creators = ({ creator });
  }
  else if (member_array (creator, projects[i]->creators) == -1) {
    projects[i]->creators += ({ creator });
  }

  return 1;  
}   

int add_twiki (string id, string twiki) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (!sizeof (projects[i]->twiki)) {
    projects[i]->twiki = ({ twiki });
  }
  else {
    projects[i]->twiki += ({ twiki });
  }

  return 1;  
}   

string* query_twiki (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
    
  return copy (projects[i]->twiki);
  
}  


class file* query_files (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
    
  return copy (projects[i]->files);
  
}  


int add_file (string id, string file, int access, string name, string task) {
  int i = find_project (id);
  int k;
  mixed ob;
  class file tmp;
  class file *files = ({ });
  
  
  if (i == -1) {
    return 0;
  }
  
  if (access == GRANT_MASK || access == LOCK_MASK) {
    ob = find_player (name);
    
    if (!ob) {
      return 0;
    }
  }
  
  if (!ob) {
    ob = name;
  }
  
  
  k = master()->valid_grant (ob, file, access);
  
  if (!k) {
    log_file ("CHEAT", ctime(time()) + ": " + name + " tried to add "
      "level " + access + " access to file " + file + "\n");
    return 0;
  }
  
  tmp = new (class file, file: file, access: access, task: task);

   
  if (!sizeof (projects[i]->files)) {
    projects[i]->files = ({ tmp });
  }
  else {
    files = find_file (i, file, task);
    
    foreach (class file f in files) {
      if (f->access == access && f->task == task) {
        return 0;
      }
    }
    
    projects[i]->files += ({ tmp });  
  }
  
  clear_file_cache (file);
  
  touch_project (id, "Added a file permission", name);
  
  return 1;  
}   

int remove_file (string id, string file, string task, string name) {
  int i = find_project (id);
  class file *tmp = ({ });
    
  if (i == -1) {
    return 0;
  }  
  
  tmp = find_file (i, file, task);
  
  if (!sizeof (tmp)) {
    return 0;
  }
  
  foreach (class file f in tmp) {
    if (f->task == task) {
      projects[i]->files -= ({ f });
      break;
    }
    
  }
  
  clear_file_cache (file);
  touch_project (id, "Removed a file permission", name);

  return 1;
}

int reset_files (string id) {
  int i = find_project (id);
  projects[i]->files = ({ });
}

int add_domain (string id, string domain) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (!sizeof (projects[i]->domains)) {
    projects[i]->domains = ({ domain });
  }
  else {
    projects[i]->domains += ({ domain });
  }

  return 1;  
}   

int set_project_leader (string id, string leader) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->leader= leader;
}

string query_project_leader (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->leader;
}

string* query_project_subprojects (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->sub_projects;
}


string* query_projects() {
  return map (projects, (: $1->id :));
}

int set_status (string id, int stat) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (stat >= sizeof (STATUS)) {
    return 0;
  }
  
  projects[i]->status = stat;
  catch_status_change (id, stat);
}  


int query_status (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->status;
}


string query_all_projects_info() {
  string ret = "";
  
  foreach (class project p in projects) {
    ret += parse_details (p);    
  }
  
  
  return ret;          
}

string* query_project_tasks (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return map (projects[i]->tasks, (: $1->id :));
}

string* query_project_notes (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (!sizeof (projects[i]->notes)) {
    return 0;
  }
  
  return map (projects[i]->notes, (: $1->id :));
}

string* query_project_domains (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->domains;
}


int set_description (string id, string desc) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->description = desc;
  return 1;
}


string query_description (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->description;
}

int set_status_change(string id, int time) {
  int i = find_project (id);
  
  if (time) {
    projects[i]->last_status_change = time;
  }
  
}

string project_info (string id) {
  int i = find_project (id);
  string ret = "";
    
  if (i == -1) {
    return "There is no project with that ID.\n";
  }
  
  
  ret += sprintf ("\n$I$5=Project %s (%s) belongs to the %s %s.\n", 
    id, projects[i]->name, query_multiple_short 
      (projects[i]->domains), (sizeof(projects[i]->domains) == 1 ?
      "domain" : "domains"));
  if (sizeof (projects[i]->description)) {
    ret += sprintf ("$I$5=Project %s has the following description: \n", 
      id, projects[i]->description);
  }        
  ret += sprintf ("$I$5=Project %s is %s percent complete.\n", 
    id, query_num (query_project_completeness (id)));
  ret += sprintf ("$I$5=Project %s has a project leader of: %s.\n",
    id, projects[i]->leader);
  ret += sprintf ("$I$5=Project %s has the following assigned creators:  %s.\n", 
    id, query_multiple_short (projects[i]->creators));
  ret += sprintf ("$I$5=Project %s is currently: in %s.\n", 
    id, STATUS[projects[i]->status]);
  ret += sprintf ("$I$5=Project %s was added at: %s by %s.\n",
    id, ctime (projects[i]->date_added), projects[i]->added_by);

  if (sizeof (projects[i]->twiki)) {
    ret += sprintf ("$I$5=Project %s has the following twiki pages: %s.\n", 
      id, query_multiple_short (projects[i]->twiki));
  }

  if (sizeof (projects[i]->tasks)) {  
    ret += sprintf ("$I$5=Project %s has the following tasks: %s.\n", 
      id, query_multiple_short (query_project_tasks (id)));
  }
  
  if (projects[i]->estimated_completion) {
    
    ret += sprintf ("$I$5=Project %s will be completed by the estimated date "
      "of: %s.\n", id, ctime (projects[i]->estimated_completion));
  }
  
//  if (file_name(previous_object())[0..3] == "/www" ) {
    ret = replace (ret, ({"$I$5=", "<p>"}));
//  }
  
  return ret;        
}

string task_info (string id, string task_id) {

  int i = find_project (id);
  int j;
  string ret = "";
    
  if (i == -1) {
    return "There is no project with that ID.\n";
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return "There is no task with that ID for project " + id + ".\n";
  }
  
  ret += sprintf ("Project %s:  task %s.\n", id, task_id);
  ret += sprintf ("task %s is %s percent complete.\n", 
    task_id, query_num (projects[i]->tasks[j]->percent_complete));
  ret += sprintf ("task %s has the following assigned creators:  %s.\n", 
    query_multiple_short (projects[i]->tasks[j]->assigned_to));
  
  return ret;
}

int set_project_completeness (string id, int percent) {
  int i = find_project (id);
  
  if (i == -1) {
    return -1;
  }
  
  projects[i]->percent_complete = percent;
}

int set_project_name (string id, string name) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->name = name;
}

string query_project_name (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->name;
}


int query_project_completeness (string id) {
  int i = find_project(id);
  int percent = 0;
  int counter = 0;
  string p;
  
  if (i == -1) {
    return 0;
  }
  
  if (projects[i]->percent_complete) {
    return projects[i]->percent_complete;
  }
  
  if (!sizeof (projects[i]->tasks)) {
    return 0;
  }
  
  if (sizeof (projects[i]->sub_projects)) {
    foreach (p in projects[i]->sub_projects) {
      if (p == id) {
        continue;
      }
      
      percent += query_project_completeness (p);
      counter++;
    }
    
  }
  
    
  foreach (class task bing in projects[i]->tasks) {
      if (!intp (bing->percent_complete)) {
        bing->percent_complete = to_int (bing->percent_complete);
      }
      
      percent += bing->percent_complete;
      counter++;
  }
  
  return percent / counter;
}

int update_project_task_completeness (string id, string task, int percent) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return -1;
  }
  
  j = find_task (i, task);
  
  if (j == -1) {
    return -1;
  }
  
  projects[i]->tasks[j]->percent_complete = percent;
}

int assign_project_task_creator (string id, string task, string creator) {
  int i = find_project (id);
  int j;
  string *cres;
  
  if (i == -1) {
    return -1;
  }
  
  j = find_task (i, task);
  
  if (j == -1) {
    return -1;
  }
  
  cres = projects[i]->tasks[j]->assigned_to;
  
  if (!sizeof (cres)) {
    cres = ({ creator });
  }
  else {
    cres += ({ creator });
  }
  
  projects[i]->tasks[j]->assigned_to = cres;
  
  if (member_array (creator, projects[i]->creators) == -1) {
    add_creator (id, creator);
  }
  
  return 1;
}

int add_task_to_project (string id, string task_id, string name, string aim) {
  int i = find_project (id);
  int j;
  class task tmp;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j != -1) {
    return 0;
  }
  
  tmp = new (class task, id: task_id, added_by: name, 
    date_added: time(), aims: aim);
  
  touch_project (id, "Added task " + task_id, name);
  return insert_into_tasks (i, tmp);  
}

int add_note_to_project (string id, string note_id, string note, string name) {
  int i = find_project (id);
  int j;
  class note tmp;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_note (i, note_id);
  
  if (j != -1) {
    return 0;
  }
  
  tmp = new (class note, id: note_id, note: note, added_by: name, 
    date_added: time());
  
  touch_project (id, "Added note " + note_id, name);
  return insert_into_notes (i, tmp);  
}


int set_task_name (string id, string task_id, string name) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  projects[i]->tasks[j]->task_name = name;
  
  return 1;  
}

int set_task_aims (string id, string task_id, string aims) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  projects[i]->tasks[j]->aims = aims;
  
  return 1;  
}

string query_task_aims (string id, string task_id) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  return projects[i]->tasks[j]->aims;
  }


string query_task_name (string id, string task_id) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  return projects[i]->tasks[j]->task_name;
  
}  

class task* project_task_data (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return ({ });
  }
  
  return projects[i]->tasks;
}

class note* project_note_data (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return ({ });
  }
  
  return projects[i]->notes;
}



int set_task_comments (string id, string task_id, string comments) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  projects[i]->tasks[j]->comments = comments;
  
  return 1;  
}

string query_task_comments (string id, string task_id) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  return projects[i]->tasks[j]->comments;
  
}  

int add_task_creator (string id, string task_id, string creator) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }

  if (!sizeof (projects[i]->tasks[j]->assigned_to)) {
    projects[i]->tasks[j]->assigned_to= ({ creator });
  }
  else {
    projects[i]->tasks[j]->assigned_to+= ({ creator });
  }
    
  return 1;  
}

string* query_task_creators (string id, string task_id) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  return projects[i]->tasks[j]->assigned_to;
  
}  

int set_task_completion (string id, string task_id, int percent) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }

  projects[i]->tasks[j]->percent_complete = percent;
      
  return 1;  
}

int query_task_competion (string id, string task_id) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  return projects[i]->tasks[j]->percent_complete;
  
}  

int set_task_completion_date(string id, string task_id, int date) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }

  projects[i]->tasks[j]->completion_date = date;
      
  return 1;  
}

int query_task_completion_date (string id, string task_id) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, task_id);
  
  if (j == -1) {
    return 0;
  }
  
  return projects[i]->tasks[j]->completion_date;
  
}

class project* filter_by_query (mapping query) {
  string tmp_str, *tmp_arr;
  int match, unassigned = 0;
  class project *tmp = ({ });
  string *un = ({"unassigned", "none", "no-one", "noone", "nobody"});

  if (member_array (query["creators"], un) != -1) {
    unassigned = 1;
  }
      
  foreach (class project p in projects) {
    
    match = 0;

    tmp_str = query ["projid"];
    
    if (sizeof (tmp_str)) {
      if (p->id == query["projid"]) {
        match = 1;
      }
      else {
        continue;
      }
    }
    
    tmp_str = query ["domains"];
  
    if (sizeof (tmp_str)) {      
      tmp_arr = explode (replace (tmp_str, ({" ", ""})), ",");
      
      if (sizeof (map (tmp_arr, (: lower_case ($1) :)) 
        & map (p->domains, (: lower_case ($1) :))) == sizeof (tmp_arr)) {
        match = 1;
      }
      else {
        continue;
      }
      
    }
    
    tmp_str = query["creators"];

    if (sizeof (tmp_str)) {      
      
      tmp_arr = explode (replace (tmp_str, ({" ", ""})), ",");
      
      if (sizeof (map (tmp_arr, (: lower_case ($1) :)) 
        & map (p->creators, (: lower_case ($1) :))) == sizeof (tmp_arr)) {
        match = 1;
      }
      else if (unassigned && !sizeof (p->creators)) {
        match = 1;
      }
      
      else {
        continue;
      }      
    }
       
    if (sizeof (query["status"])) {
     
      if (query["status"] == "all") {
        match = 1;
      }
      else if (p->status == member_array (query["status"], STATUS)) {
        match = 1;
      }
      else {
        continue;
      }
    }
    
    if (sizeof (query["leader"])) {
      if (lower_case (p->leader) == lower_case (query["leader"])) {
        match = 1;
      }
      else {
        continue;
      }      
    }
    
    
    if (query["touched"]) {
      if (p->last_touched > time() - query["touched"] ) {
        match = 1;
      }
      else {
        continue;
      }
    }  

    if (query["completeness"]) {
      if (query_project_completeness (p->id) 
        >= to_int (query["completeness"])) {
        match = 1;
      }
      else {
        continue;
      }
    }  
    
    tmp_str = query["playtesters"];

    if (sizeof (tmp_str)) {      
      
      tmp_arr = explode (replace (tmp_str, ({" ", ""})), ",");
      
      if (sizeof (tmp_arr & p->playtesters) == sizeof (tmp_arr)) {
        match = 1;
      }      
      else {
        continue;
      }      
    }    
    
    if (match) {
      tmp += ({ p });
    }
        
  }
  
  return tmp;        
}


class task* filter_tasks_by_query (string id, mapping query) {
  string tmp_str, *tmp_arr;
  int match, i, j;
  class task *tmp = ({ });
  
  i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  
  foreach (class task p in projects[i]->tasks) {
    
    match = 0;
    
    tmp_str = query["creators"];

    if (sizeof (tmp_str)) {      
      
      tmp_arr = explode (replace (tmp_str, ({" ", ""})), ",");
      
      if (sizeof (tmp_arr & p->assigned_to) == sizeof (tmp_arr)) {
        match = 1;
      }
      else {
        continue;
      }      
    }
       
    if (sizeof (query["completed"])) {

      j = to_int (query["completed"]);
      
      if (p->percent_complete > j) {           
        match = 1;
      }
      else {
        continue;
      }
    }
    
    if (match) {
      tmp += ({ p });
    }
        
  }
  
  return tmp;        
}


class project get_project_file (string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i];
}



void set_project_file (string id, class project tmp, string name) {
  int i = find_project (id);
  
  
  if (i == -1) {
    return 0;
  }
  
  projects[i] = tmp;
  touch_project (id, "Updated project file", name);
  catch_status_change (id, tmp->status);
}

class project* data_dump() {
  mapping dom_proj = ([ ]);   
  string d; 
  class project *tmp = ({ });
  string *dom;
  
  
  foreach (class project p in projects) {
    foreach (d in p->domains) {      
      if (undefinedp (dom_proj [d])) {
        dom_proj [d] = ({ p });
      }
      else {
        dom_proj [d] += ({ p });
      }      
    }
  }
  
  
  dom = sort_array (keys (dom_proj), 1);
  
  foreach (d in dom) {
    tmp += dom_proj[d];
  }
  
  
  return uniq_array (tmp);

}

class task get_task_file (string id, string t) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, t);
  
  if (j == -1) {
    return 0;
  }
    
  return projects[i]->tasks[j];
}

void set_task_file (string id, string t, class task tmp, string name) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_task (i, t);
  
  if (j == -1) {
    return 0;
  }
    
  projects[i]->tasks[j] = tmp;
  touch_project (id, "Updated task " + t, name);

}

class note get_note_file (string id, string t) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_note (i, t);
  
  if (j == -1) {
    return 0;
  }
    
  return projects[i]->notes[j];
}

void set_note_file (string id, string t, class note tmp, string name) {
  int i = find_project (id);
  int j;
  
  if (i == -1) {
    return 0;
  }
  
  j = find_note(i, t);
  
  if (j == -1) {
    return 0;
  }
    
  projects[i]->notes[j] = tmp;
  touch_project (id, "Updated note " + t, name);

}


int is_creator_on_project (string name, string id) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }
  
  if (member_array ("all", projects[i]->creators) != -1) {
    return 1;
  }
  
  if (member_array (name, projects[i]->creators) != -1) {
    return 1;
  }
  
  return 0;
}

varargs int valid_operation (string name, string action, string project, 
  string* dom) {  
  object ob;
  string *domains;
  int i = find_project (project);
  int j;
    
  if (master()->high_programmer (name)) {
    return 1;
  }
  
  if (name == "drakkos") {
    return 1;
  }  
    
  if (i != -1) {
    domains = query_project_domains (project);
  }
  else if (dom) {
    domains = dom;
  }
  
  if (is_creator_on_project (name, project)) {
    return 1;
  }
  
  if (action == PLAYTESTING) {
    ob = find_object ("/d/playtesting/master");
    
    if (ob->query_lord() == name) {
      return 1;
    }
    
    if (ob->query_deputy (name)) {
      return 1;
    }
          
  }
  
  if (!sizeof (domains)) {
    return 0;
  }
  
  foreach (string d in domains) {
    ob = find_object ("/d/" + d + "/master");


    if (function_exists (CALLBACK, ob)) {
      j = call_other (ob, CALLBACK, name, action, project);
      if (j != -1) {
        return j;
      }
      
    }
  
    if (ob->query_lord() == name) {
      return 1;
    }
  
    if (ob->query_deputy (name) && action != PLAYTESTING) {
      return 1;
    }
  }
    
  if (action == PROJECT) {
    return 1;
//    return (lower_case (name) == lower_case (query_project_leader (project)));
    return sizeof (filter (domains, (: 
      find_object ("/d/" + $1 + "/master")->query_member ($(name)) :)));
  }
  
  if (action == TASK || action == NOTE) {
    return is_creator_on_project (name, project);
  }
  
  return 0;  
}


void catch_status_change (string id, int status) {
  
  object ob;
  int i = find_project (id);
    
  foreach (string d in projects[i]->domains) {
    ob = load_object ("/d/" + d + "/master");
    
    if (!ob) {
      continue;
    }
      
    if (function_exists ("project_status_change", ob)) {
      ob->project_status_change (id, status);
    }
  }
    
  if (status == member_array ("play", STATUS)) {
    WHATWHAT->add_project (projects[i]->name, projects[i]->domains,
      projects[i]->creators, projects[i]->description, "play");
  }
  
  if (status != member_array ("playtesting", STATUS)) {
    clear_playtesting_projects (projects[i]->playtesters);
    projects[i]->playtesters = ({ });
    save_file();
  }
  projects[i]->last_status_change = time();
  
}

int assign_playtesters (string id, string *pt, string name) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }

  pt = map(pt, (: lower_case ($1) :));  
  pt = filter (pt, (: PLAYTESTER_HAND->query_playtester ($1) :));
  
  projects[i]->playtesters = pt;
  save_file();
  foreach (string p in pt) {
    if (pt_assignment_cache [p]) {
      map_delete (pt_assignment_cache, p);
    }
  }
  
  return 1;
  
  
}

string* add_playtester (string id, string *pt) {
  int i = find_project (id);
  string *names;
  
  if (i == -1) {
    return 0;
  }

  pt = map(pt, (: lower_case ($1) :));  
  pt = filter (pt, (: PLAYTESTER_HAND->query_playtester ($1) :));
  
  names = projects[i]->playtesters;
  
  projects[i]->playtesters = uniq_array (names += pt);
  clear_playtesting_projects (pt);

  save_file();
  do_mail (pt, projects[i]->id);
  return pt;
}

int remove_playtester (string id, string *pt) {
  int i = find_project (id);
  
  if (i == -1) {
    return 0;
  }

  pt = map(pt, (: lower_case ($1) :));  
  
  projects[i]->playtesters -= pt;
  clear_playtesting_projects (pt);

  save_file();
  return 1;
}


void do_load_tmp() {
  unguarded ((: restore_object, TMP_SAVE :));

}


void do_setup() {
  projects = ({ });
  foreach (class project p in tmp_projects) {
    projects += ({ p });
  }
  tmp_projects = ({ });
  do_save();
  
}

int clear_file_cache (string nfile) {
  int i = find_cache_file (nfile);
  
  if (i == -1) {
    return 0;
  }
  
  cache -= ({ cache[i] });
  
  return 1;
}


int query_permission (string project, string name, string nfile, int access) {
  int i = find_project (project);
  int j;
  string *tasks = ({ });
  class file *tmp = ({ });
    
  if (i == -1) {
    return 0;
  }
  
  if (projects[i]->leader != name && 
    member_array (name, projects[i]->creators) == -1) {
    return 0;
  }
    
  if (projects[i]->leader == name) {
    tasks = map (projects[i]->tasks, (: $1->id :));
    tasks += ({"leader"});
  }
  else {
    tasks = map (filter (projects[i]->tasks, (: member_array ($(name), 
      $1->assigned_to) != -1 :)), (: $1->id :));
  }
    
  tasks += ({""});

  foreach (string t in tasks) {   
    tmp += find_file (i, nfile, t);
  }
  
  if (!sizeof (tmp)) {
    return 0;
  }
   
  tmp = uniq_array (tmp);    
  
  foreach (class file f in tmp) {
    j = find_cache_file (f->file);
    
    if (j == -1) {
      j = add_cache_file (f->file);
    }
    
    
    if (!sizeof (f->task)) {
      if (f->access == access) {
        if (cache[j]->accesses[name] < access) {
          cache[j]->accesses[name] = access;
        }
        return 1;
      }
      
    }
    else if (member_array (f->task, tasks) != -1) {
        if (f->access == access) {
          if (cache[j]->accesses[name] < access) {
            cache[j]->accesses[name] = access;
          }
          
          return 1;
        }
        
    }
  }
  
  return 0;      
}

int nuke_creator_project_cache(string *creators) {
  
  if (!creator_project_cache) {
    return 0;
  }
  
  foreach (string c in creators) {
    map_delete (creator_project_cache, c);
    return 1;
  }
}



int query_file_permission (string name, string nfile, int access) {
  string *a_projects = query_projects_for_creator (name);
  int i, j, k, l = 0;
  
  if (!sizeof (a_projects)) {
    return 0;
  }

  j = find_cache_file (nfile);
    
  if (j != -1 ) {
    k = cache[j]->accesses[name];
    
    if (k == -1) {
      return 0;
    }
    
    if (k >= access) {
      return 1;
    }
  }
        
  foreach (string p in a_projects) {
    
    i = query_permission (p, name, nfile, access);
 
    if (!l && i) {
      l = 1;
    }
    
  }
    
  return l;
    
}


void set_project_lock (string project) {
  project_locks[project] = time();
}

int query_project_lock (string project) {
  if (project_locks[project] + PROJECT_LOCK > time()) {
    return 1;
  }
  
  map_delete (project_locks, project);
  return 0;
}

string generate_pt_report() {
  string *new_cache = ({ });
  string missing_projects = "";
  string ret = "";
  string *det = ({ });
  string project_list = "";
  string tmp;
  string post = "";
  int i;
  class project *in_pt = ({ });
  class project *out_pt = ({ });
  class project *new_pt = ({ });
  string *del = ({ });
 
  class project *playtesting = filter_by_query ((["status": "playtesting"]));
  


  foreach (class project p in playtesting) {
      new_cache += ({ p->id });
      if (member_array (p->id, pt_post_cache) == -1) {
        new_pt += ({ p });
      }
      else {
        in_pt += ({ p });
      }
    }
    
  if (sizeof (pt_post_cache)) {
    foreach (string id in pt_post_cache) {
      if (member_array (id, new_cache) == -1) {
        i = find_project (id);
        if (i == -1) {
          del += ({ id });
        }
        else {
          out_pt += ({ projects[i] });
        }
      }
    }
  }    
  
  if (sizeof (new_pt)) {  
    ret += "The following projects have entered playtesting:\n\n";
    foreach (class project p in new_pt) {
      det = ({ });
      det += ({sprintf ("*  %s (%s): %s.\n   Description: %s\n   "
        "Project leader: %s.\n   "
        "Creators: %s.\n   Notes: %s.\n", p->id, p->name, 
          query_multiple_short (p->domains), (p->description ? replace 
            (p->description, ({"\n", ""})) : 
          "None"), (p->leader ? p->leader : 
          "No-one"), (p->creators ? query_multiple_short (p->creators) :
          "None"), (sizeof (p->pt_notes) ? replace (p->pt_notes, 
          ({"\n", ""})) : "None")) });
          
      ret += implode (det, "\n\n");          
    } 
    ret += "\n\n";
  }      
  
  if (sizeof (in_pt)) {  
    ret += "The following projects are still in playtesting:\n\n";
    det = ({ });
    foreach (class project p in in_pt) {
      det += ({sprintf ("*  %s (%s): %s.", p->id, p->name, 
        query_multiple_short (p->domains)) });          
    } 
    ret += implode (det, "\n");    
    ret += "\n\n";      
  }   

  if (sizeof (out_pt)) {  
    det = ({ });
    ret += "The following projects have been removed from playtesting:\n\n";
    foreach (class project p in out_pt) {
      det += ({sprintf ("*  %s (%s): %s.", p->id, p->name, 
        STATUS[p->status]) });
          
    } 
    ret += implode (det, "\n\n");          
    ret += "\n\n";
  }    
  
  if (sizeof (del)) {    
    ret += "The following projects have been deleted from the handler:\n\n";
    foreach (string d in del) {
      det += ({sprintf ("*  %s.", d) });
    }
    ret += implode (det, "\n\n");        
    ret += "\n\n";
  }
  
  pt_post_cache = new_cache;  
    
  if (!sizeof (ret)) {
    ret= "Wow, absolutely nothing is officially in playtesting at the "
      "moment.  You can all have the week off.\n\n";
  }
  else {
    ret += "Remember that you can use the 'playtesting' command for more "
      "information on projects that are currently in playtesting.\n\n";
  }
    
  
  return ret;
     
      
}  

int make_post() {
  int ret;

  ret = BOARD_HAND->add_message("ptforum",
         "The Project Tracker", "What's in Playtesting?",
         generate_pt_report() +
         TWIKI_HANDLER->find_changes_in_database("Playtesters", last_summary));

  last_summary = time();
  save_file();    
}

string* query_pt_cache() {
  return pt_post_cache;
}

string* query_pt_projects (string name) {
  class project *tmp;

  if (!undefinedp (pt_assignment_cache [name])) {
    return pt_assignment_cache [name];
  }
    
  tmp = filter_by_query ((["playtesters" : name ]));
  
  if (!tmp) {
    pt_assignment_cache[name] = ({ });
  }
  else {
    pt_assignment_cache[name] = map (tmp, (: $1->id :));
  }
  
  return pt_assignment_cache[name];
}


int query_number_of_pts (mixed proj) {
  int i;
  int t;
  
  if (!intp (proj)) {
    i = find_project (proj);
  }
  else {
    i = proj;
  }
  
  if (i == -1) {
    return -1;
  }  

  switch (projects[i]->size) {
    case 0:
      t = 2;
    break;
    case 1:
      t = 3;
    break;
    case 2:
      t = 4;
    break;
    case 3:
      t = 5;
    break;
    case 4:
      t = 6;
    break;
    case 5:
      t = 8;
    break;
    case 6:
      t = 0;
    break;
    default:
      t = 2;
    break;
  }  

  return t;
}

int next_rotation_time(mixed proj) {
  int i, t;
  
  if (!intp (proj)) {
    i = find_project (proj);
  }
  else {
    i = proj;
  }

  if (i == -1) {
    return -1;
  }

  if (projects[i]->complexity == member_array ("special", COMPLEXITY)) {
    return 0;
  }
    
  switch (projects[i]->size) {
    case 0:
      t = WEEK;
    break;
    case 1:
      t = WEEK * 2;
    break;
    case 2:
      t = WEEK * 2;
    break;
    case 3:
      t = WEEK * 4;
    break;
    case 4:
      t = WEEK * 4;
    break;
    case 5:
      t = WEEK * 6;
    break;
    case 6:
      t = WEEK * 6;
    default:
      t = WEEK;
  }  
 
  if (!projects[i]->current_rotation) {
    projects[i]->last_rotation = time();
    projects[i]->current_rotation = 1;
    save_file();
  }
  
  return projects[i]->last_rotation + t;
}

int query_time_to_rotate (mixed proj) {
  int i;
  
  i = next_rotation_time(proj);
      
  if (i < time()) {
    return 1;
  }
  else {
    return 0;
  }
}
      
int query_number_rotations(mixed proj) {
  int i ;

  if (!intp (proj)) {
    i = find_project (proj);
  }
  else {
    i = proj;
  }
    
  if (i == -1) {
    return -1;
  }
  
  if (projects[i]->complexity == member_array ("special", COMPLEXITY)) {
    return projects[i]->current_rotation + 1;
  }
  return projects[i]->complexity + 2;
}

int query_rotation (mixed proj) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->current_rotation;
}

string* query_playtesters (mixed proj) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->playtesters;
}



int set_complexity (mixed proj, int com) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->complexity = com;
  return 1;
}

int set_size(mixed proj, int siz) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->size = siz;
  return 1;
}

int query_size(mixed proj) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->size;
}



int set_guild_specific (mixed proj, string *guilds) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  projects[i]->guild_specific = guilds;
  return 1;
}

string *query_guild_specific(mixed proj) {
  int i = (!intp (proj) ? find_project (proj) : proj);  
  
  if (i == -1) {
    return 0;
  }
  
  return projects[i]->guild_specific;

}

int clear_playtesting_projects (string *names) {
  foreach (string v in names) {
    if (pt_assignment_cache[v]) {
      map_delete (pt_assignment_cache, v);
    }

    if (!num_assignments[v]) {
      num_assignments[v] = 1;
    }
    else {
      num_assignments[v] += 1;
    }
  }  
  save_file();
}

class project* get_playtesting_projects () {
  class project *tmp;
  
  tmp = filter_by_query ((["status": "playtesting"]));
   
  return tmp;
}

void set_rotations (string proj, int rot) {
  int i = find_project (proj);
  
  if (i == -1) {
    return;
  }
  
  projects[i]->current_rotation = rot;
}

string* assign_pts_to_project (mixed proj) {
  int i = (!intp (proj) ? find_project (proj) : proj);
  int needed, tmp;
  int min = sizeof (projects) + 1, max = -1;
  mapping guilds;
  mapping tmp_assign_cache = ([ ]);
  string tpt;
  string *valid = ({ });
  string *selected = ({ });
  string *virgins = ({ });
  string *ordered = ({ });
  string *last = ({ });
  string *nvalid = ({ });
  string *unassigned = ({ });
  string *assigned = ({ });
  
  
  if (i == -1) {
    return 0;
  }


  if (projects[i]->size != member_array ("special", SIZE)) {
    needed = query_number_of_pts (proj);
  }
  else {
    return ({ });
  }  
  
  if (sizeof (projects[i]->guild_specific)) {
    guilds = PLAYTESTER_HAND->query_guilds();
    
    foreach (string g in projects[i]->guild_specific) {
      foreach (string name, string gu in guilds) {
        if (load_object (gu)->query_name() == g) {
          valid += ({ name });
        }
      }      
    }
  }
  else {
    valid = PLAYTESTER_HAND->query_playtesters();
  }

  
  foreach (string v in valid) {
    if (!sizeof (query_pt_projects (v))) {
      unassigned += ({ v });
    }
    else {
      assigned += ({ v });
    }
  }
  
  if (sizeof (unassigned) >= needed) {
    valid = unassigned;
  }
  else {
    valid = unassigned;
    while (sizeof (valid) < needed) {
      tmp = random (sizeof (assigned));
      valid += ({ assigned[tmp] });
      assigned -= ({ assigned[tmp] });
    }
  }
    
  foreach (string va in valid) {
    if (member_array (lower_case (va), excluded) != -1) {
      nvalid += ({ va });
    }
    if (PLAYTESTER_HAND->query_leave (va)) {
      nvalid += ({ va });
    }
  }

  valid -= nvalid;  
  virgins = valid - (projects[i]->last_pt_cohort ? projects[i]->last_pt_cohort : ({ }));
  
  if (sizeof (projects[i]->last_pt_cohort)) {
    clear_playtesting_projects (projects[i]->last_pt_cohort);
  }
  last = projects[i]->last_pt_cohort;
  projects[i]->last_pt_cohort = ({ });
  projects[i]->last_rotation = time();


  foreach (string v in valid) {
    tmp_assign_cache[v] = sizeof (query_pt_projects (v)) + num_assignments[v];    
  }
  
  foreach (string pt, int val in tmp_assign_cache) {
    if (val < min) {
      min = val;
    }
    if (val > max) {
      max = val;
    }
  }
      
  for (int v = min; v <= max; v++) {
    ordered = ({ });
    
    foreach (string pt, int val in tmp_assign_cache) {
      if (val == v) {
        ordered += ({ pt });
      }        
    }
     
    if ((sizeof (selected) + sizeof (ordered))  >= needed) {
      do {
        tpt = ordered[random(sizeof(ordered))];
        selected += ({ tpt });
        ordered-= ({ tpt });
      } while (sizeof (selected) < needed);
      projects[i]->playtesters = selected;
      save_file();
      if (!last) {
        last = ({ });
      }
      clear_playtesting_projects ((selected ? selected : ({ })) + (last? last : ({ })));
      do_mail (selected, projects[i]->id);         
      return selected;      
    }    
    else {
      selected += ordered;
    }
  }
  
  valid = uniq_array (valid);
  
  projects[i]->last_pt_cohort = projects[i]->playtesters;
  if (sizeof (projects[i]->playtesters)) {
    if (sizeof (projects[i]->all_pts)) {
      projects[i]->all_pts += ({ projects[i]->playtesters });
    }
    else {
      projects[i]->all_pts = ({ projects[i]->playtesters });
    }
    
  }
  
  projects[i]->playtesters = selected;
    
  clear_playtesting_projects ((selected ? selected : ({ })) + (last? last : ({ })));
  
  do_mail (selected, projects[i]->id);
  
  save_file();
  return selected;       
}

void rotate_project (mixed tmp) {
  int i;
  
  if (intp (tmp)) {
    i = tmp;
  }
  else {
    i = find_project (tmp);
  }
  
  if (projects[i]->current_rotation == query_number_rotations (i)) {
    // Set it to Limbo
    AUTO_MAILER->auto_mail( "drakkos", "The Playtester Wheel Of Fate", 
      "Your project in playtesting", projects[i]->leader, 
      "Bing!\n\nThe project " + projects[i]->id + " has reached the end of "
      "its automatic playtesting lifetime and has been put back into a state "
      "of limbo.  If you wish for the project to continue in playtesting, then "
      "please set the project back into playtesting and give Drakkos a nudge!\n\n"
      "Thx bye,\nThe Playtester Wheel Of Fate.\n", 0, 0, 1 );       
    
    AUTO_MAILER->auto_mail( "drakkos", "The Playtester Wheel Of Fate", 
      "Your project in playtesting", implode (projects[i]->playtesters, ","), 
      "Bing!\n\nThe project " + projects[i]->id + " has reached the end of "
      "its automatic playtesting lifetime and has been put back into a state "
      "of limbo.  Thank you for your work on the last stage of its current "
      "playtesting phase.\n\n"
      "Thx bye,\nThe Playtester Wheel Of Fate.\n", 0, 0, 1 );             
    set_status(projects[i]->id, 4);
    projects[i]->current_rotation = 0;
    return;
  }
  projects[i]->current_rotation = projects[i]->current_rotation+1;
  projects[i]->last_rotation = time();
  assign_pts_to_project(i);
  
}



void reset() {
  if (!last_summary) {
    last_summary = time();
  }
  
  if ((last_summary + TIME_BETWEEN_POSTINGS) < time()) {
    make_post();
  }
  
  for (int i = 0; i < sizeof (projects); i++) {
    if (projects[i]->status != member_array ("playtesting", STATUS)) {
      continue;
    }
    
    if (query_time_to_rotate (i)) {
      rotate_project (i);
    }
  }
  
}
  

/*

int validate_structure() {
  int i;
  class project_tmp updated;
  
  tmp_projects = ({ });
  
  for (i = 0; i < sizeof (projects); i++) {
      updated = new (class project_tmp);
      updated->id = projects[i]->id;
      updated->name = projects[i]->name;
      updated->leader = projects[i]->leader;
      updated->added_by = projects[i]->added_by;
      updated->description = projects[i]->description;
      updated->pt_notes = "";
      updated->domains = projects[i]->domains;
      updated->creators = projects[i]->creators;
      updated->sub_projects = projects[i]->sub_projects;
      updated->files = ({ });
      updated->last_rotation = 0;
      updated->size = 1;
      updated->complexity = 1;
      updated->current_rotation = 0;
      updated->last_pt_cohort = ({ });
      updated->all_pts = ({ });
      updated->guild_specific = ({ });
      updated->twiki = projects[i]->twiki;
      updated->playtesters = projects[i]->playtesters;
      updated->what_touched = projects[i]->what_touched;
      updated->touched_by = projects[i]->touched_by;
      updated->date_added = projects[i]->date_added;
      updated->date_completed = projects[i]->date_completed;
      updated->status = projects[i]->status;
      updated->percent_complete = projects[i]->percent_complete;
      updated->estimated_completion = projects[i]->estimated_completion;
      updated->last_touched = projects[i]->last_touched;
      updated->last_status_change = time();
      updated->tasks = projects[i]->tasks;
      updated->notes = projects[i]->notes;
    
      tmp_projects += ({ updated });
  }
  
  save_object (TMP_SAVE);
  return 1;
}



int migrate() {
  projects = tmp_projects;
  tmp_projects = ({ });
  save_file();
  return 1;
}

*/

void do_mail (string *playtesters, mixed proj) {
  string text = "Hello!\n\nYou have been assigned to project " +
    proj + " on its latest rotation.  You can use " 
    "'playtesting details " + proj + " to find out more about "
    "the project if neccessary.\n\nRegards,\nThe Playtester Wheel "
    "Of Fate.\n";
  
  AUTO_MAILER->auto_mail( "drakkos", "The Playtester Wheel Of Fate", 
    "Your new playtester assignment", implode (playtesters, ","), 
    text, 0, 0, 1 );        
}


void clear() {
  string *names;
  
  for (int i = 0; i < sizeof (projects); i++) {
    projects[i]->all_pts = ({ });
    projects[i]->last_pt_cohort = ({ });
    projects[i]->playtesters = ({ });    
  }
  
  names = PLAYTESTER_HAND->query_playtesters();
  
  clear_playtesting_projects (names);
  save_file();
  
}

void test_num() {
  printf ("%O\n", pt_assignment_cache);
}