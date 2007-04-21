/**
 * This handler is for the control of player houses.
 * @author Pinkfish
 * @started Fri Jun 23 22:50:02 PDT 2000
 */
inherit "/obj/handlers/inherit/nomic_rules";
inherit "/obj/handlers/inherit/case_control";
inherit "/obj/handlers/inherit/citizen_elections";
#define NOMIC_SYSTEM_NO_CLASSES
#include <nomic_system.h>
#include <player.h>

private mapping _motions;
private mapping _new_citizens;
private int _completion_id;

void save_me();
class nomic_motion find_motion(string area, int rule_no, int type);
void update_immutables(string area);
void send_council_inform(string area, int only_magistrates,
                         string mess);
string motion_as_string(int indent, string area, class nomic_motion motion, int brief);

void create() {
   _motions = ([ ]);
   _new_citizens = ([ ]);
   citizen_elections::create();
   nomic_rules::create();
   case_control::create();

   //
   // Check for motion completions at midnight... (well, 2 minutes after).
   //
   _completion_id = call_out("check_for_completions",
       (time() - time() % (24 * 60 * 60) + (24 * 60 * 60) + (2 * 60)) - time());
} /* create() */

/**
 * This method creates a new area.
 * @param area the new area
 */
void create_area(string area) {
   citizen_elections::create_area(area);
   nomic_rules::create_area(area);
   case_control::create_area(area);

   _motions[area] = ({ });
   _new_citizens[area] = ({ });
   update_immutables(area);
} /* create_area() */

/**
 * This method updates the immutable laws with the new set of general rules.
 * @param area the area to update.
 */
void update_immutables(string area) {
   string tmp;
   string type;
   string file;
   int int_type;
   class nomic_rule rule;

   //
   // Get rid of the old immutables first.
   //
   foreach (rule in query_all_nomic_rules(area)) {
      if (rule->type == NOMIC_TYPE_IMMUTABLE) {
         remove_nomic_rule(area, rule->id);
      }
   }

   //
   // Add in all the default rules.
   //
   foreach (file in get_dir(NOMIC_RULE_DEFAULT_DIR)) {
      if (file_size(NOMIC_RULE_DEFAULT_DIR + "/" + file) > 0) {
         tmp = read_file(NOMIC_RULE_DEFAULT_DIR + "/" + file);
         type = explode(tmp, "\n")[0];
         tmp = implode(explode(tmp, "\n")[1..], "\n");
         switch (type) {
         case "immutable" :
            int_type = NOMIC_TYPE_IMMUTABLE;
            break;
         case "citizen" :
            int_type = NOMIC_TYPE_CITIZEN;
            break;
         case "general" :
            int_type = NOMIC_TYPE_GENERAL;
            break;
         default :
            int_type = -1;
            printf("Ignoring file " + file + "\n");
            break;
         }
         if (int_type != -1) {
            add_nomic_rule(area, int_type, area + " administrator", tmp);
         }
      }
   }
   save_me();
} /* create_area() */

private int query_next_motion_id(string area) {
   int num;
   class nomic_motion fluff;

   num = 1;
   foreach (fluff in _motions[area]) {
      if (num <= fluff->identifier) {
         num = fluff->identifier + 1;
      }
   }

   return num;
} /* query_next_motion_id() */

/**
 * This method puts a new rule up for voting.  It is first placed into
 * the hands of the magistrates, then into the hands of the players.
 * @param area the area for the rule to be voted on in
 * @param type the type of the rules
 * @param text the text associated with the new rule
 * @return 1 on success, 0 on failure
 */
