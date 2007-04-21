/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: status.c,v 1.2 2000/11/08 20:08:54 pinkfish Exp $
 * $Log: status.c,v $
 * Revision 1.2  2000/11/08 20:08:54  pinkfish
 * Fix up the status command to use add_command.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* status command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

mixed cmd(string str) {
  if (str)
    this_player()->more_string(mud_status(1), "Status info");
  else
    printf("%s", mud_status());
  return 1;
}

mixed* query_patterns() {
   return ({ "", (: cmd(0) :),
             "detailed", (: cmd("detailed") :) });
} /* query_patterns() */
