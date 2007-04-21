#include <nomic_system.h>

inherit "/std/room/basic_room";
inherit "/std/room/inherit/council_base";


void create() {
   ::create();
} /* create() */

/**
 * This method lists the current set of rules from the specified area.
 * @return 1 on success, 0 on failure
 */
int do_list_rules() {
   class nomic_rule* rules;
   class nomic_rule rule;
   string ret;
   int cur_type;

   rules = NOMIC_HANDLER->query_all_nomic_rules(query_council_area());
   if (!sizeof(rules)) {
      add_failed_mess("There are no rules, oh no!  Lawlessness!\n");
      return 0;
   }

   rules = sort_array(rules, (: $1->id - $2->id :) );

   ret = "";
   cur_type = -1;
   foreach (rule in rules) {
      if (rule->type != cur_type) {
         switch (rule->type) {
         case NOMIC_TYPE_IMMUTABLE :
            ret += "%^BOLD%^Immutable Rules%^RESET%^\n";
            break;
         case NOMIC_TYPE_CITIZEN :
            ret += "%^BOLD%^Citizen Rules%^RESET%^\n";
            break;
         case NOMIC_TYPE_GENERAL :
            ret += "%^BOLD%^General Rules%^RESET%^\n";
            break;
         }
         cur_type = rule->type;
      }
      ret += NOMIC_HANDLER->rule_as_string(0, rule);
      ret += "\n";
   }
   write("$P$Nomic rules$P$" + ret);
   return 1;
} /* do_list_rules() */


/**
 * This method lists the current set of rules from the specified area.
 * @return 1 on success, 0 on failure
 */
int do_list_one_rule(int num) {
   class nomic_rule rule;
   string ret;

   rule = NOMIC_HANDLER->query_nomic_rule(query_council_area(), num);
   if (!rule) {
      add_failed_mess("There is no rule number " + num + ".\n");
      return 0;
   }

   ret = "";
   switch (rule->type) {
   case NOMIC_TYPE_IMMUTABLE :
      ret += "%^BOLD%^Immutable Rules%^RESET%^\n";
      break;
   case NOMIC_TYPE_CITIZEN :
      ret += "%^BOLD%^Citizen Rules%^RESET%^\n";
      break;
   case NOMIC_TYPE_GENERAL :
      ret += "%^BOLD%^General Rules%^RESET%^\n";
      break;
   }
   ret += NOMIC_HANDLER->rule_as_string(0, rule);
   ret += "\n";
   write("$P$Nomic rules$P$" + ret);
   return 1;
} /* do_list_rules() */

/**
 * This lists all the new rules up for discussion or whatever by the
 * magistrates.
 * @return 1 on success, 0 on failure
 */
int do_list_single_motion(int num) {
   class nomic_motion* rules;
   class nomic_motion rule;
   int is_mag;
   string ret;

   rules = NOMIC_HANDLER->query_all_motions(query_council_area());
   is_mag = NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name());
   if (!is_mag) {
      rules = filter(rules, (: $1->state == NOMIC_STATE_CITIZEN_VOTE :));
   }

   if (!sizeof(rules)) {
      add_failed_mess("No motion to look at or vote on.\n");
      return 0;
   }

   ret = "";
   foreach (rule in rules) {
      if (rule->identifier == num) {
         ret += NOMIC_HANDLER->motion_as_string(0, query_council_area(), rule,
                                             0);
         ret += "\n";
      }
   }

   if (ret == "") {
      add_failed_mess("The motion " + num + " was not found.\n");
      return 0;
   }

   write("$P$New Nomic Rules$P$" + ret);
   return 1;
} /* do_list_single_motion() */

/**
 * This lists all the new rules up for discussion or whatever by the
 * magistrates.
 * @return 1 on success, 0 on failure
 */
int do_list_motions(int brief) {
   class nomic_motion* rules;
   class nomic_motion rule;
   int is_mag;
   string ret;

   rules = NOMIC_HANDLER->query_all_motions(query_council_area());
   is_mag = NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name());
   if (!is_mag) {
      rules = filter(rules, (: $1->state == NOMIC_STATE_CITIZEN_VOTE :));
   }

   if (!sizeof(rules)) {
      add_failed_mess("No motion to look at or vote on.\n");
      return 0;
   }

   ret = "";
   foreach (rule in rules) {
      ret += NOMIC_HANDLER->motion_as_string(0, query_council_area(), rule,
                                             brief);
      ret += "\n";
   }
   write("$P$New Nomic Rules$P$" + ret);
   return 1;
} /* do_list_motions() */

