/**
 * This is an inheritable for printing out different sorts of club
 * listings.
 * @author Pinkfish
 * @started Sat Oct 24 02:28:04 EDT 1998
 * @see /obj/handlers/club_handler.c
 * @see /std/room/club_control_room.c
 */

#include <clubs.h>
#include <broadcaster.h>
#include <player_handler.h>

/**
 * This method will sort and capitalise the array of strings sent into
 * the method.
 * @param arr the array to sort and capitalise
 * @return the sorted and capitalised string
 * @see club_members()
 */
protected string sort_and_capitalise(string *arr) {
   return query_multiple_short(sort_array(map(arr, (: capitalize :)), 0));
} /* sort_and_capitalise() */

/**
 * This method returns the player in control of the item.
 * @param ob the item
 * @return the player in control
 * @see club_members()
 */
protected object person_in_control(object ob) {
   if (ob && environment(ob) &&
       !ob->query_channel_off()) {
      if (living(environment(ob))) {
         return environment(ob);
      }
      ob = environment(ob);
   }
   return 0;
} /* person_in_control() */

private string add_flags_to_name(string club, string name, string output_name) {
   if (CLUB_HANDLER->is_recruiter_of(club, name)) {
      return output_name + "(%^YELLOW%^r%^RESET%^)";
   }
   return output_name;
}

/**
 * This method prints out all the members of the club, or all the members
 * of the club online.  This method is designed to work with
 * add_command().
 * @param club_name the name of the club to print the members of
 * @param online print only the online members, 0 not online, 1 online, 2 online
 *  and listening to the club channel
 * @return 1 if successful, 0 if not
 */
protected int club_members(string club, int online, string person) {
   string *members;
   object *obs;
   string *bing;

   if (CLUB_HANDLER->query_club_secret(club)) {
      if (!CLUB_HANDLER->is_member_of(club, person)) {
         add_failed_mess("You must be a member of a secret club to get "
                         "the members of it.\n");
         return 0;
      }
   }

   members = CLUB_HANDLER->query_members(club);
   if (online == 1) {
      members = filter(members, (: find_player($1) && find_player($1)->query_cap_name() :) );
      write("The currently online members of " + club +
                            " are:\n$C$$I$5=" +
                      query_multiple_short(sort_array(map(members,
                    (: add_flags_to_name($(club), $1, find_player(lower_case($1))->query_cap_name()) :)), 0)) +
                      ".\n");
   } else if (online == 2) {
      obs = BROADCASTER->query_channel_members(lower_case(club));
      bing = map(filter(map(obs, (: person_in_control($1) :)),
                        (: $1 && reference_allowed($1, $(person)) :)),
                 (: $1?$1->query_name():0 :));
      bing = bing & members;
      write("The current members of " + club +
                            " listening are:\n$C$$I$5=" +
                      sort_and_capitalise(bing) + ".\n");
   } else {
      write("The current members of " + club +
                            " are:\n$C$$I$5=" +
                      sort_and_capitalise(
       map(members, (: add_flags_to_name($(club), $1, $1) :))) + ".\n");
   }
   return 1;
} /* club_members() */

/**
 * This method prints out a list of the current clubs.
 * @param person the person who is doing the listing
 * @param print_families 1 print the families, 0 print the clubs
 * @param check_list this function is called to see if the club should be listed
 * @return 0 on failure and 1 for success
 * @see club_info()
 * @see club_members()
 * @see www_club_list()
 * @see www_club_info()
 */
