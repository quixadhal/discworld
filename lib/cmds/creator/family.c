/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: family.c,v 1.5 2003/03/21 02:03:45 ceres Exp $
 * $Log: family.c,v $
 * Revision 1.5  2003/03/21 02:03:45  ceres
 * Modified to use player handler
 *
 * Revision 1.4  1999/04/28 22:53:53  pinkfish
 * Change to use a pager.
 *
 * Revision 1.3  1999/04/16 00:53:16  presto
 * Added #include <money.h>
 *
 * Revision 1.2  1999/04/07 01:01:12  pinkfish
 * Add in balance stuff.
 *
 * Revision 1.1  1999/01/30 10:46:43  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
 */
#include <clubs.h>
#include <money.h>

inherit "/cmds/base";
inherit "/std/basic/club_listing";

#include <player_handler.h>
#include <playerinfo.h>

mixed family_reverse(string from, string to) {
  string family_from;
  string family_to;
  string *relations_to;
  string *relations_from;
  string opp;
  string relation;

  family_from = PLAYER_HANDLER->test_family(from);
  family_to = PLAYER_HANDLER->test_family(to);
  if (!family_from) {
    add_failed_mess("Sorry, " + from + " is not in a family.\n");
  }
  if (!family_to) {
    add_failed_mess("Sorry, " + to + " is not in a family.\n");
  }
  if (!family_to ||
      !family_from) {
    return 0;
  }
  relations_to = CLUB_HANDLER->query_relationships_to(family_to,
                                                     to,
                                                     from);
  relations_from = CLUB_HANDLER->query_relationships_to(family_from,
                                                       from,
                                                       to);
  if (!sizeof(relations_to) ||
      !sizeof(relations_from)) {
    add_failed_mess(from + " and " + to + " are not related.\n");
    return 0;
  }

  foreach (relation in relations_from) {
    opp = CLUB_HANDLER->query_opposite_relationship(relation);
    if (!opp) {
      write("Unable to swap " + relation + ".\n");
    } else {
      if (!CLUB_HANDLER->remove_relationship(family_from,
                                             from,
                                             family_to,
                                             to,
                                             relation)) {
        write("Could not remove relationship " + relation + " between " +
              from + " and " + to + ".\n");
      } else {
        CLUB_HANDLER->add_relationship(family_from,
                                       from,
                                       family_to,
                                       to,
                                       opp);
        PLAYERINFO_HANDLER->add_entry(this_player(),
                                      from,
                                      "family",
                                      "Changed relationship with " + to + " to " +
                                      opp);
        PLAYERINFO_HANDLER->add_entry(this_player(),
                                      to,
                                      "family",
                                      "Changed relationship with " + from + " to " +
                                      relation);
        write("Swapped relationship " + relation + " between " +
              from + " and " + to + ".\n");
      }
    }
  }
  return 1;
} /* family_reverse() */

int family_remove(string from, string to) {
  string family_from;
  string family_to;
  string *relations_to;
  string *relations_from;
  string relation;

  family_from = PLAYER_HANDLER->test_family(from);
  family_to = PLAYER_HANDLER->test_family(to);
  if (!family_from) {
    add_failed_mess("Sorry, " + from + " is not in a family.\n");
  }
  if (!family_to) {
    add_failed_mess("Sorry, " + to + " is not in a family.\n");
  }
  if (!family_to ||
      !family_from) {
    return 0;
  }
  relations_to = CLUB_HANDLER->query_relationships_to(family_to,
                                                     to,
                                                     from);
  relations_from = CLUB_HANDLER->query_relationships_to(family_from,
                                                       from,
                                                       to);
  if (!sizeof(relations_to) ||
      !sizeof(relations_from)) {
    add_failed_mess(from + " and " + to + " are not related.\n");
    return 0;
  }

  foreach (relation in relations_from) {
    if (!CLUB_HANDLER->remove_relationship(family_from,
                                           from,
                                           family_to,
                                           to,
                                           relation)) {
      write("Could not remove relationship " + relation + " between " +
            from + " and " + to + ".\n");
    } else {
      write("Removed relationship " + relation + " between " +
            from + " and " + to + ".\n");
      PLAYERINFO_HANDLER->add_entry(this_player(),
                                    from,
                                    "family",
                                    "Removed relationship with " + to);
      PLAYERINFO_HANDLER->add_entry(this_player(),
                                    to,
                                    "family",
                                    "Removed relationship with " + from);      
    }
  }
  return 1;
} /* family_remove() */

private int do_balance(string club_name) {
   int balance;
   string place;

   if (!CLUB_HANDLER->is_club(club_name)) {
      add_failed_mess("There is no family called '" +
                      CLUB_HANDLER->query_club_name(club_name) + "'.\n");
   }
   place = environment(this_player())->query_property("place");
   if (!place) {
      place = "default";
   }
   balance = CLUB_HANDLER->query_balance(club_name);
   write("The balance of the family '" +
         CLUB_HANDLER->query_club_name(club_name) + "' is " +
         MONEY_HAND->money_value_string(balance, place) + ".\n");
   return 1;
} /* do_balance() */

private int do_all_balance() {
   string *clubs;
   string club;
   string place;
   mapping bing;
   string ret;

   clubs = CLUB_HANDLER->query_clubs();
   place = environment(this_player())->query_property("place");
   if (!place) {
      place = "default";
   }
   bing = ([ ]);
   foreach (club in clubs) {
      if (!CLUB_HANDLER->is_family(club)) {
         continue;
      }
      bing[club] = CLUB_HANDLER->query_balance(club);
   }

   clubs = keys(bing);
   clubs = sort_array(clubs, (: $3[$1] - $3[$2] :), bing);
   ret = "";
   foreach (club in clubs) {
      ret += sprintf("Family '%-20s': %s\n",
            CLUB_HANDLER->query_club_name(club),
            MONEY_HAND->money_value_string(bing[club], place));
      
   }
   write("$P$Club Balances$P$" + ret);
   return 1;
} /* do_all_balance() */

mixed *query_patterns() {
  return ({ "info <word'family name'>",
              (: family_info($4[0], this_player()->query_name()) :),
            "reverse relationship <word'player name'> <word'player name'>",
              (: family_reverse($4[0], $4[1]) :),
            "remove relationship <word'player name'> <word'player name'>",
              (: family_remove($4[0], $4[1]) :),
            "balance <string'family name'>", (: do_balance($4[0]) :),
            "balance all", (: do_all_balance() :) });
} /* query_patterns() */