/**
 * This method allows the person to make a comment on a current rule.
 * Comments are only allowed by magistrates and only in the first two
 * phases.
 * @param num the id of the rule to comment on
 * @return 1 on success, 0 on failure
 */
int do_comment_on(int num) {
   class nomic_motion rule;
   int is_mag;

   rule = NOMIC_HANDLER->query_motion(query_council_area(), num);
   is_mag = NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name());
   if (!rule || !is_mag && rule->state != NOMIC_STATE_CITIZEN_VOTE) {
      add_failed_mess("Could not find the motion " + num + ".\n");
      return 0;
   }
   write("You are commenting on:\n" +
         NOMIC_HANDLER->motion_as_string(0, query_council_area(), rule) +
         "\n");
   this_player()->do_edit("", "finish_comment_on", this_object(), 0, rule);
   return 1;
} /* do_comment_on() */

/** @ignore yes */
void finish_comment_on(string comment, class nomic_motion rule) {
   if (!comment) {
      write("Aborting comment.\n");
      return ;
   }
   if (NOMIC_HANDLER->comment_on_motion(query_council_area(), rule->identifier,
                                       comment, this_player()->query_name())) {
      write("Succeeded in making a comment on the rule.\n");
   } else {
      write("Could not make a comment on the rule for some reason.\n");
   }
} /* finish_comment_on() */

/**
 * This method allows the person to amend a new rule.
 * Amendments are only allowed by magistrates and only during the discusion
 * phase.
 * @param num the id of the rule to comment on
 * @return 1 on success, 0 on failure
 */
int do_amend_motion(int num) {
   class nomic_motion rule;
   int is_mag;

   is_mag = NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name());
   if (!is_mag) {
      add_failed_mess("You cannot amend rules.\n");
      return 0;
   }

   rule = NOMIC_HANDLER->query_motion(query_council_area(), num);
   if (!rule) {
      add_failed_mess("Could not find the motion " + num + ".\n");
      return 0;
   }
   if (rule->state != NOMIC_STATE_COUNCIL_REVIEW) {
      add_failed_mess("You cannot amend the motion " + num + ".\n");
      return 0;
   }
   if (rule->motion_type != NOMIC_MOTION_TYPE_RULE_ADD &&
       rule->motion_type != NOMIC_MOTION_TYPE_RULE_AMEND) {
      add_failed_mess("You cannot amend the motion " + num + ".\n");
      return 0;
   }

   write("You are adding an amendment to:\n" +
         NOMIC_HANDLER->motion_as_string(0, query_council_area(), rule) +
         "\n\nWARNING!  Ammending the motion replaces the current text.\n");
   this_player()->do_edit("", "finish_amend_motion", this_object(), 0, rule);
   return 1;
} /* do_amend_motion() */

/** @ignore yes */
void finish_amend_motion(string amendment, class nomic_motion rule) {
   if (!amendment) {
      write("Aborting amendment.\n");
      return ;
   }
   if (NOMIC_HANDLER->amend_motion(query_council_area(), rule->identifier,
                                   amendment, this_player()->query_name())) {
      write("Succeeded in adding an amendment to the motion.\n");
   } else {
      write("Could not add an amendment to the rule for some reason.\n");
   }
} /* finish_amend_motion() */

/**
 * This method will handle voting on the specific new nomic rule.
 * @param num the rule number to vote on
 * @param vote their vote
 * @return 1 on success, 0 on failure
 */
