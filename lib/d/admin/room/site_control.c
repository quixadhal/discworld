#include "path.h"
/* domain administration room */
#include "access.h"
inherit "/std/room/basic_room";
#define MASTER "/secure/master"

int do_banish(string);
int do_access(); /* Show the current access list. */
int do_suspend(string);
int do_unsuspend(string);


void setup() {
//  string *doms, com;
//  int i;
  
  set_light(100);
  set_short("site-access control room");
  add_property("determinate", "the ");
  set_long(
    "In front of you teem hundreds of small packages.  They rush around "+
    "yelling out strange numbers and get directed into various parts "+
    "of the maze by large mice with red flags.  Everything happens at "+
    "break neck speed.  Every now and then one of the packages "+
    "spontaneously combusts.\n"+
"Available commands:\n"+
"  ban       :  Ban a site.\n"+
"  access    :  Show the current access of various sites.\n"+
"  suspend   :  Suspend someones access.\n"+
"  unsuspend :  Unsuspend someones access.\n");

  seteuid("Admin");

  add_exit("north", ROOM+"domain_control", "corridor");
} /* setup() */

#if efun_defined(add_action)
#define NUM
#else
#define NUM 1
#endif

void init() {
  ::init();
  if (!interactive(previous_object(NUM))) return 0;
  if ("/secure/master"->query_lord(geteuid(previous_object(NUM))))
    add_command("ban", "<string>", (:do_banish($4[0]):));
  add_command("access", "", (:do_access():));
  add_command("suspend", "<string'person'>", (:do_suspend($4[0]):));
  add_command("unsuspend", "<string'person'>", (:do_unsuspend($4[0]):));
} /* init() */

int print_access(string bit, mapping bing, int depth, int cols) {
  mixed *bits;
  int i;

  if (this_player() != this_player(1))
    return 0;
  bits = m_indices(bing);
  if (depth == 4) {
/* Do the ident printing... */
    for (i=0;i<sizeof(bits);i++)
      switch (bing[bits[i]][0]) {
        case NO_NEW :
          printf("%s@%-=*s", bits[i], cols - strlen(bits[i]), bit +
                             " set to no new characters.\n");
          break;
        case NO_ACCESS :
          printf("%s@%-=*s", bits[i], cols - strlen(bits[i]), bit +
                             " set to no characters.\n");
          break;
        case ACCESS :
          printf("%s@%-=*s", bits[i], cols - strlen(bits[i]), bit +
                             " set to normal access.\n");
          break;
    }
    return 1;
  }
  for (i=0;i<sizeof(bits);i++)
    print_access(bit+"."+bits[i], bing[bits[i]], depth+1, cols);
  return 1;
} /* print_access() */

int do_access() {
  mixed bing;

  if (this_player() != this_player(1))
    return 0;
  bing = (mixed)"/secure/bastards"->query_all_access();
  if (!m_sizeof(bing)) {
    notify_fail("No access control defined.\n");
    return 0;
  }

  write("Current access list:\n");
  print_access("", bing, 0, (int)this_player()->query_cols());
  return 1;
} /* do_access() */

int do_banish(string str) {
  string ip, ident, reason;
  int level;

  if (this_player() != this_player(1))
    return 0;
  if (!"/secure/master"->high_programmer(geteuid(previous_object())))
    return 0;
  notify_fail("Syntax: banish <ip number> <ident> <level> <reason>\n"+
              "        <level> := 0 delete ident, 1 normal access,\n"+
              "                   2 no new players, 3 No access.\n");
  if (!str)
    return 0;
  if (sscanf(str, "%s %s %d %s", ip, ident, level, reason) != 4)
    return 0;

  if (!"/secure/bastards"->change_access(explode(ip, "."), ident, level,
                                         reason))
    return 0;
  write("Access permisions changed.\n");
  return 1;
} /* do_banish() */

int do_suspend(string str) {
  string reason;
  string name;
  int tim;

  if (this_player() != this_player(1))
    return 0;
  if (!"/secure/master"->query_lord(geteuid(previous_object())))
    return 0;
  if (!str || sscanf(str, "%s %d %s", name, tim, reason) != 3) {
    notify_fail("Syntax: suspend <name> <time> <reason>\nWhere time is in hours.\n");
    return 0;
  }
  notify_fail("We failed!\n");
  if (!"/secure/bastards"->suspend_person(name, tim*60*60, reason))
    return 0;
  write(name+" suspended until "+ctime(time()+tim*60*60)+"\n");
  return 1;
} /* do_suspend() */

int do_unsuspend(string str) {
  if (this_player() != this_player(1))
    return 0;
  if (!"/secure/master"->query_lord(geteuid(previous_object())))
    return 0;
  notify_fail("We failed!\n");
  if (!"/secure/bastards"->unsuspend_person(str))
    return 0;
  write(str+" unsuspended.\n");
  return 1;
} /* do_unsuspend() */