int add_create_rule_motion(string area, int type, string text,
                          string creator) {
   class nomic_motion fluff;

   if (!is_magistrate_of(area, creator)) {
      return 0;
   }
   fluff = new(class nomic_motion);
   fluff->motion_type = NOMIC_MOTION_TYPE_RULE_ADD;
   fluff->rule = new(class nomic_rule);
   fluff->rule->creator = creator;
   fluff->rule->text = text;
   fluff->rule->type = type;
   fluff->rule->amendments = ({ });
   fluff->rule->date_created = time();
   fluff->comments = ({ });
   fluff->voted = ({ });
   fluff->state = NOMIC_STATE_COUNCIL_REVIEW;
   fluff->date_added = time();
   fluff->date_event = time();
   fluff->yes_votes = 0;
   fluff->no_votes = 0;
   fluff->added_by = creator;
   fluff->identifier = query_next_motion_id(area);
   _motions[area] += ({ fluff });
   save_me();
   send_council_inform(area, 1, creator + " added a new rule motion");
   post_magistrate_message(area, "New: Create rule motion",
              PLAYER_OB->convert_message(motion_as_string(0, area, fluff, 0)));
   return 1;
} /* add_motion_for_voting() */

/**
 * This method puts up a request to move a rule from one type to another.
 * @param area the area to do the move in
 * @param rule_no the rule number to move
 * @param type the type to move the rule to
 * @param creator the creator of this request
 * @return 1 on success, 0 on failure
 */
int add_move_rule_type_motion(string area, int rule_no, int type,
                              string creator) {
   class nomic_motion fluff;
   class nomic_rule bing;

   bing = query_nomic_rule(area, rule_no);
   if (!bing) {
      return 0;
   }

   if (bing->type == type) {
      return 0;
   }

   //
   // First check and see if are alread moveing this rule.
   //
   fluff = find_motion(area, rule_no, NOMIC_MOTION_TYPE_RULE_MOVE);
   if (fluff) {
      return 0;
   }

   fluff = new(class nomic_motion);
   fluff->motion_type = NOMIC_MOTION_TYPE_RULE_MOVE;
   fluff->rule = bing;
   fluff->new_rule_type = type;
   fluff->comments = ({ });
   fluff->voted = ({ });
   fluff->state = NOMIC_STATE_COUNCIL_REVIEW;
   fluff->date_added = time();
   fluff->date_event = time();
   fluff->yes_votes = 0;
   fluff->no_votes = 0;
   fluff->added_by = creator;
   fluff->identifier = query_next_motion_id(area);
   _motions[area] += ({ fluff });
   save_me();
   send_council_inform(area, 1, creator + " added a move rule motion");
   post_magistrate_message(area, "New: Move rule motion",
              PLAYER_OB->convert_message(motion_as_string(0, area, fluff, 0)));
   return 1;
} /* add_move_rule_type() */

/**
 * This method adds in a request to amend a rule.
 * @param area the area to put the amendment in
 * @param rule_no the rule to amend
 * @param amendment the amendment text
 * @param creator the person doing the amendment
 * @return 1 on success, 0 on failure
 */
int add_amend_rule_motion(string area, int rule_no, string amendment,
                           string creator) {
   class nomic_motion fluff;
   class nomic_rule bing;
   class nomic_amendment amend;

   bing = query_nomic_rule(area, rule_no);
   if (!bing) {
      return 0;
   }

   //
   // First check and see if are already amending this rule.
   //
   fluff = find_motion(area, rule_no, NOMIC_MOTION_TYPE_RULE_AMEND);
   if (fluff) {
      return 0;
   }

   fluff = new(class nomic_motion);
   fluff->motion_type = NOMIC_MOTION_TYPE_RULE_AMEND;
   amend = new (class nomic_amendment);
   amend->amender = creator;
   amend->text = amendment;
   amend->date_amended = time();
   bing->amendments += ({ amend });
   fluff->rule = bing;
   fluff->comments = ({ });
   fluff->voted = ({ });
   fluff->state = NOMIC_STATE_COUNCIL_REVIEW;
   fluff->date_added = time();
   fluff->date_event = time();
   fluff->yes_votes = 0;
   fluff->no_votes = 0;
   fluff->added_by = creator;
   fluff->identifier = query_next_motion_id(area);
   _motions[area] += ({ fluff });
   save_me();
   send_council_inform(area, 1, creator + " added an add rule motion");
   post_magistrate_message(area, "New: Ammend rule motion",
              PLAYER_OB->convert_message(motion_as_string(0, area, fluff, 0)));
   return 1;
} /* add_rule_amendment() */

