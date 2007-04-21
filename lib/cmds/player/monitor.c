/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: monitor.c,v 1.5 2002/08/04 08:09:43 ceres Exp $
 * $Log: monitor.c,v $
 * Revision 1.5  2002/08/04 08:09:43  ceres
 * Modified to use options.h for monitor options.
 *
 * Revision 1.4  2002/08/03 19:29:15  ceres
 * New combat system version
 *
 * Revision 1.3  2001/06/01 20:58:35  ceres
 * Can't remember
 *
 * Revision 1.2  1998/04/14 02:24:30  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
 */
#include <cmds/options.h>

inherit "/cmds/base";

#define TP this_player()

mixed cmd(string str) {
  int monitor;

  monitor = TP->query_monitor();
  
  if(!str) {
    write("Your hit point monitor is set to " + MONITOR_OPTIONS[monitor] +
          ".\n");
    return 1;
  }

  if(member_array(str, MONITOR_OPTIONS) != -1) {
    write("Your hit point monitor is set to " + str + ".\n");
    TP->set_monitor(member_array(str, MONITOR_OPTIONS));
  }
  return 1;
}

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "{" + implode(MONITOR_OPTIONS, "|") + "}", (: cmd($4[0]) :) });
} /* query_patterns() */
