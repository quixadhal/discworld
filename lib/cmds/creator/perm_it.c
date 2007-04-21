/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: perm_it.c,v 1.7 2003/05/13 18:25:01 ceres Exp $
 * $Log: perm_it.c,v $
 * Revision 1.7  2003/05/13 18:25:01  ceres
 * Modified to allow multiple assignments
 *
 * Revision 1.6  2003/04/18 00:14:23  pinkfish
 * Remove the update abilioty.
 *
 * Revision 1.5  2003/04/17 20:56:20  pinkfish
 * Add in some code to update the error assignments.
 *
 * Revision 1.4  2003/03/27 22:14:01  trilogy
 * Was using /secure/login.
 *
 * Revision 1.3  2003/03/21 17:14:48  drakkos
 * Added a filter option for permit assignment summary
 *
 * Revision 1.2  2003/03/05 22:46:28  pinkfish
 * Make it sort the keys correctly and make it pick up details of domain
 * directory assignments.
 *
 * Revision 1.1  2003/03/05 18:57:00  pinkfish
 * Initial revision
 *
 * Revision 1.6  2003/03/04 23:01:49  pinkfish
 * Allow you to remove permissions too.
 *
 * Revision 1.5  2003/03/04 22:29:34  pinkfish
 * Add in directory stuff.
 *
 * Revision 1.4  1999/01/21 18:35:19  ceres
 * Forgot to remove a debug statement.
 *
 * Revision 1.2  1999/01/20 00:54:55  ceres
 * Modified add_command
 *
 * Revision 1.1  1998/12/29 06:03:04  ceres
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <access.h>
#include <player_handler.h>
#include <error_handler.h>

inherit "/cmds/base";

#define READ_MASK 1
#define WRITE_MASK 2
#define GRANT_MASK 4
#define LOCK_MASK 8

int help();

mixed cmd(string command, string euid, string path) {
  string* euids;
  string* bing;

  if (this_player() != this_player(1)) {
    return 0;
  }

  seteuid("Root");
  
  notify_fail("Something went wrong.\n");
  path = (string)this_player()->get_path(path);
  
  switch(command) {
  case "assign" :
    // Assign someone to the directory.
    if (file_size(path) != -2) {
       notify_fail("The path " + path + " does not exist.\n");
       return 0;
    }
    euids = explode(replace_string(euid, " ", ""), ",") - ({ "none" });
    bing = filter(euids, (: !PLAYER_HANDLER->test_creator($1) &&
                            $1 != ERROR_ASSIGNED_NO_ONE:));
    if (sizeof(bing)) {
       notify_fail("The people " + query_multiple_short(bing) +
                   " are not creators.\n");
       return 0;
    }
    if ((int)master()->assign_people_to_directory(path, euids)) {
       add_succeeded_mess("Assigned the directory " + path +
                         " to " + query_multiple_short(euids) + ".\n");
       return 1;
    }
    notify_fail("Unable to assign the directory, not creator names?\n");
    return 0;

  case "read":
    if (!master()->high_programmer(previous_object(-1)) &&
        !master()->valid_grant(this_player(), path, READ_MASK))
      return notify_fail("You do not have permission to add read access.\n");
    return (int)master()->add_read_permission(euid, path);
    break;

  case "write":
      if (!master()->high_programmer(previous_object(-1)) &&
          !master()->valid_grant(this_player(), path, WRITE_MASK))
        return notify_fail("You do not have permission to add write "
                           "access.\n");
      return (int)master()->add_write_permission(euid, path);
      break;

  case "grant":
    if (!master()->high_programmer(previous_object(-1)) &&
        !master()->valid_grant(this_player(), path, GRANT_MASK))
      return notify_fail("You do not have permission to add grant access.\n");
    return (int)master()->add_grant_permission(euid, path);
    break;

  case "lock":
    if (!master()->query_lord(previous_object(-1)))
      return notify_fail("You don't have permission to lock paths.\n");
    if ((path[0..2] != "/d/") &&
        !master()->high_programmer(previous_object(-1)))
      return notify_fail("You don't have permission to lock paths there.\n");
    return (int)master()->lock_path(path);

  case "noread":
    if (!master()->high_programmer(previous_object(-1)) &&
        !master()->valid_grant(this_player(), path, READ_MASK))
      return notify_fail("You do not have permission to remove read "
                         "access.\n");
    return (int)master()->remove_read_permission(euid, path);
  break;

  case "nowrite":
    if (!master()->high_programmer(previous_object(-1)) &&
        !master()->valid_grant(this_player(), path, WRITE_MASK))
      return notify_fail("You do not have permission to remove write "
                         "access.\n");
    return (int)master()->remove_write_permission(euid, path);
    break;

  case "nogrant":
    if (!master()->high_programmer(previous_object(-1)) &&
        !master()->valid_grant(this_player(), path, GRANT_MASK))
      return notify_fail("You do not have permission to remove grant "
                         "access.\n");
    return (int)master()->remove_grant_permission(euid, path);
    break;

  case "unlock":
    if (!master()->query_lord(previous_object(-1)))
      return notify_fail("You don't have permission to lock paths.\n");
    if ((path[0..2] != "/d/") &&
        !master()->high_programmer(previous_object(-1)))
      return notify_fail("You don't have permission to lock paths there.\n");
    return (int)master()->unlock_path(path);
    break;

  default:
    return help();
  }
} /* cmd() */