/**
 * This method remove a request to amend a rule.
 * @param area the area to put the amendment in
 * @param rule_no the rule to amend
 * @param amendment the amendment text
 * @param creator the person doing the amendment
 * @return 1 on success, 0 on failure
 */
int remove_rule_motion(string area, int rule_no, string creator) {
   class nomic_motion fluff;
   class nomic_rule bing;

   bing = query_nomic_rule(area, rule_no);
   if (!bing) {
      return 0;
   }

   //
   // First check and see if are alread removeing this rule.
   //
   fluff = find_motion(area, rule_no, NOMIC_MOTION_TYPE_RULE_REMOVE);
   if (fluff) {
      return 0;
   }

   fluff = new(class nomic_motion);
   fluff->motion_type = NOMIC_MOTION_TYPE_RULE_REMOVE;
   fluff->rule = bing;
   fluff->comments = ({ });
   fluff->voted = ({ });
   fluff->state = NOMIC_STATE_COUNCIL_REVIEW;
   fluff->date_added = time();
   fluff->date_event = time();
   fluff->yes_votes = 0;
   fluff->no_votes = 0;
   fluff->added_by = creator;
   fluff->identifier = query_next_motion_id(area);
   _motions[area] += ({ fluff });
   save_me();
   send_council_inform(area, 1, creator + " added a remove rule motion");
   post_magistrate_message(area, "New: Remove rule motion",
              PLAYER_OB->convert_message(motion_as_string(0, area, fluff, 0)));
   return 1;
} /* remove_rule_motion() */

/**
 * This method returns a nice new rule.
 * @param area the areat to look in
 * @param id the id of the new rule to find
 * @return 1 on success, 0 on failure
 */
class nomic_motion query_motion(string area, int id) {
   class nomic_motion fluff;

   if (!_motions[area]) {
      return 0;
   }

   foreach (fluff in _motions[area]) {
      if (fluff->identifier == id) {
         return fluff;
      }
   }

   return 0;
} /* query_motion() */

/**
 * This method returns all the new rules for the specified area.
 * @param area the area to return the new rules in
 * @return the array of new rules
 */
class nomic_motion* query_all_motions(string area) {
   if (!_motions[area]) {
      return ({ });
   }

   return copy(_motions[area]);
} /* query_all_motions() */

/**
 * This method finds the specified motion.
 * @param area the area to look in
 * @param rule_no the rule number
 * @param type the type of the motion
 * @return the motion if successful, 0 if not
 */
class nomic_motion find_motion(string area, int rule_no, int type) {
   class nomic_motion motion;

   foreach (motion in _motions[area]) {
      if (motion->rule->id == rule_no &&
          motion->motion_type == type) {
         return motion;
      }
   }
   return 0;
} /* find_motion() */

/**
 * This method adds a comment to the new rule/amendment/whatever.
 * @param area the area the comment is to go in
 * @param id the rule update id
 * @param comment the comment to make
 * @param commenter the person commenting
 * @return 1 on success, 0 on failure
 */
int comment_on_motion(string area, int id, string comment_text,
                            string commenter) {
   class nomic_comment comment; 
   class nomic_motion bing;
   
   bing = query_motion(area, id);
   if (!bing) {
      return 0;
   }

   if (bing->state != NOMIC_STATE_COUNCIL_REVIEW &&
       bing->state != NOMIC_STATE_COUNCIL_VOTE) {
      return 0;
   }

   comment = new(class nomic_comment);
   comment->text = comment_text;
   comment->commenter = commenter;
   comment->date_commented = time();
   bing->comments += ({ comment });
   save_me();
   return 1;
} /* add_motion_update_comment() */

/**
 * This method adds an amendment to the new rule/amendment/whatever, this
 * rewrites the text of the ammendment of whatever it is.
 * @param area the area the amendmend is to go in
 * @param id the rule update id
 * @param amendmend the amendmend to make
 * @param amendmender the person amendmending
 * @return 1 on success, 0 on failure
 */