protected int club_list(string person,
                        int print_families,
                        function check_list) {
   string *clubs;
   string club;
   string stuff;
   int num;
   int found;

   clubs = CLUB_HANDLER->query_clubs();
   if (!sizeof(clubs)) {
       add_failed_mess("There are no clubs or families to list.\n");
       return 0;
   }
   if (print_families) {
      clubs = filter(clubs, (: CLUB_HANDLER->is_family($1) :));
      stuff = "Families:\n";
   } else {
      clubs = filter(clubs, (: !CLUB_HANDLER->is_family($1) :));
      stuff = "Clubs:\n";
   }
   clubs = sort_array(clubs, 1);
   foreach (club in clubs) {
      if (!check_list ||
          evaluate(check_list, club, person)) {
         found++;
         num = sizeof(CLUB_HANDLER->query_members(club));
         if (CLUB_HANDLER->is_family(club)) {
            stuff += "'" + CLUB_HANDLER->query_club_name(club) +
                     "' was founded by " +
               capitalize(CLUB_HANDLER->query_founder(club)) +
                     " with " + query_num(num) + " member" +
                     (num > 1?"s":"") + ".\n";
         } else if (!CLUB_HANDLER->query_club_secret(club) ||
             CLUB_HANDLER->is_member_of(club, person)) {
            stuff += "'" + CLUB_HANDLER->query_club_name(club) +
                     "' founded by " + 
                     capitalize(CLUB_HANDLER->query_founder(club)) +
                     " with " + query_num(num) + " member" +
                     (num > 1?"s":"") + ".\n";
          } else {
            stuff += "'" + CLUB_HANDLER->query_club_name(club) +
                     "' founded by " + 
                     capitalize(CLUB_HANDLER->query_founder(club)) +
                     " with an unknown number of members.\n";
          }
       }
    }

    if (!found) {
       return 0;
    }

    if (print_families) {
       write("$P$Family list$P$" + stuff);
    } else {
       write("$P$Club list$P$" + stuff);
    }
    return 1;
} /* club_list() */

/**
 * This method returns the club information as a string.
 * @param name the name of the club to show the information about
 * @param show_election if non-zero show the election information
 * @param person the person who is showing the information
 * @return the club information string, 0 if no information
 * @see club_info()
 */
