/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: base_master.c,v 1.17 2003/04/15 23:52:04 pinkfish Exp $
 * $Log: base_master.c,v $
 * Revision 1.17  2003/04/15 23:52:04  pinkfish
 * Fix up the assignment stuff anfd add in the ability to set a directory
 * assigned to no one.
 *
 * Revision 1.16  2003/03/05 22:40:59  pinkfish
 * Fix up an error which caused the size of the array to get over keen.
 *
 * Revision 1.15  2003/03/05 22:30:32  pinkfish
 * Make the lord default to the top of the domain.
 *
 * Revision 1.14  2003/03/04 22:57:33  pinkfish
 * Fix it up to remove things correctly.
 *
 * Revision 1.13  2003/03/04 22:47:21  pinkfish
 * Fix up the assignment stuff.
 *
 * Revision 1.12  2003/03/04 22:38:45  pinkfish
 * Make it do directory assignment stuff.
 *
 * Revision 1.11  2002/03/01 15:14:24  taffyd
 * Fixed a STUPID mistake.
 *
 * Revision 1.10  2002/02/24 17:02:47  taffyd
 * Added set_idle_mess() and query_idle_mess() for use in /secure/finger.
 *
 * Revision 1.9  2002/02/24 06:20:05  taffyd
 * Added better rcsforce allowed code
 *
 * Revision 1.8  2001/11/05 06:33:46  drakkos
 * Added support for $project$ codes in creator projects to query the project tracker for project info.
 *
 * Revision 1.7  2001/09/08 00:23:13  ceres
 * Mofified to let HLs change stuff
 *
 * Revision 1.6  2001/08/14 14:36:48  drakkos
 * Added a base query_changelog() method.
 *
 * Revision 1.5  2001/04/05 21:00:25  pinkfish
 * Add in allowing deputies to rcsforce.
 *
 * Revision 1.4  2001/02/27 23:54:22  pinkfish
 * Fix up the copy() around the deputies.
 *
 * Revision 1.3  2001/02/27 23:53:27  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.2  1999/10/04 23:31:22  turrican
 * Fixed permission checking function
 *
 * Revision 1.1  1998/01/06 04:03:45  ceres
 * Initial revision
 * 
*/
/*
 * This file is inherited by the domain master objects.
 */

#include <project_management.h>
#include <player_handler.h>
#include <error_handler.h>

#define LOCK_MASK 8
#define PROJECT_CODE "$project$"

mapping members;
mapping access;
nosave private mapping _project_cache;
private int _use_tracker;
private string _idle_mess;
string *deputies;
private mapping _directory_assignments;
private mapping _rcsforce_allowed; 

string query_lord();
int query_use_tracker();
void set_use_tracker(int);

string query_domain() {
  string *words;
  
  if (!file_name (this_object())) {
    return "";
  }    
  words =  explode (file_name(this_object()), "/");
  
  if (sizeof (words) < 2) {
    return "";
  }
  
  return words[1]; 
}

int query_prevent_shadow() {
   return 1;
}                               /* query_prevent_shadow() */

protected void create() {
   members = ([ ]);
   access = ([ ]);
   deputies = ({ });
   _project_cache = ([ ]);
   _rcsforce_allowed = ([ ]); 
   _idle_mess = 0; 
   unguarded((: restore_object, file_name(this_object()) :));
   if (!deputies) { 
      deputies = ({ });
   }

   if (!_directory_assignments) {
      _directory_assignments = ([ ]);
   }

   if ( !_rcsforce_allowed ) {
       _rcsforce_allowed = ([ ]);
   }
}                               /* create() */

void save_me() {
   unguarded((: save_object, file_name(this_object()) :));
}                               /* save_me() */

int add_permission(string euid,
                   string path,
                   int mask)
{
   if (previous_object() != master()) {
      return 0;
   }
   if (!access[path]){
      access[path] = ([ euid : mask ]);
   } else {
      access[path][euid] |= mask;
   }
   save_me();
   return 1;
}                               /* add_permission() */

int remove_permission(string euid,
                      string path,
                      int mask)
{
   if (previous_object() != master())
      return 0;
   if (!access[path])
      return 0;
   if (!access[path][euid])
      return 0;
   access[path][euid] &= ~mask;
   if (!access[path][euid])
      map_delete(access[path], euid);
   if (!sizeof(access[path]))
      map_delete(access, path);
   save_me();
   return 1;
}                               /* remove_permission() */

