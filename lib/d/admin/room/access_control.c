/* file access control room */

#include <access.h>
#include "path.h"

inherit "/std/room/basic_room";

#define LOGIN "/secure/login"
#define READ_MASK 1
#define WRITE_MASK 2
#define GRANT_MASK 4
#define LOCK_MASK 8

int do_read(string euid, string path);
int do_write(string euid, string path);
int do_grant(string euid, string path);
int do_lock(string path);
int do_noread(string euid, string path);
int do_nowrite(string euid, string path);
int do_nogrant(string euid, string path);
int do_unlock(string path);
int do_summary(string str);
int do_tidy();

void setup() {
  set_light(100);
  set_short("file-access control room");
  add_property("determinate", "the ");
  set_long(
    "You are in a room full of filing cabinets.  There are filing "
    "cabinets everywhere, they are all packed full of papers and look "
    "in a horrible mess.\n"
"Available commands:\n"
"  read    <euid> <path>     : add read permission to the path.\n"
"  write   <euid> <path>     : add write permission to the path.\n"
"  grant   <euid> <path>     : add granting privileges to the path.\n"
"  lock           <path>     : restrict access to the path.\n"
"  noread  <euid> <path>     : remove read permission from the path.\n"
"  nowrite <euid> <path>     : remove write permission from the path.\n"
"  nogrant <euid> <path>     : remove granting privileges to the path.\n"
"  unlock         <path>     : remove restrictions to path.\n"
"  summary [path|euid]       : give a list of all the read/write perms.\n"
"  tidy                      : tidy away unnecessary perms.\n" );
  add_exit("east", ROOM+"domain_control", "corridor");
} /* setup() */

void init() {
  ::init();
  this_player()->add_command("read",    this_object(),
    "<string'euid'> <string'path'>", (: do_read($4[0], $4[1]) :) );
  this_player()->add_command("write",   this_object(), 
    "<string'euid'> <string'path'>", (: do_write($4[0], $4[1]) :) );
  this_player()->add_command("grant",   this_object(), 
    "<string'euid'> <string'path'>", (: do_grant($4[0], $4[1]) :) );
  this_player()->add_command("lock",    this_object(),
    "<string'path'>",                (: do_lock($4[0]) :) );
  this_player()->add_command("noread",  this_object(),
    "<string'euid'> <string'path'>", (: do_noread($4[0], $4[1]) :) );
  this_player()->add_command("nowrite", this_object(),
    "<string'euid'> <string'path'>", (: do_nowrite($4[0], $4[1]) :) );
  this_player()->add_command("nogrant", this_object(),
    "<string'euid'> <string'path'>", (: do_nogrant($4[0], $4[1]) :) );
  this_player()->add_command("unlock",  this_object(),
    "<string'path'>",                (: do_unlock($4[0]) :) );
  this_player()->add_command("summary", this_object(),
    "<string'path or euid'>",        (: do_summary($4[0]) :) );
  this_player()->add_command("tidy",    this_object(),
    "",                              (: do_tidy() :) );
} /* init() */