int amend_motion(string area, int id, string amend_text,
                    string amender) {
   //class nomic_amendment amend;
   class nomic_motion bing;
   
   bing = query_motion(area, id);
   if (!bing) {
      return 0;
   }

   // This only works in the review phase.
   if (bing->state != NOMIC_STATE_COUNCIL_REVIEW) {
      return 0;
   }

/*
   amend = new(class nomic_amendment);
   amend->text = amend_text;
   amend->amender = amender;
   amend->date_amended = time();
   bing->rule->amendments += ({ amend });
 */
   switch (bing->motion_type) {
   case NOMIC_MOTION_TYPE_RULE_ADD :
      bing->rule->text = amend_text;
      break;
   case NOMIC_MOTION_TYPE_RULE_REMOVE :
   case NOMIC_MOTION_TYPE_RULE_MOVE :
      return 0;
   case NOMIC_MOTION_TYPE_RULE_AMEND :
      bing->rule->amendments[<1]->text = amend_text;
   }
   save_me();
   return 1;
} /* add_motion_update_comment() */

/**
 * This method votes for the specified ammentment/whatever.
 * @param area the area to vote in
 * @param id the id to vote for
 * @param vote_type the type of the vote
 * @param voter the person is voting
 * @return 1 on success, 0 on failure
 */
int vote_for_motion(string area, int id, int vote_type, string voter) {
   class nomic_motion bing;

   bing = query_motion(area, id);
   if (!bing) {
      return 0;
   }

   if (member_array(voter, bing->voted) != -1) {
      return 0;
   }

   if (bing->state != NOMIC_STATE_COUNCIL_VOTE &&
       bing->state != NOMIC_STATE_CITIZEN_VOTE) {
      return 0;
   }

   switch (vote_type) {
   case NOMIC_VOTE_YES :
      bing->yes_votes++;
      break;
   case NOMIC_VOTE_NO :
      bing->no_votes++;
      break;
   case NOMIC_VOTE_ABSTAIN :
      bing->abstain_votes++;
      break;
   default :
      return 0;
   }
   bing->voted += ({ voter });
   save_me();
   return 1;
} /* vote_for_motion() */

/**
 * This method checks to see if the person has voted for the specified
 * new rule/amendment/whatever
 * @param area the area to check
 * @param id the id to check
 * @param voter the person to check for voting
 * @return 1 on success, 0 on failure
 */
int has_voted_for_motion(string area, int id, string voter) {
   class nomic_motion bing;

   bing = query_motion(area, id);
   if (!bing) {
      return 0;
   }

   return member_array(voter, bing->voted) != -1;
} /* has_voted_for_motion() */

/**
 * This method returns the current set of votes for the specied new rule.
 * @param area the area to lookup the votes in
 * @param id the idea to check
 * @return ({ yes, no, abstain })
 */
int* query_votes_for_motion(string area, int id) {
   class nomic_motion bing;

   bing = query_motion(area, id);
   if (!bing) {
      return 0;
   }

   return ({ bing->yes_votes, bing->no_votes, bing->abstain_votes });
} /* query_voted_for_motions() */

private void remove_motion(string area, class nomic_motion motion) {
   int i;

   for (i = 0; i < sizeof(_motions[area]); i++) {
      if (_motions[area][i] == motion) {
         _motions[area] = _motions[area][0..i-1] + _motions[area][i+1..];
      }
   }
} /* remove_motion() */

/**
 * This method sends informs to all the specified council of some sort of
 * nifty council event.
 * @param area the area the council is for
 * @param only_magistrates only send it to the magistrates?
 * @param mess the message to send
 */
void send_council_inform(string area, int only_magistrates,
                         string mess) {
   object *obs;

   if (only_magistrates) {
      obs = filter(users(), (: is_magistrate_of($2, $1->query_name()) :), area);
   } else {
      obs = filter(users(), (: is_citizen_of($2, $1->query_name()) :), area);
   }
   call_other(obs, "event_inform", this_object(), mess, "council");
} /* send_council_inform() */

