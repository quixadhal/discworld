/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unignore.c,v 1.2 2002/09/21 05:21:40 presto Exp $
 * $Log: unignore.c,v $
 * Revision 1.2  2002/09/21 05:21:40  presto
 * Made case-insensitive
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <drinks.h>
#include <language.h>
#include <player.h>

#define TP this_player()

inherit "/cmds/base";

int cmd(string str) {
  string *ignoring;
  
  ignoring = TP->query_property("ignoring");
  if(!ignoring)
    ignoring = ({ });

  if(!str)
    return notify_fail("unignore who?\n");

  str = lower_case(TP->expand_nickname(str));

  if(str == "all") {
    TP->remove_property("ignoring");
    write("You are no longer ignoring anyone.\n");
    return 1;
  }
  
  if(member_array(str, ignoring) == -1) {
    write("You are not ignoring "+str+".\n");
    return 1;
  }

  ignoring = delete(ignoring, member_array(str, ignoring), 1);
  TP->add_property("ignoring", ignoring);
  write(str+" has been removed from your list of ignored players.\n");
  return 1;
}
