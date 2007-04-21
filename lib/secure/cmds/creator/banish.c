/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: banish.c,v 1.7 2003/04/15 20:27:22 ceres Exp $
 * $Log: banish.c,v $
 * Revision 1.7  2003/04/15 20:27:22  ceres
 * Removed dependence on login object
 *
 * Revision 1.6  2001/12/23 14:47:47  taffyd
 * Extended to add Liaison deputies as valid people who can banish names.
 *
 * Revision 1.5  2001/06/01 21:15:47  ceres
 * Added senior banish limit?
 *
 * Revision 1.4  2000/09/07 02:32:17  presto
 * Clear player cache for name before checking if the player exists.
 *
 * Revision 1.3  1998/09/21 04:47:18  ceres
 * Got it right this time
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
#include <player_handler.h>

#define LIAISON "/d/liaison/master"

inherit "/cmds/base";
/*
 * Banish code removed from the patricians office by Pinkfish.
 */

int cmd(string str)
{
  string str1, temp1, ban;

  if (this_player() != this_player(1)) {
    return 0;
  }

  if (this_player()->GetForced()) { 
    return 0;
  }

  if(!master()->query_senior(geteuid(previous_object())) && 
      !LIAISON->query_deputy(geteuid(previous_object()))) {
    return notify_fail( "Only Liaison domain "
        "deputies, Senior Creators and above can banish player names.\n" ); 
  }  

  notify_fail("Syntax : banish <name> <reason>\n");
  if (!str)
    return 0;
  if (sscanf(str, "%s %s", str, str1) != 2)
    return 0;
  str = lower_case(str);
  if (sscanf(str,"%s.",temp1))
    return 0;
  if (sscanf(str, "%s/", temp1))
    return 0;
  if (file_size("/banish/"+str[0..0]+"/"+str+".o") >= 0)
  {
    notify_fail("That name is already banished.\n");
    return 0;
  }
  PLAYER_HANDLER->remove_cache_entry(str);
  if (PLAYER_HANDLER->test_user(str))
    return notify_fail("That is a player.  You must rm or mv the player "
                       "file first.\n");
  ban = "Banished by : "+(string)this_player()->query_name()+"\n";
  ban += ctime(time())+"\n";
  ban += "Banish Reason : " + str1 + "\n";
  unguarded((: write_file, "/banish/"+str[0..0]+"/"+str+".o", ban :));
  write(str + " banished.\n");
  return 1;
} /* cmd() */

string *query_patterns() {
  return ({ "", (: cmd(0) :), "<string>", (: cmd($4[0]) :) });
} /* query_patterns() */
