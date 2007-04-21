/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: clubs.c,v 1.3 2002/03/01 22:42:50 ceres Exp $
 * 
*/

#include <runtime_config.h>
#include <clubs.h>
#include <mail.h>
#include <money.h>

inherit "/cmds/base";

#define CONTROL_ROOM "/std/room/club_control_room"

// Length of a list entry
#define ELEN 20

private void do_list(string extra, int width);
private void do_invalid(string extra, int width);

mixed cmd(string command, string extra, string rest) {

  if (this_player() != this_player(1))
    return 0;

  switch(command) {
  case "list":
    do_list(extra, this_player()->query_cols());
    break;
  case "invalid":
    do_invalid(extra, this_player()->query_cols());
    break;
  }
  return 1;
} /* cmd() */

private string club_line(string club, int width, int verbose) {
  string tmp, tmp2, member;
  int clubtype, i;

  clubtype = CLUB_HANDLER->query_club_type(club);
  
  tmp = sprintf("%%^BOLD%%^%%^RED%%^%-9s%%^RESET%%^ %-15s\n", 
                CLUB_ARRAY[clubtype]+":", club);

  if(verbose) {
        tmp+= sprintf(" %%^BOLD%%^Club Name:%%^RESET%%^ %s\n",
                  CLUB_HANDLER->query_club_name(club));
    tmp += sprintf(" %%^BOLD%%^Founder:%%^RESET%%^ %s\n"
                   " %%^BOLD%%^Members:%%^RESET%%^\n",
                   CLUB_HANDLER->query_founder(club));
    i = 0;
    tmp2 = "";
    foreach(member in CLUB_HANDLER->query_members(club)) {
      tmp2 += sprintf("%-15s ", member);
      if((i-1 * ELEN) % (width/ELEN) == 0  && tmp2 != "") {
        tmp += this_player()->fix_string("   " + tmp2 + "\n", width, 10);
        tmp2 = "";
      }
      i++;
    }
  }
  return tmp;
}

private void do_list(string extra, int width) {
  string club, tmp, t2, *clubs;

  tmp = "";
  clubs = sort_array(CLUB_HANDLER->query_clubs(),
                     (: strcmp($1, $2 ) :) );

  foreach(club in clubs) {
    if(extra == "" || extra == "brief" || extra == "verbose" ||
       strsrch(club, extra) != -1) {
      t2 = club_line(club, width, (extra != "brief"));
      if(strlen(tmp) + strlen(t2) < 200000)
        tmp += t2;
      else
        break;
    }
}

  if(tmp == "")
    write("No clubs matching " + extra + ".\n");
  else
    this_player()->more_string(tmp, "Clubs", 1);
} /* do_list() */

private int do_balance(string club_name) {
   int balance;
   string place;

   if (!CLUB_HANDLER->is_club(club_name)) {
      add_failed_mess("There is no club called '" +
                      CLUB_HANDLER->query_club_name(club_name) + "'.\n");
   }
   place = environment(this_player())->query_property("place");
   if (!place) {
      place = "default";
   }
   balance = CLUB_HANDLER->query_balance(club_name);
   write("The balance of the club '" +
         CLUB_HANDLER->query_club_name(club_name) + "' is " +
         MONEY_HAND->money_value_string(balance, place) + ".\n");
   return 1;
} /* do_balance() */

mixed *query_patterns() {
  return ({ "list", (: cmd("list", "", "") :),
            "list <string'club name'>", (: cmd("list", $4[0], "") :),
            "balance <string'club name'>", (: do_balance($4[0]) :)
        });
} /* query_patterns() */
