/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unsuspend.c,v 1.1 2001/03/12 05:08:14 ceres Exp $
 * $Log: unsuspend.c,v $
 * Revision 1.1  2001/03/12 05:08:14  ceres
 * Initial revision
 *
 * Revision 1.1  1998/12/29 06:03:35  ceres
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <access.h>

inherit "/cmds/base";

mixed cmd(string str) {
  if (this_player() != this_player(1))
    return 0;
  if (!"/secure/master"->query_lord(geteuid(previous_object())) &&
      !"/d/liaison/master"->query_deputy(geteuid(previous_object())))
    return notify_fail("You must be a lord or liaison deputy to unsuspend "
		       "someone.");

  notify_fail("We failed!\n");
  if (!"/secure/bastards"->unsuspend_person(str))
    return 0;
  write(str+" unsuspended.\n");
  return 1;
} /* cmd() */

int help() {
  return notify_fail("Syntax: unsuspend <name>.\n");
}

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :), });
} /* query_patterns() */
