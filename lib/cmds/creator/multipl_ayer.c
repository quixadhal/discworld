/*  -*- LPC -*-  */
/*
 * $Id: multipl_ayer.c,v 1.9 2003/03/25 09:42:40 wyvyrn Exp ceres $
 */

/* multiplayer command to add/delete names to the list
 * of checked players and enter multiplay events into playerinfos
 */
#include <creator.h>
#include <player.h>

inherit "/cmds/base";

varargs int cmd(string command, string name, string what, string extra) {
  switch (command) {
  case "add":
    if (!call_other(PLAYER_MULTIPLAYER_HANDLER, "add_" + what, name, extra)) {
      printf("\"%s\"%s isn't known here.\n", name, (extra ? " and/or \"" + 
                extra + "\"" : ""));
    } else {
      if (what == "allowed") {
        printf("\"%s\" and \"%s\" added as allowed together.\n", name, extra);
      } else {
        printf("\"%s\" added.\n", name);
      }
    }
    break;
  case "del":
    if(!call_other(PLAYER_MULTIPLAYER_HANDLER, "delete_" + what, name)) {
      printf("\"%s\" isn't on the list.\n", name);
    } else {
      printf("\"%s\" deleted.\n", name);
    }
    break;
  case "log":
    PLAYER_MULTIPLAYER_HANDLER->mark_log(name, extra);
    if (extra) {
      printf("Added \"multiplay\" playerinfo event for \"%s\": \"%s\".\n",
           name, extra);
    } else {
      printf("Marked \"multiplay\" playerinfo events for \"%s\" as handled.\n", 
           name);
    }
    break;
  case "list":
    write("The list currently consists of: $I$5=\n" +
          query_multiple_short(sort_array(call_other(PLAYER_MULTIPLAYER_HANDLER,
                                                     "query_" + what),
                                          (: strcmp($1, $2) :)))
          + "$I$0=\n");
    break;
  case "site":
    if(find_player(name)) {
      name = query_ip_number(find_player(name));
    }
    if("/secure/bastards"->change_multi(name, 1, time() + 3600)) {
      write("Site " + name + " has been permitted for multiplayers "
            "for the next hour.\n");
    } else {
      write("Error permitting site " + name + " for multiplayers.\n");
    }
    break;
  default:
    write("Unknown option.\n");
  }
  return 1;
} /* cmd */

mixed *query_patterns() {
  return ({ "[list]",  (: cmd("list", 0, "gits") :),
              "{list allow}", (: cmd("list", 0, "allowed") :),
            "add git <word'name'>", (: cmd("add", $4[0], "git") :),
            "add allowed <word'first name'> <word'second name'>",
            (: cmd("add", $4[0], "allowed", $4[1]) :),
            "allow site <word'address|name'>", (: cmd("site", $4[0], "allowed") :),
            "del allowed <word'name'>", (: cmd("del", $4[0], "allowed") :),
            "log <word'name'>", (: cmd("log", $4[0], "git") :),
            "log <word'name'> <string'log text'>", (: cmd("log", $4[0], "git",
                                                       implode($4[1..], " ")) :)
         });
}
