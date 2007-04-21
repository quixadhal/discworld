#define LORD "taffyd"
#define DOMAIN "learning"

/*
 * Beware editing this file.
 * Make sure you know what you are doing.  Stuffing this up could be
 * fatal in some ways.
 */

inherit "/std/dom/base_master";

string info = "";

string query_lord() {
  return LORD;
}

string author_file(string *path) {
  return capitalize(DOMAIN);
}

int check_permission(string euid, string *path, int mask) {
  if (euid == query_lord())
    return 1;
  return ::check_permission(euid, path, mask);
}

/* Allow ftp downloads and copies with only read permission */
int copy_with_read(string path, string euid, string func) {
  return 1;
}

/*
 * By default, use the valid_read()/valid_write() functions defined in the
 * inherited file /std/dom/base_master.c.
 * These default to reading allowed for everyone, writing only allowed
 * for independent creators and the domain lord.
 * This is all overridden by the check_permission() function
 * which is called first, which should be used instead.

int valid_read(string *path, string euid, string funct);

int valid_write(string *path, string euid, string funct) {
  if ((sizeof(path) >= 3) && (path[2] == "master.c" || path[2] == "master.o" ||
                              path[2] == "master")){
    return 0;
  }
  
  if(path[2] == "cutnpaste" && master()->query_senior(euid)){
    return 1;
  }
  
  return (members[euid] != 0);
}
*/

/*
 * By default only the lord of the domain can add/remove members, or
 * change the projects of the members.
 * This is a bit contrictive, but handy as a default.
 * Change this if you want.
 */
int add_member(string name) {
  if (geteuid(this_player(1)) != query_lord())
    return 0;
  return ::add_member(name);
}

int remove_member(string name) {
  if (geteuid(this_player(1)) != query_lord())
    return 0;
  return ::remove_member(name);
}

int set_project(string name, string pro) {
  if (geteuid(this_player(1)) != query_lord())
    return 0;
  return ::set_project(name, pro);
}

int query_member(string name) {
  return !undefinedp(members[name]) || name == LORD;
} /* query_member() */

string log_who(string where) {
   return LORD;
} /* log_who() */

/* Please change this to a text describing the domain, it will be used
   on the WWW pages and in finger info. */

void set_info( string words ) {
  info = words;
  save_me();
} /* query_info() */

string query_info() {
  return info;
} /* query_info() */
