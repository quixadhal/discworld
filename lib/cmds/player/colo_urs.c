/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: colo_urs.c,v 1.11 2002/02/24 03:07:12 ceres Exp $
 * 
 */
#include <colour.h>
#include <clubs.h>
#include <creator.h>
#include <newbiehelpers.h>

inherit "cmds/base";

private string* _colours;

void create() {
  ::create();
  _colours = ({
   "BOLD",
   "FLASH",
   "BLACK",
   "RED",
   "BLUE",
   "CYAN",
   "MAGENTA",
   "ORANGE",
   "YELLOW",
   "GREEN",
   "WHITE",
   "B_RED",
   "B_ORANGE",
   "B_YELLOW",
   "B_BLACK",
   "B_CYAN",
   "B_WHITE",
   "B_GREEN",
   "B_MAGENTA" });
} /* create() */

int is_valid_colour(string name) {
   if (member_array(name, _colours) != -1) {
      return 1;
   }
   return 0;
} /* is_valid_colour() */

int show_allowed_colours() {
   write("The allowed colour codes are " +
         query_multiple_short(map(_colours, (: lower_case($1) :)), 0, 0, 1) +
         ".\n");
   return 1;
} /* show_allowed_colours() */


string* query_colour_list(int inform) {
  string* colour_list;
  string* clubs;
  string bing;

  switch (inform) {
  case 0:
    colour_list = USER_COLOUR_LIST;
    if( NEWBIEHELPERS_HANDLER->query_can_chat(this_player()) ) {
      colour_list += ({"newbiehelpers"});
    }
    if(this_player()->query_creator()) {
      colour_list += this_player()->channel_list();
    }
    break;
  case 1 :
    colour_list = this_player()->query_inform_types();
    break;
  case 2 :
    clubs = this_player()->query_player_clubs();
    clubs = filter(clubs, (: !CLUB_HANDLER->is_club($1) :));
    foreach (bing in clubs) {
       this_player()->remove_player_club(bing);
    }
    colour_list = map(this_player()->query_player_clubs(), (: "club_" + $1 :));
    break;
  }
  return colour_list;
} /* query_colour_list() */

mixed set_colours(int inform, string event_type, string colour, int force) {
  mapping my_colours;
  string *colour_list;
  string *bad;
  string tmp;
  string name;

  colour_list = query_colour_list(inform);
  
  if (inform == 2) {
    name = CLUB_HANDLER->query_club_name(event_type);
    event_type = "club_" + event_type;
  } else {
    name = lower_case(event_type);
  }
  
  if(member_array(event_type, colour_list) == -1) {
    return notify_fail("No such type.\n");
  }

  my_colours = this_player()->query_my_colours();
  
  if(colour == "default") {
    this_player()->set_my_colours(event_type, colour);
    write(name + " colour set to default.\n");
  } else if(colour == "none") {
    this_player()->set_my_colours(event_type, colour);
    write(name + " colour set to none.\n");
  } else {
    colour_list = map(explode(colour, " "),
                      (: upper_case($1) :));
    bad = filter(colour_list, (: !is_valid_colour($1) :));
    if (sizeof(bad) && !force) {
       if (sizeof(bad) > 1) {
          add_failed_mess("The colours " + query_multiple_short(bad) +
                          " are not valid, valid colours are: " +
                          query_multiple_short(_colours) + ".\n");
       } else {
          add_failed_mess("The colour " + query_multiple_short(bad) +
                          " is not valid, valid colours are: " +
                          query_multiple_short(_colours) + ".\n");
       }
       return 0;
    }

    tmp = "%^" + implode(colour_list, (: $1 + "%^ %^" + $2 :)) + "%^";

    this_player()->set_my_colours(event_type, tmp);
    write(name + " colour set to " + tmp + "[" + colour +
          "]%^RESET%^.\n");
  }
  return 1;
} /* set_colours() */

private int show_colours(int inform) {
  string *colour_list, event;
  mapping my_colours;
  string name;

  colour_list = query_colour_list(inform);
  my_colours = this_player()->query_my_colours();
 
  if (!inform) {
    write(sprintf("%-20s %s\n", "Clubs", "<list>"));
    write(sprintf("%-20s %s\n", "Inform", "<list>"));
  }
  foreach(event in colour_list) {
    if (inform == 2) {
       name = CLUB_HANDLER->query_club_name(event[5..]);
    } else {
       name = event;
    }
    if(my_colours[event]) {
      if(my_colours[event] != "") {
        write(sprintf("%-20s %s\n", name, my_colours[event] +
                      "[" + replace(my_colours[event], "%^", "") +
                      "]%^RESET%^"));
      } else {
        write(sprintf("%-20s %s\n", name, "[none]"));
      }
    } else {
      write(sprintf("%-20s %s\n", name, "[default]"));
    }
  }
  return 1;
} /* show_colours() */


mixed *query_patterns() {
  return ({ "<word'event type'> <string'colour'>",
              (: set_colours(0, $4[0], $4[1], 0) :),
            "force <word'event type'> <string'colour'>",
              (: set_colours(0, $4[0], $4[1], 1) :),
            "", (: show_colours(0) :),
            "inform <word'inform type'> <string'colour'>",
              (: set_colours(1, $4[0], $4[1], 0) :),
            "inform force <word'inform type'> <string'colour'>",
              (: set_colours(1, $4[0], $4[1], 1) :),
            "club <string:quoted'club name'> <string'colour'>",
              (: set_colours(2, $4[0], $4[1], 0) :),
            "club force <string:quoted'club name'> <string'colour'>",
              (: set_colours(2, $4[0], $4[1], 1) :),
            "inform", (: show_colours(1) :),
            "allowed", (: show_allowed_colours() :),
            "clubs", (: show_colours(2) :) });
} /* query_patterns() */
