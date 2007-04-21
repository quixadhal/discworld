/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: su.c,v 1.7 2003/01/24 02:00:47 ceres Exp $
 * 
*/
inherit "/cmds/base";

#include <login_handler.h>;
#define TEAM_HANDLER "/obj/handlers/team"

#define TP this_player()

mixed cmd(string str) {
  string from;
  
  if (!str || str == "")
    return notify_fail("Usage: su <name>\n");
  if ( TP->query_auto_loading() ) {
    printf("You cannot quit yet: your inventory is still being generated.\n" );
    return 1;
  }

  if ( find_object( "/obj/shut" )) {
    write("You cannot su during a reboot.\n");
    return 1;
  }
  
  if(sizeof(TP->query_attacker_list())) {
    write("You cannot quit while in combat.\n");
    return 1;
  }

  if (TEAM_HANDLER->query_group(this_player()))  {
    "/cmds/player/leaveg_roup"->cmd(0);
//    catch(command("leavegroup"));  This doesn't work.  "command" assumes this_object() is the issuer
  }

  if (environment(this_player()) == find_object("/room/departures"))  {
    write("You can't use su in the departure lounge.\n");
    return 1;
  }

  from = TP->query_name();
  clone_object("/secure/login")->do_su(str);

  seteuid( "Root" );
  if(str == from)
    LOGIN_HANDLER->write_log("Su", TP->query_name(),
                             TP->query_property("guest"), "(to self)");
  else
    LOGIN_HANDLER->write_log("Su", from, TP->query_property("guest"), str);
  return 1;
} /* do_su() */

mixed* query_patterns() {
   return ({ "<string'new name'>", (: cmd($4[0]) :) });
}