protected string club_info_string(string name,
                                  int show_election,
                                  string person) {
   string *nominations;
   string *positions;
   string *online;
   string pos;
   string nom;
   string last;
   string ret;
   string desc;
   string status;
   int member;
   int min;

   if (!CLUB_HANDLER->is_club(name) ||
       CLUB_HANDLER->is_family(name)) {
      return 0;
   }

   ret = "";

   member = CLUB_HANDLER->is_member_of(name, person);
   if (CLUB_HANDLER->query_club_secret(name)) {
      if (!member) {
         return "This is a secret club and no information is shown "
                "about the club to non members.\n";
      } else {
         ret += "This is a secret club!  Information about members is not "
                "shown to people outside the club.\n";
      }
   }

   ret += "Information on club '" + name + "' with a founder of " +
         capitalize(CLUB_HANDLER->query_founder(name)) +
         " based in " +
         CLUB_HANDLER->query_club_region(name) +
         ".\n";
   desc = CLUB_HANDLER->query_club_description(name);
   if (desc) {
      ret += "$I$2=  " + desc + "\n%^RESET%^$I$0=";
   }

   online = filter(CLUB_HANDLER->query_members(name), (: find_player($1) :));
   if (sizeof(online)) {
      ret += "$I$5=The online members are: " +
          sort_and_capitalise(online) +
          ".$I$0=\n";
   }
   ret += "$I$5=The not currently online members are: " +
          sort_and_capitalise(CLUB_HANDLER->query_members(name) - online) +
          ".$I$0=\n";
   if (!CLUB_HANDLER->is_family(name)) {
      ret += "$I$5=The recruiters are: " +
            sort_and_capitalise(CLUB_HANDLER->query_recruiters(name)) +
            ".$I$0=\n";
   }
   if (CLUB_HANDLER->is_elected_club(name)) {
      ret += "This is an elected club type.\n";
      positions = CLUB_HANDLER->query_valid_positions(name);
      foreach (pos in positions) {
         ret += capitalize(pos) + ": " +
               capitalize(CLUB_HANDLER->query_position_members(name,
                                                     pos)) + "\n";
      }

      if (CLUB_HANDLER->is_nomination_in_progress(name) &&
          show_election) {
         status = CLUB_HANDLER->query_election_status(name);
         ret += "\nThe player must nominate themselves to accept the "
               "nomination.  Until this point the nomination is not "
               "accepted.\n";
         if (status) {
            ret += status + "\n";
         }
         ret += "The club is currently accepting nominations for "
               "positions, the nomination period ends at " +
               ctime(CLUB_HANDLER->query_next_event_time(name)) +
               ".  So far the following have been "
               "nominated:\n";
         foreach (pos in positions) {
            nominations = CLUB_HANDLER->query_nominated_for(name, pos);
            if (sizeof(nominations)) {
               ret += capitalize(pos) + " has the following nominations:\n";
               foreach (nom in nominations) {
                  if (CLUB_HANDLER->is_nomination_accepted(name, pos, nom)) {
                     last = " (accepted).\n";
                  } else {
                     last = ".\n";
                  }
                  if (sizeof(CLUB_HANDLER->query_nominators_for(name, pos, nom))) {
                     ret += "$I$9=     " + capitalize(nom) + " nominated by " +
                           sort_and_capitalise(
                           CLUB_HANDLER->query_nominators_for(name, pos, nom)) +
                           last + "$I$0=";
                  } else {
                     ret += "$I$9=     " + capitalize(nom) + " nominated by " +
                           "noone" + last + "$I$0=";
                  }
               }
           } else {
               ret += "No nominations for the position of " + capitalize(pos) +
                     ".\n";
           }
         }
      } else if (CLUB_HANDLER->is_election_in_progress(name) &&
                 show_election) {
         status = CLUB_HANDLER->query_election_status(name);
         ret += "\nThe club is currently electing new committee members, the "
               "vote ends at " +
               ctime(CLUB_HANDLER->query_next_event_time(name)) +
               ".\n";
         if (status) {
            ret += status + "\n";
         }
         if (member) {
            ret += "You can vote for:\n";
         } else {
            ret += "Nominations for the positions are:\n";
         }
         foreach (pos in positions) {
            nominations = CLUB_HANDLER->query_nominated_for(name, pos);
            if (sizeof(nominations)) {
               ret += capitalize(pos) + ": " +
                     sort_and_capitalise(nominations) +
                     ".";
               if (member) {
                  if (CLUB_HANDLER->has_person_voted(name,
                                                pos,
                                                this_player()->query_name())) {
                     ret += " (voted)\n";
                  } else {
                     ret += " (not voted)\n";
                  }
               } else {
                  ret += "\n";
               }
            } else {
               ret += capitalize(pos) + ": " +
                      capitalize(CLUB_HANDLER->query_position_members(name,
                                                           pos)) +
                      " elected with no opposition.\n";
            }
         }
      } else {
         min = CLUB_HANDLER->query_minimum_next_election(name);
         if (min < time()) {
            ret += "The next default election will be held at " +
                   ctime(CLUB_HANDLER->query_next_election(name)) + " and an "
                   "election can now be forced.\n";
         } else {
            ret += "The next default election will be held at " +
                   ctime(CLUB_HANDLER->query_next_election(name)) + " and an "
                   "election can be forced after " +
                   ctime(min) + ".\n";
         }
      }
   }
   return ret;
} /* club_info_string() */

/**
 * This method prints out the information associated with the club.
 * The stuff printed out here is directly lifted from the club control room.
 * This method is designed to work with add_command().
 * @param name the name of the club to show the information about
 * @param show_election if non-zero show the election information
 * @param person the person who is showing the information
 * @return 1 on success, 0 on failure
 */
protected int club_info(string name, int show_election, string person) {
   if (!CLUB_HANDLER->is_club(name)) {
      add_failed_mess("There is no club called " + name + ".\n");
      return 0;
   }

   if (CLUB_HANDLER->query_club_secret(name)) {
      if (!CLUB_HANDLER->is_member_of(name, person)) {
         add_failed_mess("You must be a member of a secret club to "
                         "see information on it.\n");
         return 0;
      }
   }

   write("$P$" + CLUB_HANDLER->query_club_name(name) + "$P$" +
          club_info_string(name, show_election, person));
   return 1;
} /* club_info() */