int check_permission(string euid,
                     string *path,
                     int mask)
{
   string p;
   mapping perms;

   if ((sizeof(path) >= 3)
       && (path[2] == "master.c" || path[2] == "master.o"
           || path[2] == "master"))
      return 0;
   p = "/" + implode(path, "/");
   perms = master()->permission_match_path(access, p);
   if (!perms || !sizeof(perms)) {
      return 0;
   }
   if (perms[euid] && ((perms[euid] & mask) || (perms["all"] & mask))) {
      return 1;
   }
   if (perms["all"] & LOCK_MASK) {
      return -1;
   }
   return 0;
}                               /* check_permission() */

int valid_read(string,
               string euid,
               string)
{
   /*
    * Default is to allow reading for everyone.
    */
   return 1;
   /* Use this if you only want your domain members to be able to read in the
    * the domain. */
   return (members[euid] != 0);
}                               /* valid_read() */

int valid_write(string *path,
                string euid,
                string)
{
   /*
    * Default is to deny write access for everyone, except the lord
    * and independent creators.
    */
   return 0;
   /*
    * Blue...  Use this if you want all your creators to have
    * write access everywhere in the domain.
    */
   if ((sizeof(path) >= 3)
       && (path[2] == "master.c" || path[2] == "master.o"
           || path[2] == "master"))
      return 0;
   return (members[euid] != 0);
}                               /* valid_write() */

void dest_me()
{
   destruct(this_object());
}                               /* dest_me() */

int check_call_tree(int only_lord)
{
   object *obs;
   int found;
   int i;

   obs = previous_object(-1);
   for (i = 0; i < sizeof(obs); i++) {
      /* Look for all the creator/lord objects */
     if (file_name(obs[i])[0..12] == "/global/lord#" ||
         (file_name(obs[i])[0..15] == "/global/creator#")) {
       if("/secure/master"->query_administrator(obs[i]->query_name()) ||
          obs[i]->query_name() == query_lord() ||
          (!only_lord &&
           (member_array(obs[i]->query_name(), deputies) != -1))) {
         found = 1;
       } else {
         return 0;
       }
     }
   }
   return found;
}                               /* check_call_tree() */

int add_member(string name)
{
   if (!check_call_tree(0)) {
      return 0;
   }
   if (members[name])
      return 0;
   members[name] = "newbie";
   save_me();
   return 1;
}                               /* add_member() */

int remove_member(string name)
{
   int pos;

   /* Only the lord can delete deputies... */
   if (!check_call_tree(member_array(name, deputies) != -1)) {
      return 0;
   }
   if (!members[name])
      return 0;
   members = m_delete(members, name);
   pos = member_array(name, deputies);
   if (pos != -1) {
      deputies = deputies[0..pos - 1] + deputies[pos + 1..];
   }
   save_me();
   return 1;
}                               /* remove_member() */

int add_deputy(string name)
{
   if (!check_call_tree(1)) {
      return 0;
   }
   if (!members[name])
      return 0;
   if (!deputies)
      deputies = ({ });
   if (member_array(name, deputies) != -1)
      return 1;
   deputies += ({ name });
   save_me();
   return 1;
}                               /* add_deputy() */

int remove_deputy(string name)
{
   int pos;

   if (!check_call_tree(1)) {
      return 0;
   }
   if (!deputies)
      deputies = ({ });
   pos = member_array(name, deputies);
   if (pos == -1)
      return 0;
   deputies = deputies[0..pos - 1] + deputies[pos + 1..];
   save_me();
   return 1;
}                               /* remove_deputy() */

int add_rcsforce_allowed( string creator, string directory ) {
    if (!check_call_tree(1)) {
      return 0;
   }

   if ( _rcsforce_allowed[ creator ] ) {
        _rcsforce_allowed[ creator ] += ({ directory });
   }
   else { 
        _rcsforce_allowed[ creator ] = ({ directory }); 
   }

   save_me(); 
   return 1;
} /* add_rcsforce_allowed() */ 

int remove_rcsforce_allowed( string creator, string directory ) {
    if (!check_call_tree(1)) {
      return 0;
   }

   if ( _rcsforce_allowed[ creator ] ) {
        _rcsforce_allowed[ creator ] -= ({ directory });

        if ( sizeof( _rcsforce_allowed ) == 0 ) {
            map_delete( _rcsforce_allowed, creator );
        }
        save_me(); 
   }

   return 1;
} /* remove_rcsforce_allowed() */ 

string *query_members()
{
   return keys(members);
}                               /* query_members() */

int set_project(string name,
                string pro)
{
   if (!check_call_tree(0)) {
      return 0;
   }
   if (!members[name])
      return 0;
   if (!pro || pro == "")
      pro = "project unset";
   members[name] = pro;
   save_me();
   return 1;
}                               /* set_project() */

