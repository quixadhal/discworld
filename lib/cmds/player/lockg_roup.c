/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: lockg_roup.c,v 1.1 1998/01/06 05:29:43 ceres Exp $
 * $Log: lockg_roup.c,v $
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "cmds/base";

#define TEAM_HANDLER "/obj/handlers/team"

mixed cmd(string str) {
  object owner;
  int locked;

   str = TEAM_HANDLER->query_group(this_player());
   if(!str)
      return notify_fail("You are not a member of any group.\n");

  owner = TEAM_HANDLER->query_owner(str);
  if(!owner || owner != this_player())
    return notify_fail("Sorry, you are not the owner of "+str+".\n");

  locked = TEAM_HANDLER->query_locked(str);

  if(locked) {
    TEAM_HANDLER->set_locked(str, 0);
    write("Team "+str+" unlocked for new members.\n");
  } else {
    TEAM_HANDLER->set_locked(str, 1);
    write("Team "+str+" locked to new members.\n");
  }

  return 1;
}