/**
 * This method returns the relations that the specified player
 * has as a string.  It will return a blank string if the player
 * has no relationships.
 * @param player the player to check relationships for
 */
string player_relationships_string(string player) {
   string family;
   string ret;
   string region;
   class relationship_data *wombles;
   int gender;
   int i;

   family = PLAYER_HANDLER->test_family(player);
   if (!family) {
      return "";
   }

   if (!CLUB_HANDLER->is_family(family)) {
      return "";
   }

   ret = "";
   region = CLUB_HANDLER->query_club_region(family);
   wombles = CLUB_HANDLER->query_relationships(family,
                                               player);
   wombles = sort_array(wombles, 
                        (: strcmp($1->related_to, $2->related_to) :));
   gender = PLAYER_HANDLER->test_gender(player);
   if (gender) {
      gender -= 1;
   }
   if (sizeof(wombles)) {
      for (i = 0; i < sizeof(wombles); i++) {
         ret += CLUB_HANDLER->query_relationship_gender(wombles[i]->relationship, gender, region) +
             " of " + capitalize(wombles[i]->related_to);
         if (i == sizeof(wombles) - 2) {
            ret += " and ";
         } else if (i < sizeof(wombles) -2) {
            ret += "; ";
         }
      }
   }
   return ret;
} /* player_relationships_string() */

/**
 * This method returns the family information as a string.
 * @param name the name of the family to show the information about
 * @param show_election if non-zero show the election information
 * @param person the person who is showing the information
 * @return the family information string, 0 if no information
 * @see family_info()
 */
protected string family_info_string(string name,
                                    string person) {
   string ret;
   string desc;

   if (!CLUB_HANDLER->is_family(name)) {
      return 0;
   }

   ret = "";
   ret += "Information on family '" + name + "' with a founder of " +
         capitalize(CLUB_HANDLER->query_founder(name)) +
         " based in " +
         CLUB_HANDLER->query_club_region(name) + ".\n";
   desc = CLUB_HANDLER->query_club_description(name);
   if (desc) {
      ret += "$I$2=  " + desc + "\n%^RESET%^$I$0=";
   }
/*
   foreach (per in CLUB_HANDLER->query_members(name)) {
      ret += "$I$5=" + capitalize(per);
      wombles = CLUB_HANDLER->query_relationships(name,
                                                  per);
      wombles = sort_array(wombles, 
                           (: strcmp($1->related_to, $2->related_to) :));
      gender = PLAYER_HANDLER->test_gender(per);
      if (gender) {
         gender -= 1;
      }
      if (sizeof(wombles)) {
         ret += " is the ";
         for (i = 0; i < sizeof(wombles); i++) {
            //gender = PLAYER_HANDLER->test_gender(wombles[i]->related_to);
            //if (gender) {
               //gender -= 1;
            //}
            ret += CLUB_HANDLER->query_relationship_gender(wombles[i]->relationship, gender) +
                   " of " + capitalize(wombles[i]->related_to);
            if (i == sizeof(wombles) - 2) {
               ret += " and ";
            } else if (i < sizeof(wombles) -2) {
               ret += "; ";
            }
         }
         ret += "\n";
      } else {
         ret += " with no relationships.\n";
      }
   }
 */
      
   ret += "$I$5=The members are: " +
          sort_and_capitalise(CLUB_HANDLER->query_members(name)) +
          ".$I$0=\n";
   return ret;
} /* family_info_string() */

/**
 * This method prints out the information associated with the family.
 * The stuff printed out here is directly lifted from the family control room.
 * This method is designed to work with add_command().
 * @param name the name of the family to show the information about
 * @param show_election if non-zero show the election information
 * @param person the person who is showing the information
 * @return 1 on success, 0 on failure
 */
