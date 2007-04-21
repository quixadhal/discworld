/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: setmin.c,v 1.2 1998/04/14 02:40:30 pinkfish Exp $
 * $Log: setmin.c,v $
 * Revision 1.2  1998/04/14 02:40:30  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#define TP this_player()

mixed cmd(string str) {
  if(TP->query_creator() || TP->query_property("setm")) {
    str = str + " ";
    if(sizeof(explode("^"+str, "$N")) < 2 ||
       sizeof(explode("^"+str, "$F")) < 2)
      return notify_fail("Must have a $N and a $F in your entry message.\n");
    TP->set_msgin(str);
    write("Ok.\n");
    return 1;
  }
  return notify_fail("You are not allowed that power yet.\n");
} /* cmd() */

mixed *query_patterns() {
   return ({ "<string>", (: cmd($4[0]) :) });
} /* query_patterns() */