/**
 * This method figures out the compleion time for the specified motion.
 * @param area the area to check
 * @param the motion in the area
 * @return the completion time
 */
int query_completion_time(string area, class nomic_motion motion) {
   return (motion->date_event - motion->date_event % (24 * 60 * 60) +
           7 * (24 * 60 * 60));
} /* query_completion_time() */

/**
 * This method completes the vote on the specified new rule.
 * @param area the area to complete the rule in
 * @param id the id to complete
 * @return 1 on success, 0 on failure
 */
int complete_motion(string area, int id) {
   class nomic_motion bing;
   class nomic_motion motion;
   int passed;
   int new_num;
   string mess;
   int only_magistrates;
   string voting_result;

   bing = query_motion(area, id);
   if (!bing) {
      return 0;
   }

   passed = bing->yes_votes > bing->no_votes;

   voting_result = "Voting Results:\n"
                   "    Yes: " + bing->yes_votes + "\n"
                   "     No: " + bing->no_votes + "\n"
                   "Abstain: " + bing->abstain_votes + "\n";
   mess = "Motion added by " + bing->added_by + " to ";
   switch (bing->motion_type) {
   case NOMIC_MOTION_TYPE_RULE_ADD :
      mess += "add a new rule";
      break;
   case NOMIC_MOTION_TYPE_RULE_REMOVE :
      mess += "remove rule " + bing->rule->id;
      break;
   case NOMIC_MOTION_TYPE_RULE_MOVE :
      mess += "move rule " + bing->rule->id;
      break;
   case NOMIC_MOTION_TYPE_RULE_AMEND :
      mess += "amend rule " + bing->rule->id;
      break;
   }
   if (passed || bing->state == NOMIC_STATE_COUNCIL_REVIEW) {
      switch (bing->state) {
      case NOMIC_STATE_COUNCIL_REVIEW :
         bing->yes_votes = 0;
         bing->no_votes = 0;
         bing->abstain_votes = 0;
         bing->voted = ({ });
         bing->state = NOMIC_STATE_COUNCIL_VOTE;
         mess += " is open for magistrates to vote";
         only_magistrates = 1;
         post_magistrate_message(area, "Motion now in voting phase",
              PLAYER_OB->convert_message(motion_as_string(0, area, bing, 0)));
         break;
      case NOMIC_STATE_COUNCIL_VOTE :
         //
         // Pass this and set all the defaults back up.
         //
         bing->yes_votes = 0;
         bing->no_votes = 0;
         bing->abstain_votes = 0;
         bing->voted = ({ });
         bing->state = NOMIC_STATE_CITIZEN_VOTE;
         bing->comments = ({ });
         mess += " is open for citizens to vote";
         post_magistrate_message(area, "Law passed onto citizens",
                    PLAYER_OB->convert_message(rule_as_string(0, bing->rule)) +
                              "\n\n" + voting_result);
         post_citizen_message(area, "New motion for voting on",
              PLAYER_OB->convert_message(motion_as_string(0, area, bing, 0)));
         break;
      case NOMIC_STATE_CITIZEN_VOTE :
         switch (bing->motion_type) {
         case NOMIC_MOTION_TYPE_RULE_ADD :
            add_nomic_rule_class(area, bing->rule, bing->motion_type);
            break;
         case NOMIC_MOTION_TYPE_RULE_AMEND :
            change_nomic_rule(area, bing->rule);
            break;
         case NOMIC_MOTION_TYPE_RULE_MOVE :
            new_num = move_nomic_rule(area, bing->rule, bing->new_rule_type);
            foreach (motion in _motions[area]) {
               if (motion->rule->id == bing->rule->id) {
                  motion->rule->id = new_num;
               }
            }
            break;
         case NOMIC_MOTION_TYPE_RULE_REMOVE :
            remove_nomic_rule(area, bing->rule->id);
            break;
         }
         remove_motion(area, bing);
         mess += " is now law";
         post_citizen_message(area, "New Law!",
                    PLAYER_OB->convert_message(rule_as_string(0, bing->rule)));
         post_magistrate_message(area, "New Law!",
                    PLAYER_OB->convert_message(rule_as_string(0, bing->rule)) +
                    "\n\n" + voting_result);
         break;
      }
      bing->date_event = time();
   } else {
      if (bing->state == NOMIC_STATE_COUNCIL_VOTE) {
         mess += " not passed by the magistrates";
         only_magistrates = 1;
         post_magistrate_message(area, "Law not passed onto citizens",
                              rule_as_string(0, bing->rule) + "\n\n" +
                              voting_result);
      } else {
         mess += " not ratified by the citizens";
         only_magistrates = 0;
         post_citizen_message(area, "Law not passed",
                              rule_as_string(0, bing->rule));
         post_magistrate_message(area, "Law not passed",
                              rule_as_string(0, bing->rule) + "\n\n" +
                              voting_result);
      }
      remove_motion(area, bing);
   }

   save_me();
   send_council_inform(area, only_magistrates, mess);
   return 1;
} /* complete_motion() */