protected int family_info(string name, string person) {
   if (!CLUB_HANDLER->is_family(name)) {
      add_failed_mess("There is no family called " + name + ".\n");
      return 0;
   }

   write("$P$" + CLUB_HANDLER->query_club_name(name) + "$P$" +
          family_info_string(name, person));
   return 1;
} /* family_info() */

/**
 * This method turns a name into a fingerable name.
 * @param name the name to make fingerable
 * @return the htmlised name string
 */
protected string www_finger_name(string name) {
   return "<a href=\"finger.c?player=" + lower_case(name) + "\">" +
          name + "</a>";
} /* www_finger_name() */

/**
 * This method prints out a list of the current clubs into html.
 * @param person the person who is doing the listing
 * @param print_families 1 print the families, 0 print the clubs
 * @param start the index to start from
 * @param show how many to show
 * @return the club listing string
 * @see club_info()
 * @see club_members()
 * @see www_club_list()
 * @see www_club_info()
 */
protected string www_club_list(string person, int print_families, int start, int show) {
   string *clubs;
   string club;
   string stuff;
   int num;

   clubs = CLUB_HANDLER->query_clubs();
   if (!sizeof(clubs)) {
       return "There are no clubs or families to list.\n";
   }
   if (print_families) {
      clubs = filter(clubs, (: CLUB_HANDLER->is_family($1) :));
      stuff = "<h2>Current " + mud_name() + " Families</h2>\n";
   } else {
      clubs = filter(clubs, (: !CLUB_HANDLER->is_family($1) :));
      stuff = "<h2>Current " + mud_name() + " Clubs</h2>\n";
   }
   clubs = sort_array(clubs, 1);
   stuff += "<ul>";
   foreach (club in clubs[start..start+show]) {
      num = sizeof(CLUB_HANDLER->query_members(club));
      if (CLUB_HANDLER->is_family(club) ||
          !CLUB_HANDLER->query_club_secret(club) ||
          CLUB_HANDLER->is_member_of(club, person)) {
         stuff += "<li>'<a href=\"club_info.c?club_name=" + club + "\">" +
                  CLUB_HANDLER->query_club_name(club) +
                  "</a>' was founded by " +
            www_finger_name(capitalize(CLUB_HANDLER->query_founder(club))) +
                  " with " + query_num(num) + " member" +
                  (num > 1?"s":"") + ".<br>\n";
       } else {
         stuff += "<li>'<a href=\"club_info.c?club_name=" + club + "\">" +
                  CLUB_HANDLER->query_club_name(club) +
                  "</a>' was founded by " +
            www_finger_name(capitalize(CLUB_HANDLER->query_founder(club))) +
                  " with an unknown number of members.\n";
       }
   }
   if (print_families) {
      return stuff + "</ul>\n" +
           "There are currently " + query_num(sizeof(clubs)) + " "
           "families on " + mud_name() + " \n";
   } else {
       return stuff + "</ul>\n" +
           "There are currently " + query_num(sizeof(clubs)) + " "
           "clubs on " + mud_name() + " \n";
   }
} /* www_club_list() */

/**
 * This method returns the club information as a html string.
 * @param name the name of the club to show the information about
 * @param show_election if non-zero show the election information
 * @param person the person who is showing the information
 * @return the club information string, 0 if no information
 * @see club_info()
 */