int do_vote_on(int num, string vote) {
   class nomic_motion rule;
   int is_mag;
   int vote_id;

   rule = NOMIC_HANDLER->query_motion(query_council_area(), num);
   is_mag = NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name());
   if (!rule || (!is_mag && rule->state != NOMIC_STATE_CITIZEN_VOTE)) {
      add_failed_mess("Could not find the motion " + num + ".\n");
      return 0;
   }

   if (rule->state != NOMIC_STATE_CITIZEN_VOTE &&
       rule->state != NOMIC_STATE_COUNCIL_VOTE) {
      add_failed_mess("The rule is not in a state that you can vote on.\n");
      return 0;
   }

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
   default :
      add_failed_mess("Unknown type of vote, must be one of yes, no or "
                      "abstain.\n");
      return 0;
   }

   if (NOMIC_HANDLER->has_voted_for_motion(query_council_area(), num,
                     this_player()->query_name())) {
      add_failed_mess("You have already voted for this rule!\n");
      return 0;
   } else if (NOMIC_HANDLER->vote_for_motion(query_council_area(), num, vote_id,
                     this_player()->query_name())) {
      add_succeeded_mess(({ "$N $V for " + vote + " in motion id " + num +
                            ".\n",
                            "$N $V for a motion.\n" }));
      return 1;
   }
   add_failed_mess("Could not vote for the rule for some reason.\n");
   return 0;
} /* do_vote_on() */

/**
 * This method adds an amendment to a current rule.
 * @param num the number of the current rule
 * @return 1 on success, 0 on failure
 */
