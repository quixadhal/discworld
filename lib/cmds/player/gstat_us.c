/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gstat_us.c,v 1.3 1999/11/12 19:50:24 tape Exp $
 * $Log: gstat_us.c,v $
 * Revision 1.3  1999/11/12 19:50:24  tape
 * + disabled the command as it's no longer used
 *
 * Revision 1.2  1998/09/27 20:26:46  ceres
 * Added locked to status.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "cmds/base";

#define TEAM_HANDLER "/obj/handlers/team"

mixed cmd(string str) {
  string group;
  string locked;
  object member;
  
  // Disabled by Tape.
  return notify_fail( "This command is no longer used.  Please use the "
   "\"group\" command from now on.  Also have a read of \"help "
   "grouping\".\n" );

  group = TEAM_HANDLER->query_group(this_player());
  if(!group || !TEAM_HANDLER->query_members(group))
    return notify_fail("You are not a member of any group.\n");

  if(TEAM_HANDLER->query_locked(group))
    locked = "(locked) ";
  else
    locked = "";
  
  printf("Members of %s%s:\n", locked, group);
  printf("%-12s %|20s   Idle\n", "Name", "Health");
  foreach(member in TEAM_HANDLER->query_members(group)) {
    if(member && interactive(member))
      printf("%-12s %-20s %3d:%02d\n", member->short(),
       member->health_string(), (query_idle(member)/60),
       (query_idle(member)%60));
    else if(member)
      printf("%s\n", member->short());
  }
  return 1;
}
