/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: echo.c,v 1.1 1998/01/06 05:28:43 ceres Exp $
 * $Log: echo.c,v $
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#include <player.h>
#include <drinks.h>

inherit "/cmds/base";

#define TP this_player()

void my_mess(string fish, string erk);

mixed cmd(string str) {
  if(!TP->query_creator() && !TP->query_property("echo"))
    return notify_fail(NOT_ALLOWED);
  
  if (!str || str == "")
    return notify_fail("Syntax : echo <text>\n");

  if (!TP->query_creator() && TP->adjust_sp(-ECHO_COST) < 0)
    return notify_fail(NO_POWER);
  
  if (TP->query_volume(D_ALCOHOL))
    str = TP->drunk_speech(str);
  log_file("ECHOS", ctime(time())+" "+this_player()->query_name()+" echos: "+
     str+"\n");
  str += "%^RESET%^";
  my_mess("You echo: ", str);
  TP->comm_event(environment(TP), "player_echo", str + "\n");
  return 1;
}

void my_mess(string fish, string erk) {
  if(!interactive(TP)) return;
  printf("%s%-=*s\n", fish, (int)TP->query_cols()-strlen(fish), 
          TP->fix_string(erk));
} /* my_mess() */