protected string www_club_info_string(string name,
                                      int show_election,
                                      string person) {
   string *nominations;
   string *positions;
   string pos;
   string nom;
   string last;
   string ret;
   string desc;
   string status;
   string *stuff;
   int member;

   if (!CLUB_HANDLER->is_club(name) ||
       CLUB_HANDLER->is_family(name)) {
      return 0;
   }

   ret = "";

   member = CLUB_HANDLER->is_member_of(name, person);
   if (CLUB_HANDLER->query_club_secret(name)) {
      if (!member) {
         ret += "This is a secret club!  Information about members is not "
                "shown to people outside the club.<p>\n";
         // Still show them the description.
         desc = CLUB_HANDLER->query_club_description(name);
         if (desc) {
            ret += "<blockquote>" + desc + "\n</blockquote>";
         }
         return ret;
      } else {
         ret += "This is a secret club!  Information about members is not "
                "shown to people outside the club.<p>\n";
      }
   }

   ret += "<h4>Information on club '" +
          CLUB_HANDLER->query_club_name(name) +
          "' with a founder of "  +
          www_finger_name(CLUB_HANDLER->query_founder(name)) + 
          " based in " +
          CLUB_HANDLER->query_club_region(name) + ".</h4>\n";
   desc = CLUB_HANDLER->query_club_description(name);
   if (desc) {
      ret += "<blockquote>" + desc + "\n</blockquote>";
   }
   stuff = CLUB_HANDLER->query_members(name);
   stuff = sort_array(stuff, 0);
   stuff = map(stuff, (: www_finger_name(capitalize($1)) :) );
   ret += "<dl><dt><b>The members are:</b>\n<dd>" +
          query_multiple_short(stuff) +
          "</dl>\n";
   if (!CLUB_HANDLER->is_family(name)) {
      stuff = CLUB_HANDLER->query_recruiters(name);
      stuff = sort_array(stuff, 0);
      stuff = map(stuff, (: www_finger_name(capitalize($1)) :) );
      ret += "<dl><dt><b>The recruiters are:</b>\n<dd>" +
             query_multiple_short(stuff) +
             "</dl>\n";
   }
   if (CLUB_HANDLER->is_elected_club(name)) {
      ret += "This is an elected club type.<br>\n";

      if (CLUB_HANDLER->query_club_board(name)) {
         ret += "<a href=\"boards.c?board=" +
            replace_string(CLUB_HANDLER->query_club_board_name(name), 
                           " ", "%20") +
            "\">The club board</a><p>\n";
      }

      positions = CLUB_HANDLER->query_valid_positions(name);
      ret += "<ul>\n";
      foreach (pos in positions) {
         ret += "<li>" + capitalize(pos) + ": " +
               www_finger_name(capitalize(CLUB_HANDLER->query_position_members(name,
                                                     pos))) + "\n";
      }
      ret += "</ul>";
      if (CLUB_HANDLER->is_nomination_in_progress(name) &&
          show_election) {
         status = CLUB_HANDLER->query_election_status(name);
         ret += "<p>The player must nominate themselves to accept the "
               "nomination.  Until this point the nomination is not "
               "accepted.\n";
         if (status) {
            ret += status + "<p>\n";
         }
         ret += "<p>The club is currently accepting nominations for "
               "positions, the nomination period ends at " +
               ctime(CLUB_HANDLER->query_next_event_time(name)) +
               ".  So far the following have been "
               "nominated:\n<dl>";

         foreach (pos in positions) {
            nominations = CLUB_HANDLER->query_nominated_for(name, pos);
            if (sizeof(nominations)) {
               ret += "<dt>" + capitalize(pos) +
                      " has the following nominations:\n";
               foreach (nom in nominations) {
                  if (CLUB_HANDLER->is_nomination_accepted(name, pos, nom)) {
                     last = " (accepted).\n";
                  } else {
                     last = ".\n";
                  }
                  stuff = CLUB_HANDLER->query_nominators_for(name, pos, nom);
                  stuff = sort_array(stuff, 0);
                  stuff = map(stuff, (: www_finger_name(capitalize($1)) :) );
                  ret += "<dd>" +
                         www_finger_name(capitalize(nom)) + " nominated by " +
                         query_multiple_short(stuff) + 
                         last;
               }
           } else {
               ret += "No nominations for the position of " + capitalize(pos) +
                     ".\n";
           }
           ret += "</dl>";
         }
      } else if (CLUB_HANDLER->is_election_in_progress(name) &&
                 show_election) {
         status = CLUB_HANDLER->query_election_status(name);
         ret += "<p>The club is currently electing new committee members, the "
               "vote ends at " +
               ctime(CLUB_HANDLER->query_next_event_time(name)) +
               ".<p>";
         if (status) {
            ret += status + "<p>\n";
         }
         ret += "You can vote for:\n<ul>";
         foreach (pos in positions) {
            stuff = CLUB_HANDLER->query_nominated_for(name, pos);
            if (sizeof(stuff)) {
               stuff = sort_array(stuff, 0);
               stuff = map(stuff, (: www_finger_name(capitalize($1)) :) );
               ret += "<li>" + capitalize(pos) + ": " +
                      query_multiple_short(stuff) +
                     ".";
               if (member) {
                  if (CLUB_HANDLER->has_person_voted(name, pos,
                                     person)) {
                     ret += " (voted)\n";
                  } else {
                     ret += " (not voted)\n";
                  }
               } else {
                  ret += "\n";
               }
            } else {
               ret += "<li>" + capitalize(pos) + ": " +
                   www_finger_name(capitalize(CLUB_HANDLER->query_position_members(name,
                                                        pos))) +
                   " elected with no opposition.\n";
            }
         }
         ret += "</ul>\n";
      }  else {
         ret += "The next election will be held at " +
                ctime(CLUB_HANDLER->query_next_election(name)) + "\n<p>";
      }
   }
   return ret;
} /* www_club_info_string() */

