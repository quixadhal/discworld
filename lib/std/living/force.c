/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: force.c,v 1.4 1999/12/13 04:13:54 taffyd Exp $
 * $Log: force.c,v $
 * Revision 1.4  1999/12/13 04:13:54  taffyd
 * Fixed up the force check.
 *
 * Revision 1.3  1998/08/31 11:00:05  terano
 * Fixed a runtime and made parts of it a bit nicer :)
 *
 * Revision 1.2  1998/08/28 04:51:15  pinkfish
 * Make the force stuff use add_command and the new defines in player.h
 *
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 * 
*/
/**
 * This file contains the code to handle forcing.  
 * The method used will be to ask the object to
 * do the command.  While this is similar to the original method of doing this
 * it will be used to be more in the style of asking things to do things,
 * rather than the making things do things without asking them first.
 * @author Furball
 */

#include <player.h>

#define FORCE_COST 1
#define ILLEGAL_FORCE ({ "alias", "unalias", "mv", "mkdir", "call", "rm", \
                         "force", "kill", "gauge", "exec", "promote", \
                         "new_domain", "rmdir", "cd", "history", "echoall", \
                         "shout" })

nosave string fname;
nosave int no_force_me;

protected int no_force(string str);
protected int do_force(object *obs, string str);

/**
 * These are the commands which will allow people to force others.
 * @see no_force()
 * @see do_force()
 */
void force_commands() {
  fname = base_name(this_object());
  if ((!this_object()->query_property("npc") &&
      master()->query_senior( geteuid(this_object()) ) ) || fname == LORD_OB) {
    add_command("force", "<indirect:distant-living> <string'command'>",
                (: do_force($1, $4[1]) :));
 }
  if (fname == LORD_OB) {
    add_command("noforce", "{on|off}", (: no_force($4[0]) :));
    add_command("noforce", "", (: no_force(0) :));
  }
} /* force_commands() */

/**
 * This method is used to set the ability to let yourself be forced on and
 * off.
 * @param str the string which is on, off or 0
 * @see force_commands()
 */
protected int no_force(string str) {
  if (str == "on") {
    no_force_me = 1;
    write("Ok, forcing you disabled.\n");
    return 1;
  }
  if (str == "off") {
    no_force_me = 0;
    write("Ok, forcing you enabled.\n");
    return 1;
  }
  write("Forcing you is currently ");
  if (no_force_me) {
    write("disabled.\n");
  }
  else {
    write("enabled.\n");
  }
  return 1;
} /* no_force() */

/**
 * This is the function which forces another player to do something.
 * @see do_force_on_me()
 * @see force_commands()
 */
protected int do_force(object *obs, string str) {
  object ob;

#ifdef PLAYERS_CAN_FORCE
  if (fname == PLAYER_OB && !this_player()->query_property("force")) {
    notify_fail("You do not have the ability to do that yet.\n");
    return 0;
  }
  if (this_player()->adjust_social_points(-FORCE_COST) < 0) {
    notify_fail("Insufficient social points.\n");
    return 0;
  }
#endif

  foreach( ob in obs ) {
    if(!ob->do_force_on_me( str )) {
      write(capitalize( ob->query_name() ) + " didn't want to do that.\n");
    }
  }
  write("Ok.\n");
  return 1;
} /* do_force() */

/**
 * This is the method called to do the actual force.
 * @param str the string to force us to do
 * @see do_force()
 */
nomask int do_force_on_me(string str) {
  string temp1, temp2;
  object forcer;

  forcer = this_player(1);
  if (!forcer || (this_player() != previous_object())) {
    if (interactive(previous_object())) {
      forcer = previous_object();
    } else {
      log_file("FORCE", "Attempted illegal force from "+ 
               file_name(previous_object())+"\n");
      return 0;
    }
  }

  log_file("FORCE", ctime(time()) + " " + forcer->query_name() + " " +
                    this_object()->query_name() + " : " + str);
  tell_object(this_object(), forcer->query_cap_name() +
        " tries to force you to " + str + "\n");
  if (no_force_me || (string)this_object()->query_name() == "pinkfish") {
    event(users(), "inform", forcer->query_cap_name()+" forces "+
          this_object()->query_name()+" to "+str+" (failed)", "force");
    log_file("FORCE", " (failed)\n");
    return 0;
  }
  if (sscanf(str, "%s %s", temp1, temp2) != 2) {
    temp1 = str;
  }
  /* Delete this at your own peril */
  if (member_array(temp1, ILLEGAL_FORCE) != -1 &&
      !master()->high_programmer(geteuid(forcer))) {
    event(users(), "inform", forcer->query_cap_name()+" forces "+
          this_object()->query_name()+" to "+str+" (failed)", "force");
    log_file("FORCE", " (failed)\n");
    return 0;
  }
  if (function_exists("eventForce", this_object())) {
    command(str);
  } else {
    this_object()->eventForce(str);
  }
  event(users(), "inform", forcer->query_cap_name()+" forces "+
        this_object()->query_name()+" to "+str, "force");
  log_file("FORCE", " (succeeded)\n");
  return 1;
} /* do_force_on_me() */
