/* domain administration room */
#include "access.h"
#include "path.h"
inherit "/std/room";
#define MASTER "/secure/master"

void setup() {
  string *doms, com;
  int i;
  
  set_light(100);
  set_short("File access control room");
  set_long(
    "You are in a room full of filling cabinets.  There are filing "+
    "cabinets everywhere, they are all packed full of papers and look "+
    "in a horrible mess.\n"+
"Available commands:\n"+
"  read <creator> <path>        : add read permison to the path.\n" +
"  write <creator> <path>       : add write permission to the path.\n" +
"  grant <creator> <path>       : add granting privilages to the path.\n" +
"  noread <creator> <path>      : remove read permission from the path.\n" +
"  nowrite <creator> <path>     : remove write permission from the path.\n" +
"  nogrant <creator> <path>     : remove granting privilages to the path.\n" +
"  summary [path]               : give a list of all the read/write perms.\n");
  add_exit("east", ROOM+"domain_control", "corridor");
  seteuid("Admin");
} /* setup() */

void init() {
  ::init();
  add_action("do_read", "read");
  add_action("do_write", "write");
  add_action("do_grant", "grant");
  add_action("do_noread", "noread");
  add_action("do_nowrite", "nowrite");
  add_action("do_nogrant", "nogrant");
  add_action("do_summary", "summary");
} /* init() */

int do_read(string str) {
  string euid, path, eu;

  if (this_player() != this_player(1))
    return 0;
  if (!str || sscanf(str, "%s %s", euid, path) != 2) {
    notify_fail("Syntax: "+query_verb()+" <creator> <path>\n");
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!"/secure/master"->high_programmer((eu = geteuid(previous_object()))) &&
      !"/secure/master"->check_permission(eu, path, 4)) {
    notify_fail("You do not have permission to add read access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)"/secure/master"->add_read_permission(euid, path);
} /* do_read() */

int do_write(string str) {
  string euid, path, eu;

  if (this_player() != this_player(1))
    return 0;
  if (!str || sscanf(str, "%s %s", euid, path) != 2) {
    notify_fail("Syntax: "+query_verb()+" <creator> <path>\n");
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!"/secure/master"->high_programmer((eu = geteuid(previous_object()))) &&
      !"/secure/master"->check_permission(eu, explode(path,"/"), 4)) {
    notify_fail("You do not have permission to add write access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)"/secure/master"->add_write_permission(euid, path);
} /* do_write() */

int do_grant(string str) {
  string euid, path, eu;

  if (this_player() != this_player(1))
    return 0;
  if (!str || sscanf(str, "%s %s", euid, path) != 2) {
    notify_fail("Syntax: "+query_verb()+" <creator> <path>\n");
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!"/secure/master"->high_programmer((eu = geteuid(previous_object()))) &&
      !"/secure/master"->check_permission(eu, path, 4)) {
    notify_fail("You do not have permission to add grant access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)"/secure/master"->add_grant_permission(euid, path);
} /* do_grant() */

int do_noread(string str) {
  string euid, path, eu;

  if (this_player() != this_player(1))
    return 0;
  if (!str || sscanf(str, "%s %s", euid, path) != 2) {
    notify_fail("Syntax: "+query_verb()+" <creator> <path>\n");
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!"/secure/master"->high_programmer((eu = geteuid(previous_object()))) &&
      !"/secure/master"->check_permission(eu, path, 4)) {
    notify_fail("You do not have permission to remove read access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)"/secure/master"->remove_read_permission(euid, path);
} /* do_noread() */

int do_nowrite(string str) {
  string euid, path, eu;

  if (this_player() != this_player(1))
    return 0;
  if (!str || sscanf(str, "%s %s", euid, path) != 2) {
    notify_fail("Syntax: "+query_verb()+" <creator> <path>\n");
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!"/secure/master"->high_programmer((eu = geteuid(previous_object()))) &&
      !"/secure/master"->check_permission(eu, path, 4)) {
    notify_fail("You do not have permission to remove write access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)"/secure/master"->remove_write_permission(euid, path);
} /* do_nowrite() */

int do_nogrant(string str) {
  string euid, path, eu;

  if (this_player() != this_player(1))
    return 0;
  if (!str || sscanf(str, "%s %s", euid, path) != 2) {
    notify_fail("Syntax: "+query_verb()+" <creator> <path>\n");
    return 0;
  }
  path = (string)this_player()->get_path(path);
  if (!"/secure/master"->high_programmer((eu = geteuid(previous_object()))) &&
      !"/secure/master"->check_permission(eu, path, 4)) {
    notify_fail("You do not have permission to remove grant access.\n");
    return 0;
  }
  notify_fail("Something went wrong.\n");
  return (int)"/secure/master"->remove_grant_permission(euid, path);
} /* do_nogrant() */

int do_summary(string str) {
  mapping perms;
  string *paths, *euids, ret;
  int i, j, k;

  if (this_player() != this_player(1))
    return 0;
  perms = (mapping)"/secure/master"->query_permissions();
  if (str) {
    if (!perms[str]) {
      write("There are no permissions for "+str+".\n");
      return 1;
    }
    perms = ([ str : perms[str] ]);
  }
  paths = m_indices(perms);
  if (!sizeof(paths))
    ret = "No permissions set.\n";
  else
    ret = sprintf("%11-s    Path\n", "Euid");
  for (i=0;i<sizeof(paths);i++) {
    euids = m_indices(perms[paths[i]]);
    for (j=0;j<sizeof(euids);j++)
      ret += sprintf("%11-s %c%c%c %s\n", euids[j],
             ((k=perms[paths[i]][euids[j]])&1?'R':' '),
             (k&2?'W':' '), (k&4?'G':' '), paths[i]);
  }
  this_player()->more_string(ret);
  return 1;
} /* do_summary() */
