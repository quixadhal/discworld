#define LORD "pinkfish"
#define DOMAIN "admin"
#define LOGS "/d/admin/log/"
#define BACKUP_COUNT 9

/*
 * Beware editing this file.
 * Make sure you know what you are doing.  Stuffing this up could be
 * fatal in some ways.
 */

inherit "/std/dom/base_master";

void create() {
   string file;
   int i;
   
   ::create();
   foreach ( file in unguarded( (: get_dir, LOGS +"*.log" :) ) ) {
     if( ( unguarded( (: file_size, LOGS + file :) ) > 1024000) ||
         ( file != "ENTER.log" &&
           unguarded( (: file_size, LOGS + file :) ) > 64000 ) ) {
       file = explode( file, "." )[ 0 ];

       // delete the last one if necessary.
       if(file_size(LOGS+file+"."+BACKUP_COUNT) > 0)
         unguarded( (: rm, LOGS + file +"."+ BACKUP_COUNT :) );

       // rename those down to the first.
       for(i=(BACKUP_COUNT-1); i>0; i--)
         if(file_size(LOGS+file+"."+i) > 0)
           unguarded( (: rename, LOGS+file+"."+i, LOGS+file+"."+(i+1) :) );

       // rename the .log one to .1
       unguarded( (: rename, LOGS+file+".log", LOGS+file+"."+"1" :) );
     }
   } 
} /* create() */

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

/*
 * By default, use the valid_read()/valid_write() functions defined in the
 * inherited file /std/dom/base_master.c.
 * These default to reading allowed for everyone, writing only allowed
 * for independent creators and the domain lord.
 * This is all overridden by the check_permission() function
 * which is called first, which should be used instead.
 */
int valid_read(string *path, string euid, string funct);

int valid_write(string *path, string euid, string funct);

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

string query_info() {
  return "\nThe admin domain is made up of the active Trustees of "
    "Discworld.  Their role is to ensure the smooth running of the game.  "
    "They carry the ultimate responsibility for Discworld, how it operates "
    "and its policies and rules.\n\n";
}