string query_project(string name)
{
  string *proj;
  string project;  
  
  project = members[name];

  // Check to see if they have a project set.  
  if (!project) {
    // Nope.  Return project unset.
    return "project unset";
  }
  
  // Check to see if the string $project$ appears
  // anywhere in the string.  If it does, we will
  // replace this string with the project information
  // from the tracker.
  if (strsrch (project, PROJECT_CODE) != -1) {
    // No sizeof the cache... so let's initialise it.
    if (!sizeof (_project_cache)) {
      _project_cache = ([ ]);
    }              
   if (undefinedp (_project_cache[name])) {    
      // We haven't already queried the manager for this 
      // creator, so we'd better do that.  We cache what we get
      // back so that we don't continually hammer the CPU every
      // time.
      proj = PROJECT_HANDLER->query_projects_for_creator_in_domain
        (name, query_domain());
            
      if (sizeof (proj)) {
        // There were projects returned by the tracker, so 
        // let's make the project cache equal a q_m_s string
        // of the projects.
        _project_cache[name] = query_multiple_short (proj);
      }
      else {
        // Nothing is stored for this creator, so we'll
        // simply replace $project$ with the currently set
        // project.  
        _project_cache[name] = members[name];
      }

    }
  
    // Now, return the proper project code.
    return replace (project, ({PROJECT_CODE, 
      _project_cache[name]}));
  }
  else {
    return members[name];
  }
}                               /* query_project() */

int query_deputy(string name)
{
   if (!deputies)
      return 0;
   if (member_array(name, deputies) != -1)
      return 1;
   return 0;
}                               /* query_deputy() */

string *query_deputies()
{
   return copy(deputies);
}

mapping query_access()
{
   return copy(access);
}                               /* query_access() */

string query_lord()
{
   return "Something is broken";
}                               /* query_lord() */

/**
 * Called to check for rcsforcing of a file.  By default we allow the deputies
 * to rcsforce things.
 * @param file the file to check
 * @param person the person forcing
 * @param locker the person who has the lock
 */
int can_rcsforce(string file, string person, string locker) {
    string *directories; 
    string fdir;

    if (query_deputy(person)) {
        return 1;
    }

    directories = _rcsforce_allowed[ person ];

    if ( directories ) { 
        fdir = file[ 0..strsrch( file, "/", -1 ) -1 ]; 
        foreach ( string directory in directories ) {
            if ( fdir[ 0.. sizeof( directory ) - 1 ] == directory ) {
                return 1;
            }
        }
    }
    
    return 0; 
} /* can_rcsforce() */

string query_changelog (string filename) {
  return  "/d/" + query_domain() + "/ChangeLog";
}

void set_use_tracker(int i) {
  _use_tracker = i;
}

int query_use_tracker() {
  return _use_tracker;
}

int nuke_creator_project_cache(string *creators) {
  
  if (!_project_cache) {
    return 0;
  }
  
  foreach (string c in creators) {
    map_delete (_project_cache, c);
    return 1;
  }
}

int set_idle_mess( string txt ) {
   if (!check_call_tree(1)) {
        return 0;
   }

   _idle_mess = txt;
   save_me();
   return 1;
} /* set_idle_mess() */

string query_idle_mess() {
    return _idle_mess;
} /* query_idle_mess() */ 

/**
 * Finds the person assigned to the specific directory.  It will climb
 * up the directory tree looking for who to assign the directory too.
 * @param dir the directory to find assignments in
 * @return the array of people assigned to the directory
 */
string* query_assigned_to_directory(string* dir) {
   string str;
   int i;

   i = sizeof(dir) - 1;
   do {
      str = "/" + implode(dir[0..<i], "/");
      i--;
   } while (i >= 0 && !_directory_assignments[str]);
   if (_directory_assignments[str]) {
      return _directory_assignments[str];
   }
   // Make the default the lord
   return ({ query_lord() });
}

/**
 * This method allows a specific person to be assigned to the given
 * directory.
 * @param dir the directory to assign someone too
 * @param people the people who are assigned to the directory
 */
int assign_people_to_directory(string dir, string* people) {
   if (previous_object() != master()) {
      return 0;
   }

   

   if (sizeof(people)) {
      if (sizeof(people) == 1 && people[0] == ERROR_ASSIGNED_NO_ONE) {
         people = ({ });
      }
      _directory_assignments[dir] = people;
   } else {
      map_delete(_directory_assignments, dir);
   }
   save_me();
   return 1;
}

/**
 * This method returns all the current assignments from directory to the
 * people who are looking after them.
 * @return the directory assignment mapping
 */
mapping query_directory_assignments() {
   mapping ret;
   string dir;

   ret = copy(_directory_assignments);
   dir = "/d/" + query_domain();
   if (!ret[dir]) {
      ret[dir] = ({ query_lord() });
   } else {
      ret[dir] |= ({ query_lord() });
   }
   return ret;
}
