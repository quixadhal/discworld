#include <nomic_system.h>
#include <http.h>
#include <player_handler.h>

/*
 * Makes all the '<' turn into $gt; and the '>' the same.
 */
string htmlify(string  str) {
  return replace(str, ({ "<", "&lt;", ">", "&gt;", "\n", "<br>",
                         "&", "&amp;", " ", "%20" }));
} /* htmlify() */

string make_header(string title) {
   return "<html<head><title>" + title + "</title></head>\n"
          "<body bgcolor=#ffffff TEXT=#000030 LINK=#4a529c VLINK=#b57339>\n"
          "<font face=\"arial, helvetica\">\n"
          "<h2>Discworld MUD</h2>\n"
          "<h4><i>Where players grow and multiply</i></h4>\n"
          "<br clear=both>";
} /* make_header() */

string show_motions(string area, class http_request req) {
   class nomic_motion* motions;
   class nomic_motion motion;
   string ret;

   ret = make_header(area);

   if (NOMIC_HANDLER->is_citizen_of(area, req->user)) {
      motions = NOMIC_HANDLER->query_all_motions(area);
      if (!NOMIC_HANDLER->is_magistrate_of(area, req->user)) {
         motions = filter(motions, (: ((class nomic_motion)$1)->state == NOMIC_STATE_CITIZEN_VOTE :));
      }
      if (sizeof(motions)) {
         ret += "<h3>Voting</h3>\n<blockquote>\n";
         foreach (motion in motions) {
            ret += NOMIC_HANDLER->motion_as_html(area, motion);

            if (motion->state != NOMIC_STATE_CITIZEN_VOTE &&
                motion->state != NOMIC_STATE_COUNCIL_VOTE) {
               ret += "<h4>Not in a voting state</h4>";
            } else if (!NOMIC_HANDLER->has_voted_for_motion(area,
                                                     motion->identifier,
                                                     req->user)) {
               ret += "<h4>Vote <a href=\"nomic_rules.c?type=vote&area=" +
                      htmlify(area) + "&vote=yes&id=" + motion->identifier +
                      "\">Yes</a> | ";
               ret += "<a href=\"nomic_rules.c?type=vote&area=" +
                      htmlify(area) + "&vote=no&id=" + motion->identifier +
                      "\">No</a> | ";
               ret += "<a href=\"nomic_rules.c?type=vote&area=" +
                      htmlify(area) + "&vote=abstain&id=" + motion->identifier +
                      "\">Abstain</a></h4>";
            } else {
               ret += "<h4>Already voted</h4>";
            }
         }
         ret += "</blockquote>\n";
      }
   } else {
      ret += "You are not a citizen of " + area + ".\n";
   }
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
}

