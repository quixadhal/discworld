/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: force.c,v 1.2 2000/06/23 03:40:56 pinkfish Exp $
 * $Log: force.c,v $
 * Revision 1.2  2000/06/23 03:40:56  pinkfish
 * Fix up the references to find_match.
 *
 * Revision 1.1  1998/01/06 05:08:31  ceres
 * Initial revision
 * 
*/
/* code to handle forcing.   The method used will be to ask the object to
 * do the command.  While this is similar to the original method of doing this
 * it will be used to be more in the style of asking things to do things,
 * rather than the making things do things without asking them first. */

#include <obj_parser.h>

#define FORCE_COST 1
#define ILLEGAL_FORCE ({ "alias", "unalias", "mv", "mkdir", "call", "rm", \
                         "force", "kill", "gauge", "exec", "promote", \
                         "new_domain", "rmdir", "cd", "history", "echoall", "shout" })

nosave string fname;
nosave int no_force_me;


void force_commands() {
  sscanf(file_name(this_object()), "%s#", fname);
  if(!this_object()->query_property("npc") &&
     this_object()->query_property("force") || fname == "/global/lord")
    add_action("do_force", "force");
  if(fname == "/global/lord")
    add_action("no_force", "noforce");
}

protected int no_force(string str) {
  if(fname!="/global/lord")
    return 0;
  if(str == "on") {
    no_force_me = 1;
    write("Ok.\n");
    return 1;
  }
  if(str == "off") {
    no_force_me = 0;
    write("Ok.\n");
    return 1;
  }
  write("Forcing you is currently ");
  if(no_force_me) {
    write("disabled.\n");
  }
  else {
    write("enabled.\n");
  }
  return 1;
}

int do_force(string str) {
  string who, what;
  object *obs, ob;
  int i;
  class obj_match omatch;

  if(fname == "/global/player" && !this_player()->query_property("force")) {
    notify_fail("You do not have the ability to do that yet.\n");
    return 0;
  }
  if(!str || sscanf(str, "%s %s", who, what) != 2) {
    notify_fail("Usage : force <person> <command>\n");
    return 0;
  }
  if(this_player()->adjust_social_points(-FORCE_COST) < 0) {
    notify_fail("Insufficient social points.\n");
    return 0;
  }
  who = (string)this_player()->expand_nickname(who);
  who = lower_case(who);
  omatch = match_objects_in_environments(who, environment(this_object()));
  if (omatch->result != OBJ_PARSER_SUCCESS) {
    notify_fail(match_objects_failed_mess(omatch));
    return 0;
  }
  obs = omatch->objects;
  if (!sizeof(obs))
    if (!(ob = find_living(who))) {
      notify_fail("No such living thing.\n");
      return 0;
    } else
      obs += ({ ob });
  for (i=0;i<sizeof(obs);i++)
    if(!obs[i]->do_force_on_me(what))
      write(capitalize(who) + " didn't want to do that.\n");
  write("Ok.\n");
  return 1;
}

int do_force_on_me  (string str) {
  string temp1, temp2;
  object forcer;

  forcer = this_player(1);
  if(!forcer) {
    forcer = previous_object();
  }
  if(!this_object()->query_property("npc")) {
    log_file("FORCE", ctime(time()) + " " + forcer->query_name() + " " +
                      this_object()->query_name() + " : " + str);
  }
  tell_object(this_object(), forcer->query_cap_name() +
        " tries to force you to " + str + "\n");
  if (no_force_me || (string)this_object()->query_name() == "pinkfish") {
    log_file("FORCE", " (failed)\n");
    return 0;
  }
  if(!sscanf(str, "%s %s", temp1, temp2)) {
    temp1 = str;
  }
/* Delete this at your own peril */
  if (member_array(temp1, ILLEGAL_FORCE) != -1 &&
      !"secure/master"->high_programmer(geteuid(forcer))) {
    log_file("FORCE", " (failed)\n");
    return 0;
  }
  command(str);
  log_file("FORCE", " (succeeded)\n");
  return 1;
}