/**
 * This method figures out all the current things and when they completed.
 */
void check_for_completions() {
   string area;
   class nomic_motion* bits;
   class nomic_motion motion;

   foreach (area, bits in _motions) {
      foreach (motion in bits) {
         if (query_completion_time(area, motion) < time()) {
            complete_motion(area, motion->identifier);
         }
      }
   }

   remove_call_out(_completion_id);
   _completion_id = call_out("check_for_completions",
            (time() - time() % (24 * 60 * 60) + (24 * 60 * 60)) - time());

   //
   // Check the cases to see if they want to mess around too.
   //
   update_recently_closed_cases();
} /* check_for_completions() */

/**
 * This method returns the new motion as a string.
 * @param indent the indentation to putin front of all the lines
 * @param area the area the motion is defined in
 * @param motion the new motion to turn into a styring
 * @param brief disable comments
 * @return the string version of the motion
 */
string motion_as_string(int indent, string area, class nomic_motion motion,
                        int brief) {
   string ret;
   class nomic_comment comment;

   ret = "";
   switch (motion->motion_type) {
   case NOMIC_MOTION_TYPE_RULE_ADD :
      ret += "$I$" + (indent + 5) + "=" + sprintf("%*s", indent, "") +
             motion->identifier + ") Add new rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "\n";
      break;
   case NOMIC_MOTION_TYPE_RULE_REMOVE :
      ret += "$I$" + (indent + 5) + "=" + sprintf("%*s", indent, "") +
             motion->identifier + ") Remove rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "\n";
      break;
   case NOMIC_MOTION_TYPE_RULE_MOVE :
      ret += "$I$" + (indent + 5) + "=" + sprintf("%*s", indent, "") +
             motion->identifier + ") Transmogrify rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "\n";
      break;
   case NOMIC_MOTION_TYPE_RULE_AMEND :
      ret += "$I$" + (indent + 5) + "=" + sprintf("%*s", indent, "") +
             motion->identifier + ") Amend rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "\n";
      break;
   }

   ret += rule_as_string(5 + indent, motion->rule);
   if (sizeof(motion->comments)) {
      if (!brief) {
         ret += "$I$" + (indent + 5) + "=     Comments:\n";
         foreach (comment in motion->comments) {
            ret += "$I$" + (indent + 11) + "=" + sprintf("%*s", indent + 3, "") +                capitalize(comment->commenter) + " (" +
                   ctime(comment->date_commented) + "):\n" +
                   comment->text + "\n";
         }
      } else {
         ret += "$I$0=" + sprintf("%*s", indent, "") + "     " +
                 sizeof(motion->comments) + " Comments.\n";
      }
   } else {
      ret += "$I$0=" + sprintf("%*s", indent, "") + "     No Comments.\n";
   }

   switch (motion->state) {
   case NOMIC_STATE_COUNCIL_REVIEW :
      ret += "$I$0=" + sprintf("%*s", indent, "") +
             "The item is currently being reviewed by the magistrates.\n";
      break;
   case NOMIC_STATE_COUNCIL_VOTE :
      ret += "$I$0=" + sprintf("%*s", indent, "") +
             "The item is currently being voted on by the magistrates.\n";
      break;
   case NOMIC_STATE_CITIZEN_VOTE :
      ret += "$I$0=" + sprintf("%*s", indent, "") +
             "The item is currently being ratified by the citizens.\n";
      break;
   }

   //
   // The next stage!
   //
   ret += "$I$0=" + sprintf("%*s", indent, "") +
          "The item will go onto the next stage at " +
          ctime(query_completion_time(area, motion)) +
          ".\n";

   return ret;
} /* motion_as_string() */

