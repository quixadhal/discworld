/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: meteor.c,v 1.4 1999/10/03 20:06:10 taffyd Exp $
 * $Log: meteor.c,v $
 * Revision 1.4  1999/10/03 20:06:10  taffyd
 * Fixed bugge with gender
 *
 * Revision 1.3  1998/11/17 03:59:27  pinkfish
 * Changes to work with the new player info stuff.
 *
 * Revision 1.2  1998/11/01 11:51:03  pinkfish
 * Add in fiona's meteor stuff.
 * I mean player info stuff.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <playerinfo.h>
inherit "/cmds/base";

mixed cmd(string words) {
  string player, reason;
  object ob;

  if(this_player()->query_name() == "macchirton") {
    return notify_fail("You cannot use meteor.\n");
  }
   
  if(!words || sscanf(words, "%s %s", player, reason ) != 2) {
    return notify_fail("Usage: meteor <player> <reason>\n");
  }
  ob = find_player(this_player()->expand_nickname(player));
  if(!ob) {
    return notify_fail(player+" not found!\n");
  }
  write("Warning: This is not to be used for pranks.  If you "
    "do not have a valid reason to use this, do not use it!  "
    "Are you quite sure you wish to proceed? (y|n): ");
  input_to("are_sure", ob, reason);
  return 1;
} /* cmd() */

void are_sure(string str, object ob, string reason) {
  string gender = "boy";
  if(this_player()->query_gender() == 2) {
    gender = "girl";
  }
  str = lower_case(str);
  if(str[0] != 'y') {
    write("Good little "+gender+"!  Now go play nice with "
      +ob->one_short()+".\n");
    return;
  }
  PLAYERINFO_HANDLER->add_entry(this_player(),
                                ob->query_name(),
                                "meteor",
                                reason);
  log_file("NUKE", "%s: %s nuked %s: %s\n", ctime(time()), 
    this_player()->query_short(), ob->query_short(), reason);
  shout("You see a very large %^YELLOW%^meteor%^RESET%^ up in the sky.\n");
  write("Nuking player "+ob->query_name()+".\n");
  call_out("nuke2", 2, ob);
} /* are_sure() */

void nuke2(object ob) {
  shout("The %^YELLOW%^meteor%^RESET%^ rushes towards the ground.\n");
  tell_object(ob, "It looks like it's heading for you.\n");
  call_out("nuke3", 5, ob);
} /* nuke2() */

void nuke3(object ob) {
  tell_object(ob, "The %^YELLOW%^meteor%^RESET%^ hits you ......"+
    "%^RED%^WALLOP.%^RESET%^\n");
  shout(ob->query_cap_name()+" has been struck by a meteor.\n"+
        capitalize(ob->query_pronoun())+" is no more.\n");
  ob->save();
  ob->quit();
} /* nuke3() */