string show_area(string area, class http_request req) {
   string ret;
   class nomic_rule* rules;
   int cur_type;
   class nomic_rule rule;
   string *magistrates;

   ret = make_header(area);

   magistrates = NOMIC_HANDLER->query_magistrates(area);
    
   ret += "<br>\n";

   if (sizeof(magistrates)) { 
      ret += "<b>Magistrates:</b> " + 
         query_multiple_short(map(magistrates,
         (: "<a href=\"finger.c?player=" + $1 + "\">" +
         $1 + "</a>\n" :) )) + "\n";
   }
   else {
       ret += "<b>There are no elected magistrates in " + area + ".</b>\n";
   }

   ret += "<br>\n";

   // First figure out if there is anything to vote for.
   if (NOMIC_HANDLER->is_citizen_of(area, req->user)) {
      if (NOMIC_HANDLER->query_citizen_board(area)) {
         ret += "<b>Boards:</b> <a href=\"boards.c?type=subject&board=" +
                htmlify(NOMIC_HANDLER->query_citizen_board(area)) +                             "\">Citizen board</a>";
         if (NOMIC_HANDLER->is_magistrate_of(area, req->user) &&
             NOMIC_HANDLER->query_magistrate_board(area)) {
            ret += ", <a href=\"boards.c?type=subject&board=" +
                   htmlify(NOMIC_HANDLER->query_magistrate_board(area)) +
                   "\">Magistrate board</a>";
         }
         ret += "<p>";
      }
      

      if (NOMIC_HANDLER->is_election_phase(area)) {
         ret += "There is currently an election on, you can "
                "<a href=\"nomic_rules.c?type=voting&area=" + htmlify(area) +
                "\">vote here</a>.<p>";
      }
   }

   rules = NOMIC_HANDLER->query_all_nomic_rules(area);
   rules = sort_array(rules, (: ((class nomic_rule)$1)->id -
                                ((class nomic_rule)$2)->id :) );
   cur_type = -1;
   foreach (rule in rules) {
      if (rule->type != cur_type) {
         switch (rule->type) {
         case NOMIC_TYPE_IMMUTABLE :
            ret += "<h3>Immutable rules</h3>\n"
                   "<blockquote><font color=brown>These are the unchangeable default set of rules for "
                   "the council.</font></blockquote><p>";
            break;
         case NOMIC_TYPE_CITIZEN   :
            ret += "<h3>Citizen rules</h3>\n"
                   "<blockquote><font color=brown>These are the rules that only effect the citizens of "
                   "the council.</font></blockquote><p>";
            break;
         case NOMIC_TYPE_GENERAL   :
            ret += "<h3>General rules</h3>\n"
                   "<blockquote><font color=brown>These are the rules that effect everyone in the "
                   "jurisdiction of the council.</font></blockquote><p>";
            break;
         }
         cur_type = rule->type;
      }
      ret += "<li>" + NOMIC_HANDLER->rule_as_html(rule);
      ret += "<p>\n";
   }
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_area() */

string query_main_vote_page(string area, mapping people) {
   string person;
   string ret;

   ret = "<form method=post action=\"nomic_rules.c?area=" +
          htmlify(area) + "&type=mainvote\">\n";
   foreach (person in NOMIC_HANDLER->query_can_vote_for(area)) {
      ret += "<input type=checkbox name=\"vote_" + person +
           "\" value=1";
      if (people["vote_" + person]) {
         ret += " checked";
      }
      ret += "> " + person + "<br>\n";
   }
   ret += "<input  type=hidden name=type value=mainvote>\n";
   ret += "<input  type=hidden name=area value=\"" + area + "\">\n";
   ret += "<input type=submit name=\"Vote!\">\n";
   ret += "</form>";
   return ret;
}

string show_main_vote(string area, class http_request req) {
   string ret;

   ret = make_header(area);
   ret += "<b>Current Magistrates:</b> " + 
          query_multiple_short(map(NOMIC_HANDLER->query_magistrates(area),
                                   (: "<a href=\"finger.c?player=" + $1 + "\">" +
                                      $1 + "</a>\n" :) )) + "\n";
   ret += "<br>\n";

   // First figure out if there is anything to vote for.
   if (NOMIC_HANDLER->is_citizen_of(area, req->user)) {
      if (NOMIC_HANDLER->is_election_phase(area)) {
         if (NOMIC_HANDLER->has_voted(area, req->user)) {
            ret += "You have already voted in this election.<p>\n";
         } else {
            ret += query_main_vote_page(area, ([ ]));
         }
      } else {
         ret += "Not in the election phase.\n";
      }
   }

   ret += "Show the main <a href=\"nomic_rules.c?type=area&area=" +
          htmlify(area) + "\">" + area + "</a> page.\n<p>";

   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_main_vote() */

string citizen_vote(string area, class http_request req) {
   string ret;
   string person;
   int num;

   ret = make_header(area);
   ret += "<b>Current Magistrates:</b> " + 
          query_multiple_short(map(NOMIC_HANDLER->query_magistrates(area),
                                   (: "<a href=\"finger.c?player=" + $1 + "\">" +
                                      $1 + "</a>\n" :) )) + "\n";
   if (NOMIC_HANDLER->is_citizen_of(area, req->user)) {
      if (NOMIC_HANDLER->is_election_phase(area)) {
         if (NOMIC_HANDLER->has_voted(area, req->user)) {
            ret += "You have already voted in this election.<p>\n";
         } else {
            ret += "<p>You have decided to vote for:\n";
            ret += "<form method=post action=\"nomic_rules.c?area=" +
                   htmlify(area) + "&type=mainvoteconfirm\">\n";
            ret += "<ul>";
            foreach (person in NOMIC_HANDLER->query_can_vote_for(area)) {
               if (req->body->data["vote_" + person]) {
                  ret += "<li><input type=hidden name=\"vote_" + person +
                          "\" value=1>" + person + "<br>";
                  num++;
               }
            }
            ret += "</ul>";
            ret += "<input  type=hidden name=type value=mainvote>\n";
            ret += "<input  type=hidden name=area value=\"" + area + "\">\n";
            if (num > NOMIC_HANDLER->query_number_of_magistrates(area)) {
               ret += "You have voted for too many magistrates.<p></form>";
               // put the voting form in here.
               ret += query_main_vote_page(area, req->body->data);
            } else {
               if (num < NOMIC_HANDLER->query_number_of_magistrates(area)) {
                  ret += "You are voting for " + query_num(num) +
                         " magistrates out of " +
                         query_num(NOMIC_HANDLER->query_number_of_magistrates(area)) +
                         ".<p>";
               }
               ret += "<input type=submit value=\"Confirm\" name=\"Confirm\">\n";
               ret += "</form>";
            }
         }
      } else {
         ret += "Not in the election phase.\n";
      }
   }
   //ret += replace(sprintf("%O", req->body->data), ({ " ", "&nbsp;", "\n", "<br>" }));

   ret += "Show the main <a href=\"nomic_rules.c?type=area&area=" +
          htmlify(area) + "\">" + area + "</a> page.\n<p>";

   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* citizen_vote() */

string citizen_vote_confirm(string area, class http_request req) {
   string ret;
   string person;
   string* voted;

   ret = make_header(area);
   ret += "<b>Current Magistrates:</b> " + 
          query_multiple_short(map(NOMIC_HANDLER->query_magistrates(area),
                                   (: "<a href=\"finger.c?player=" + $1 + "\">" +
                                      $1 + "</a>\n" :) )) + "\n";
   if (NOMIC_HANDLER->is_citizen_of(area, req->user)) {
      if (NOMIC_HANDLER->is_election_phase(area)) {
         if (NOMIC_HANDLER->has_voted(area, req->user)) {
            ret += "You have already voted in this election.<p>\n";
         } else {
            ret += "<p>You have decided to vote for:\n";
            ret += "<ul>";
            voted = ({ });
            foreach (person in NOMIC_HANDLER->query_can_vote_for(area)) {
               if (req->body->data["vote_" + person]) {
                  ret += "<li>" + person;
                  voted += ({ person });
               }
            }
            ret += "</ul>";
            if (sizeof(voted) > NOMIC_HANDLER->query_number_of_magistrates(area)) {
               ret += "You are trying to vote for too many magistrates.\n";
               // put the voting form in here.
               ret += query_main_vote_page(area, req->body->data);
            } else {
               if (NOMIC_HANDLER->vote_for_people(area, voted, req->user)) {
                  ret += "You have voted for " + query_num(sizeof(voted)) +
                         " magistrates out of " +
                         query_num(NOMIC_HANDLER->query_number_of_magistrates(area)) +
                         ".<p>";
               } else {
                  ret += "There was an error trying to vote.<p>";
               }
            }
         }
      } else {
         ret += "Not in the election phase.\n";
      }
   }

   ret += "Show the main <a href=\"nomic_rules.c?type=area&area=" +
          htmlify(area) + "\">" + area + "</a> page.\n<p>";

   //ret += replace(sprintf("%O", req->body->data), ({ " ", "&nbsp;", "\n", "<br>" }));
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* citizen_vote() */

string do_vote(string area, string vote, string id, class http_request req) {
   int vote_id;
   int int_id;
   int bad;
   class nomic_motion motion;
   string ret;

   ret = make_header(area);
   switch (vote) {
   case "yes" :
      vote_id = NOMIC_VOTE_YES;
      break;
   case "no" :
      vote_id = NOMIC_VOTE_NO;
      break;
   case "abstain" :
      vote_id = NOMIC_VOTE_ABSTAIN;
      break;
   default:
      bad = 1;
      break;
   }

   if (sscanf(id, "%d", int_id) != 1 || bad) {
      ret += "<h1>Could not find the thing to vote for " + id + "</h1>";
   } else {
      if (!NOMIC_HANDLER->has_voted_for_motion(area, int_id, 
                        req->user)) {
         // Ok, find the motion.
         motion = NOMIC_HANDLER->query_motion(area, int_id);
         if (!motion) {
            ret += "<h1>Could not find the thing to vote for " + id + "</h1>";
         } else {
            ret += NOMIC_HANDLER->motion_as_html(area, motion);
            if (NOMIC_HANDLER->vote_for_motion(area, int_id,
                     vote_id, req->user)) {
               ret += "<h4>Voted " + vote + " for the above motion.</h4>\n";
            } else {
               ret += "<h4>Failed to vote " + vote +
                      " for the above motion.</h4>\n";
            }
         }
      } else {
         ret += "<h1>Already voted for the motion " + id + "</h1>" +
                NOMIC_HANDLER->motion_as_html(area, motion);
      }
   }
   ret += "<a href=\"nomic_rules.c?type=area&area=" + htmlify(area) +
          "\">Return to " + area + "</a>";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* do_vote() */

string show_all_areas(class http_request req) {
   string ret;
   string area;
   class nomic_motion* motions;

   ret = make_header("Council Areas");
   ret += "<ul>\n";
   foreach (area in NOMIC_HANDLER->query_citizenship_areas()) {
      ret += "<li>" + capitalize(area) + "\n";
      ret += "<font size=-1>[<i><a href=\"nomic_rules.c?type=area&area=" +
                htmlify(area) +
                "\">Rules</a></i>]";
      if (NOMIC_HANDLER->is_citizen_of(area, req->user) &&
          NOMIC_HANDLER->query_citizen_board(area)) {
         if (NOMIC_HANDLER->is_election_phase(area) &&
             !NOMIC_HANDLER->has_voted(area, req->user)) {
            ret += " | <i><a href=\"nomic_rules.c?type=voting&area=" +
                   htmlify(area) + "\">Magistrate Vote</a>";
         }
         motions = NOMIC_HANDLER->query_all_motions(area);
         if (!NOMIC_HANDLER->is_magistrate_of(area, req->user)) {
            motions = filter(motions, (: ((class nomic_motion)$1)->state == NOMIC_STATE_CITIZEN_VOTE :));
         }
         if (sizeof(motions)) {
            ret += " | <i><a href=\"nomic_rules.c?type=motions&area=" +
                   htmlify(area) + "\">Motions</a>";
         }
         ret += " | <i><a href=\"boards.c?type=subject&board=" +
                htmlify(NOMIC_HANDLER->query_citizen_board(area)) +
                "\">Citizen board</a>";
         if (NOMIC_HANDLER->is_magistrate_of(area, req->user) &&
             NOMIC_HANDLER->query_magistrate_board(area)) {
            ret += " | <a href=\"boards.c?type=subject&board=" +
                   htmlify(NOMIC_HANDLER->query_magistrate_board(area)) +
                   "\">Magistrate board</a>";
         }
         ret += "</i>]";
      }

      ret += "</font>";
   }
   ret += "</ul>\n";
   ret += "<h3>Help files relating to the council</h3>\n";
   ret += "<ul>\n";
   ret += "<li><a href=\"http://discworld.imaginary.com:5678/documentation.c?path=/concepts/citizenship\">Citizenship</a> - what citizenship means to you.\n";
   ret += "<li><a href=\"http://discworld.imaginary.com:5678/documentation.c?path=/room/chambers\">Council Chambers</a> - where you vote on new rules.\n";
   ret += "<li><a href=\"http://discworld.imaginary.com:5678/documentation.c?path=/room/citizenship\">Council Citizenship Room</a> - become a citizen today.\n";
   ret += "<li><a href=\"http://discworld.imaginary.com:5678/documentation.c?path=/room/cases\">Courts</a> - where you make a case and see judgements.\n";
   ret += "</ul>\n";
   return ret + ("/www/footer"->www_function("footer", ([ ]), req)) +
          "</body></html>";
} /* show_all_areas() */

// this is the function which the webserver calls
string www_request(string str, mapping args, class http_request req) {
   switch (args["type"]) {
   case "vote" :
      return do_vote(args["area"], args["vote"], args["id"], req);
   case "mainvote" :
      return citizen_vote(args["area"], req);
   case "mainvoteconfirm" :
      return citizen_vote_confirm(args["area"], req);
   case "voting" :
      return show_main_vote(args["area"], req);
   case "area" :
      return show_area(args["area"], req);
   case "motions" :
      return show_motions(args["area"], req);
   default :
      return show_all_areas(req);
   }
}