/**
 * This method turns a rule into html.
 * @param rule the rule to turn into html
 * @return the rule as a string
 */
string rule_as_html(class nomic_rule rule) {
   string ret;
   class nomic_amendment amend;

   ret = "<i><b>" + (rule->id?rule->id+"":"(" +
          NOMIC_HANDLER->query_type_name(rule->type) + ")") + "</b> by " +
          capitalize(rule->creator) + " (" +
          ctime(rule->date_created) + "):</i>\n"
          "<p style=\"margin-left: 30pt;margin-top: 5pt\">\n" +
          replace_string(rule->text, "\n", "<br>") +
          "<br>\n";
   foreach (amend in rule->amendments) {
      ret += "<i><b>Ammendment</b> by " + capitalize(amend->amender) + " (" +
             ctime(amend->date_amended) + "):</i>\n"
             "<p style=\"margin-left: 60pt;margin-top: 5pt\">\n" +
             replace_string(amend->text, "\n", "<br>") +
             "\n</p>\n";
   }

   ret += "</p>\n";
   return ret;
} /* rule_as_html() */

/**
 * This method returns the new motion as a string.
 * @param area the area the motion is defined in
 * @param motion the new motion to turn into a styring
 * @return the string version of the motion
 */
string motion_as_html(string area, class nomic_motion motion) {
   string ret;
   class nomic_comment comment;

   ret = "";
   switch (motion->motion_type) {
   case NOMIC_MOTION_TYPE_RULE_ADD :
      ret += "<b>" + motion->identifier + ") Add new rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "</b>\n";
      break;
   case NOMIC_MOTION_TYPE_RULE_REMOVE :
      ret += "<b>" + motion->identifier + ") Remove rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "</b>\n";
      break;
   case NOMIC_MOTION_TYPE_RULE_MOVE :
      ret += "<b>" + motion->identifier + ") Transmogrify rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "</b>\n";
      break;
   case NOMIC_MOTION_TYPE_RULE_AMEND :
      ret += "<b>" + motion->identifier + ") Amend rule; added by " +
             capitalize(motion->added_by) + " at " +
             ctime(motion->date_added) + "</b>\n";
      break;
   }

   ret += "<ul>" + rule_as_html(motion->rule) + "</ul>";
   if (sizeof(motion->comments)) {
      ret += "<b>Comments:</b><br>\n";
      foreach (comment in motion->comments) {
         ret += ctime(comment->date_commented) + "):<br>\n" +
                comment->text + "<p>\n";
      }
   }

   switch (motion->state) {
   case NOMIC_STATE_COUNCIL_REVIEW :
      ret += "The item is currently being reviewed by the magistrates.\n";
      break;
   case NOMIC_STATE_COUNCIL_VOTE :
      ret += "The item is currently being voted on by the magistrates.\n";
      break;
   case NOMIC_STATE_CITIZEN_VOTE :
      ret += "The item is currently being ratified by the citizens.\n";
      break;
   }

   //
   // The next stage!
   //
   ret += "<br>The item will go onto the next stage at " +
          ctime(query_completion_time(area, motion)) +
          ".\n";

   return ret;
} /* motion_as_string() */