int do_read(string euid, string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_read("+euid+", "+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call do_read("+euid+", "+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!master()->high_programmer(previous_object(-1)) &&
      !master()->valid_grant(previous_object(), path, READ_MASK)) {
    notify_fail("You do not have permission to add read access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)master()->add_read_permission(euid, path);
} /* do_read() */

int do_write(string euid, string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_write("+euid+", "+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call do_write("+euid+", "+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!master()->high_programmer(previous_object(-1)) &&
      !master()->valid_grant(previous_object(), path, WRITE_MASK)) {
    notify_fail("You do not have permission to add write access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)master()->add_write_permission(euid, path);
} /* do_write() */

int do_grant(string euid, string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_grant("+euid+", "+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call do_grant("+euid+", "+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!master()->high_programmer(previous_object(-1)) &&
      !master()->valid_grant(previous_object(), path, GRANT_MASK)) {
    notify_fail("You do not have permission to add grant access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)master()->add_grant_permission(euid, path);
} /* do_grant() */

int do_lock(string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_lock("+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call do_lock("+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  if (!path)
    return notify_fail("Syntax: "+query_verb()+" <path>\n");
  if (!master()->query_lord(previous_object(-1)))
    return notify_fail("You don't have permission to lock paths.\n");
  if ((path[0..2] != "/d/") &&
      !master()->high_programmer(previous_object(-1)))
    return notify_fail("You don't have permission to lock paths there.\n");
  notify_fail("Something went wrong.\n");
  return (int)master()->lock_path(path);
}

int do_noread(string euid, string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_noread("+euid+", "+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call do_noread("+euid+", "+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!master()->high_programmer(previous_object(-1)) &&
      !master()->valid_grant(previous_object(), path, READ_MASK)) {
    notify_fail("You do not have permission to remove read access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)master()->remove_read_permission(euid, path);
} /* do_noread() */

int do_nowrite(string euid, string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_nowrite("+euid+", "+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
                 "to call do_nowrite("+euid+", "+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!master()->high_programmer(previous_object(-1)) &&
      !master()->valid_grant(previous_object(), path, WRITE_MASK)) {
    notify_fail("You do not have permission to remove write access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)master()->remove_write_permission(euid, path);
} /* do_nowrite() */

int do_nogrant(string euid, string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_nogrant("+euid+", "+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call do_nogrant("+euid+", "+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!master()->high_programmer(previous_object(-1)) &&
      !master()->valid_grant(previous_object(), path, GRANT_MASK)) {
    notify_fail("You do not have permission to remove grant access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)master()->remove_grant_permission(euid, path);
} /* do_nogrant() */

int do_unlock(string path) {
  if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
    event(users(), "inform", "Illegal attempt to call do_unlock("+path+") by "+
          this_player()->one_short(), "cheat");
    unguarded((: write_file, "/log/CHEAT", ctime(time())+": Illegal attempt "
               "to call do_unlock("+path+").\nBacktrace: "+ back_trace() :));
    return 0;
  }
  if (!path)
    return notify_fail("Syntax: "+query_verb()+" <path>\n");
  if (!master()->query_lord(previous_object(-1)))
    return notify_fail("You don't have permission to lock paths.\n");
  if ((path[0..2] != "/d/") &&
      !master()->high_programmer(previous_object(-1)))
    return notify_fail("You don't have permission to lock paths there.\n");
  notify_fail("Something went wrong.\n");
  return (int)master()->unlock_path(path);
}

int list_before( string first, string second ) {
   if ( first < second )
      return -1;
   if ( first > second )
      return 1;
   return 0;
} /* list_before() */

int do_summary(string str) {
  mapping perms;
  string *paths, *euids, ret, creator;
  int i, j, k;
   
  perms = (mapping)master()->query_permissions();
  if (str) {
    if (!perms[str]) {
      if (!LOGIN->test_user(str) && !master()->valid_euid(str)) {
        write("There are no permissions for "+str+".\n");
        return 1;
      } else {
        creator = str; 
      }
    } else {
      perms = ([ str : perms[str] ]);
    }
  }
  paths = sort_array( m_indices( perms ), "list_before", this_object() );
  if (!sizeof(paths))
    ret = "No permissions set.\n";
  else
    ret = sprintf("%11-s    Path\n", "Euid");
  for (i=0;i<sizeof(paths);i++) {
    euids = m_indices(perms[paths[i]]);
    for (j=0;j<sizeof(euids);j++) {
      if (!creator || (euids[j] == creator ) ) {
        k = perms[paths[i]][euids[j]];
        if (k & LOCK_MASK)
          ret += sprintf("%11-s LCK %s\n", euids[j], paths[i]);
        else
          ret += sprintf("%11-s %c%c%c %s\n", euids[j],(k & READ_MASK?'R':' '),
                         (k & WRITE_MASK?'W':' '), (k & GRANT_MASK?'G':' '),
                         paths[i]);
      }
    }
  }
  this_player()->more_string( ret, "Permissions", 1 );
  return 1;
} /* do_summary() */

int do_tidy() {
   int i, j, perm, same;
   string path, creator, *bits;
   mapping perms, euids, others;

   if (!sizeof(filter(previous_object(-1), (: interactive($1) :)))) {
      event( users(), "inform", "illegal attempt to call do_tidy() by "+
            (string)this_player()->query_name(), "cheat" );
      unguarded((: write_file, "/log/CHEAT", ctime( time() ) +": illegal "
                   "attempt to call do_tidy() by "+
                   (string)this_player()->query_name() +"\n" :));
      return notify_fail( "Failed.\n" );
   }
   perms = (mapping)master()->query_permissions();
   foreach( path in keys( perms ) ) {
      euids = perms[ path ];
      foreach( creator in keys( euids ) ) {
         perm = euids[ creator ];
         if ( !LOGIN->test_creator( creator ) &&
              !master()->valid_euid( creator ) ) {
            write( "No creator: "+ creator +".\n" );
            if ( perm & READ_MASK )
               master()->remove_read_permission( creator, path );
            if ( perm & WRITE_MASK )
               master()->remove_write_permission( creator, path );
            if ( perm & GRANT_MASK )
               master()->remove_grant_permission( creator, path );
            continue;
         }
         if ( path == "/" )
            continue;
         same = perms[ "/" ][ creator ] & perm;
         if ( same ) {
            write( "Access to / supercedes "+ path +" for "+ creator +".\n" );
            if ( same & READ_MASK )
               master()->remove_read_permission( creator, path );
            if ( same & WRITE_MASK )
               master()->remove_write_permission( creator, path );
            if ( same & GRANT_MASK )
               master()->remove_grant_permission( creator, path );
            continue;
         }
         bits = explode( path, "/" );
         j = sizeof( bits ) - 1;
         for ( i = 0; i < j; i++ ) {
            others = perms[ "/"+ implode( bits[ 0 .. i ], "/" ) ];
            if ( !others )
               continue;
            same = others[ creator ] & perm;
            if ( same ) {
               write( "Access to /"+ implode( bits[ 0 .. i ], "/" ) +
                     " supercedes "+ path +" for "+ creator +".\n" );
               if ( same & READ_MASK )
                  master()->remove_read_permission( creator, path );
               if ( same & WRITE_MASK )
                  master()->remove_write_permission( creator, path );
               if ( same & GRANT_MASK )
                  master()->remove_grant_permission( creator, path );
               break;
            }
         }
      }
   }
   return 1;
} /* do_tidy() */