int do_amend_current_rule(int num) {
   class nomic_rule rule;

   rule = NOMIC_HANDLER->query_nomic_rule(query_council_area(), num);
   if (!rule) {
      add_failed_mess("The rule " + num + " does not exist.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name())) {
      add_failed_mess("You cannot do this until you are a magistrate.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->query_can_change_rule(query_council_area(), rule)) {
      add_failed_mess("You cannot change that rule.\n");
      return 0;
   }

   write("What amendment would you like to make to the rule:\n" +
         NOMIC_HANDLER->rule_as_string(0, rule) + "\n");
   this_player()->do_edit("", "amend_current_rule", this_object(), 0, rule);
   return 1;
} /* do_amend_current_rule() */

/** @ignore yes */
void amend_current_rule(string str, class nomic_rule rule) {
   if (!str) {
      write("Aborting the addition of an amendment.\n");
      return ;
   }
   write("Adding the amendment:\n" +
         str + "\nTo the rule:\n" +
         NOMIC_HANDLER->rule_as_string(0, rule) + "\nAre you sure you wish to do this? ");
   input_to("amend_current_rule_check", 0, rule, str);
} /* amend_current_rule() */

/** @ignore yes */
void amend_current_rule_check(string str,
                               class nomic_rule rule,
                               string amend) {
   str = lower_case(str);
   if (!strlen(str) || str[0] != 'y') {
      write("Aborting the addition of an amendment.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->add_amend_rule_motion(query_council_area(), rule->id, amend,
                                          this_player()->query_name())) {
      write("Cannot add the rule amendment.\n");
   } else {
      write("Added a motion to amend the rule.\n");
   }
} /* amend_current_rule_check() */

/**
 * This method removes a current rule.
 * @param num the number of the current rule
 * @return 1 on success, 0 on failure
 */
int do_remove_current_rule(int num) {
   class nomic_rule rule;

   rule = NOMIC_HANDLER->query_nomic_rule(query_council_area(), num);
   if (!rule) {
      add_failed_mess("The rule " + num + " does not exist.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name())) {
      add_failed_mess("You cannot do this until you are a magistrate.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->query_can_change_rule(query_council_area(), rule)) {
      add_failed_mess("You cannot change that rule.\n");
      return 0;
   }

   write("Would you like to remove the rule:\n" +
         NOMIC_HANDLER->rule_as_string(0, rule) +
         "\nYou must be sure about removing this.   Do you wish to "
         "remove the rule? ");
   input_to("remove_current_rule_check", 0, rule);
   return 1;
} /* do_remove_current_rule() */

/** @ignore yes */
void remove_current_rule_check(string str,
                               class nomic_rule rule) {
   str = lower_case(str);
   if (!strlen(str) || str[0] != 'y') {
      write("Aborting the removal of the rule.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->add_remove_rule_motion(query_council_area(), rule->id,
                                          this_player()->query_name())) {
      write("Cannot remove the rule.\n");
   } else {
      write("Added a motion to remove the rule.\n");
   }
} /* remove_current_rule_check() */

/**
 * This method creates a new rule.
 * @param type the type of the new rule
 * @return 1 on success, 0 on failure
 */
int do_create_motion(string type) {
   int type_no;

   type_no = NOMIC_HANDLER->query_type_number(type);
   if (type_no == NOMIC_ERROR ||
       type_no == NOMIC_TYPE_IMMUTABLE) {
      add_failed_mess("The type " + type + " is invalid.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name())) {
      add_failed_mess("You cannot do this until you are a magistrate.\n");
      return 0;
   }

   write("What rule would you like to create?\n");
   this_player()->do_edit("", "create_motion", this_object(), 0, type_no);
   return 1;
} /* do_create_motion() */

/** @ignore yes */
void create_motion(string str, int type_no) {
   if (!str) {
      write("Aborting the creation of the rule.\n");
      return ;
   }
   write("Adding the motion of type " + 
         NOMIC_HANDLER->query_type_name(type_no) + ":\n" +
         str +
         "\nAre you sure you wish to do this? ");
   input_to("create_motion_check", 0, type_no, str);
} /* create_motion() */

/** @ignore yes */
void create_motion_check(string str,
                         int type_no,
                         string motion) {
   str = lower_case(str);
   if (!strlen(str) || str[0] != 'y') {
      write("Aborting the creation of the rule.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->add_create_rule_motion(query_council_area(), type_no, motion,
                                          this_player()->query_name())) {
      write("Cannot create the rule.\n");
   } else {
      write("Added a motion to create the rule.\n");
   }
} /* create_motion_check() */

/**
 * This method transmogrifies a current rule.
 * @param num the number of the current rule
 * @return 1 on success, 0 on failure
 */
int do_transmogrify_current_rule(int num, string new_type) {
   class nomic_rule rule;
   int new_type_no;

   if (!NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                      this_player()->query_name())) {
      add_failed_mess("You cannot do this until you are a magistrate.\n");
      return 0;
   }

   rule = NOMIC_HANDLER->query_nomic_rule(query_council_area(), num);
   if (!rule) {
      add_failed_mess("The rule " + num + " does not exist.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->query_can_change_rule(query_council_area(), rule)) {
      add_failed_mess("You cannot change that rule.\n");
      return 0;
   }

   new_type_no = NOMIC_HANDLER->query_type_number(new_type);
   if (new_type_no == NOMIC_ERROR ||
       new_type_no == NOMIC_TYPE_IMMUTABLE) {
      add_failed_mess("The type " + new_type + " is not valid.\n");
      return 0;
   }

   write("Would you like to transmogrify the rule:\n" +
         NOMIC_HANDLER->rule_as_string(0, rule) +
         "\nYou must be sure about transmogrifying this.   Do you wish to "
         "transmogrify the rule? ");
   input_to("transmogrify_current_rule_check", 0, rule, new_type_no);
   return 1;
} /* do_transmogrify_current_rule() */

/** @ignore yes */
void transmogrify_current_rule_check(string str,
                               class nomic_rule rule,
                               int new_type_no) {
   str = lower_case(str);
   if (!strlen(str) || str[0] != 'y') {
      write("Aborting the removal of the rule.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->add_move_rule_type_motion(query_council_area(), rule->id,
                                          new_type_no,
                                          this_player()->query_name())) {
      write("Cannot transmogrify the rule.\n");
   } else {
      write("Added a motion to transmogrify the rule.\n");
   }
} /* transmogrify_current_rule_check() */

/** @ignore yes */
void init() {
   ::init();
   add_command("list", "[rules]", (: do_list_rules() :) );
   add_command("list", "rule <number>", (: do_list_one_rule($4[0]) :) );
   add_command("list", "motion <number'motion id'>",
               (: do_list_single_motion($4[0]) :));
   add_command("list", "motions", (: do_list_motions(1) :));
   add_command("list", "motions {verbose|brief}",
               (: do_list_motions($4[0] == "brief") :));
   add_command("comment", "on <number'motion id'>",
                (: do_comment_on($4[0]) :) );
   add_command("amend", "motion <number'motion id'>",
                (: do_amend_motion($4[0]) :) );
   add_command("vote", "{yes|no|abstain} for <number'motion id'>",
               (: do_vote_on($4[1], $4[0]) :));

   add_command("motion", "amend rule <number'rule id'>",
                (: do_amend_current_rule($4[0]) :) );
   add_command("motion", "create new rule <string'type'>",
                (: do_create_motion($4[0]) :) );
   add_command("motion", "remove rule <number'rule id'>",
                (: do_remove_current_rule($4[0]) :) );
   add_command("motion", "move rule <number'rule id'> to <string'type'>",
                (: do_transmogrify_current_rule($4[0], $4[1]) :) );
} /* init() */