/**
 * This method returns the family information as a string.
 * @param name the name of the family to show the information about
 * @param show_election if non-zero show the election information
 * @param person the person who is showing the information
 * @return the family information string, 0 if no information
 * @see family_info()
 */
protected string www_family_info_string(string name,
                                        string person) {
   string ret;
   string desc;
   string per;
   //class relationship_data *wombles;
   //int i;
   //int gender;
   string *stuff;

   if (!CLUB_HANDLER->is_family(name)) {
      return 0;
   }

   ret = "";
   ret += "<h4>Information on family '" +
         CLUB_HANDLER->query_club_name(name) + "' with a founder of " +
         www_finger_name(capitalize(CLUB_HANDLER->query_founder(name))) +
         " based in " +
         CLUB_HANDLER->query_club_region(name) + ".</h4>\n";
   desc = CLUB_HANDLER->query_club_description(name);
   if (desc) {
      ret += "<blockquote>" + desc + "</blockquote>\n";
   }
   ret += "<dl>";
   foreach (per in CLUB_HANDLER->query_members(name)) {
      ret += "<dt>" + www_finger_name(capitalize(per));
/* + " related to:\n";
      wombles = CLUB_HANDLER->query_relationships(name,
                                                  per);
      wombles = sort_array(wombles, 
                           (: strcmp($1->related_to, $2->related_to) :));
      ret += "<dd><ul>\n";
      gender = PLAYER_HANDLER->test_gender(per);
      if (gender) {
         gender -= 1;
      }
      if (sizeof(wombles)) {
         for (i = 0; i < sizeof(wombles); i++) {
            //gender = PLAYER_HANDLER->test_gender(wombles[i]->related_to);
            //if (gender) {
               //gender -= 1;
            //}
            ret += "<li>is the " +
                   CLUB_HANDLER->query_relationship_gender(wombles[i]->relationship, gender) +
                   " of " +
                   www_finger_name(capitalize(wombles[i]->related_to)) + 
                   "\n";
         }
         ret += "\n";
      } else {
         ret += "No one!\n";
      }
      ret += "</ul>\n";
 */
   }
   ret += "</dl>";

   stuff = CLUB_HANDLER->query_members(name);
   stuff = sort_array(stuff, 0);
   stuff = map(stuff, (: www_finger_name(capitalize($1)) :) );
   ret += "<dl><dt><b>The members are:</b>\n<dd>" +
          query_multiple_short(stuff) + "</dl>\n";
   return ret;
} /* family_info_string() */