int do_directory_summary(string dir) {
   mapping assignments;
   mapping new_assignments;
   string* paths;
   string path;
   string ret;
   string domain;

   assignments = master()->query_directory_assignments();
   foreach (domain in master()->query_domains()) {
      ret = catch(new_assignments = ("/d/" + domain + "/master")->query_directory_assignments());
      if (sizeof(new_assignments)) {
         assignments += new_assignments;
      }
   }
   
   if (sizeof (dir)) {
     assignments = filter (assignments, (: strsrch ($1, $(dir)) != -1 :));
  }   
   //paths = sort_array( keys( assignments ), "list_before", this_object() );
   paths = sort_array( keys( assignments ), 1 );
   ret = sprintf("%40-s Assigned To\n", "Path");
   foreach (path in paths) {
      ret += sprintf("%-40s %s\n", path, query_multiple_short(assignments[path]));
   }
   write("$P$Directory Assignments$P$" + ret);
   return 1;
}

// This updates the directory assignments
int do_directory_update(string dir) {
   mapping assignments;
   mapping new_assignments;
   string* paths;
   string path;
   string ret;
   string domain;
   string* bits;

   assignments = master()->query_directory_assignments();
   foreach (domain in master()->query_domains()) {
      ret = catch(new_assignments = ("/d/" + domain + "/master")->query_directory_assignments());
      if (sizeof(new_assignments)) {
         assignments += new_assignments;
      }
   }
   
   if (sizeof (dir)) {
     assignments = filter (assignments, (: strsrch ($1, $(dir)) != -1 :));
  }   
   //paths = sort_array( keys( assignments ), "list_before", this_object() );
   paths = sort_array( keys( assignments ), 0 );
   foreach (path in paths) {
      if (sizeof(assignments[path])) {
         bits = explode(path, "/");
         if (bits[0] == "d") {
           ERROR_HANDLER->do_update_directory_assignment(("/d/" + bits[1] + "/master")->query_lord(), assignments[path][0], path, (: 1 :));
         } else {
           ERROR_HANDLER->do_update_directory_assignment("nobody", assignments[path][0], path, (: 1 :));
         }
         write("Updated " + path + " to " + assignments[path][0] + "\n");
      }
   }
   write("$P$Directory Assignments$P$" + ret);
   return 1;
}

int do_summary(string str) {
  mapping perms;
  string *paths, *euids, ret, creator;
  int i, j, k;
   
  perms = (mapping)master()->query_permissions();
  if (str) {
    if (!perms[str]) {
      if (!PLAYER_HANDLER->test_user(str) && !master()->valid_euid(str)) {
        write("There are no permissions for "+str+".\n");
        return 1;
      } else {
        creator = str; 
      }
    } else {
      perms = ([ str : perms[str] ]);
    }
  }
  //paths = sort_array( keys( perms ), "list_before", this_object() );
  paths = sort_array( keys( perms ), 1 );
  if (!sizeof(paths)) {
    ret = "No permissions set.\n";
  } else {
    ret = sprintf("%11-s    Path\n", "Euid");
  }
  for (i=0;i<sizeof(paths);i++) {
    euids = keys(perms[paths[i]]);
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
         if ( !PLAYER_HANDLER->test_creator( creator ) &&
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

int help() {
  write("Available commands:\n"
"  read    <euid> <path>     : add read permison to the path.\n"
"  write   <euid> <path>     : add write permission to the path.\n"
"  grant   <euid> <path>     : add granting privileges to the path.\n"
"  lock           <path>     : restrict access to the path.\n"
"  noread  <euid> <path>     : remove read permission from the path.\n"
"  nowrite <euid> <path>     : remove write permission from the path.\n"
"  nogrant <euid> <path>     : remove granting privileges to the path.\n"
"  unlock         <path>     : remove restrictions to path.\n"
"  summary [path|euid]       : give a list of all the read/write perms.\n"
"  tidy                      : tidy away unnecessary perms.\n"
"  assign <euid> <path>      : assigns someone to look after a directory.\n"
"  assignment summary        : summary of directory assignments.\n" );
   return 1;
}

mixed *query_patterns() {
  return ({ "summary", (: do_summary(0) :),
              "summary <string>", (: do_summary($4[0]) :),
              "assignment summary", (: do_directory_summary(0) :),
//              "assignment update", (: do_directory_update(0) :),
              "assignment summary <string'filter'>", (: do_directory_summary($4[0]) :),

              "tidy", (: do_tidy() :),
              "help", (: help() :),
              "<word'permission'> <string'euid'> <word'path'>",
              (: cmd($4[0], $4[1], $4[2] ) :)
#ifdef 0
              ,
              "write <string'euid'> <string>", (: cmd("write", $4[0], $4[1]) :),
              "grant <string'euid'> <string>", (: cmd("grant", $4[0], $4[1]) :),
              "lock <string'euid'> <string>", (: cmd("lock", $4[0], $4[1]) :),
              "noread <string'euid'> <string>", (: cmd("noread", $4[0], $4[1]) :),
              "nowrite <string'euid'> <string>", (: cmd("nowrite", $4[0], $4[1]) :),
              "nogrant <string'euid'> <string>", (: cmd("nogrant", $4[0], $4[1]) :),
              "unlock <string'euid'> <string>", (: cmd("unlock", $4[0], $4[1])
                                                 :),
#endif        
              });
} /* query_patterns() */
