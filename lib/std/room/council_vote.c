/**
 * This is the room you will use to vote for magistrates.
 * @author Pinkfish
 * @started Mon Oct 23 22:26:11 PDT 2000
 */
inherit "/std/room/basic_room";
inherit "/std/room/inherit/council_base";
inherit "/std/room/inherit/voting_room";
#include <nomic_system.h>
#include <player_handler.h>

void post_results(string type, string desc, string* choices,
                  mixed* votes, string* voted);

void create() {
   voting_room::create();

   set_vote_conditions( (: NOMIC_HANDLER->is_citizen_of(query_council_area(),
                            $1->query_name()) :));
   set_completion_action( (: post_results :));
   set_vote_duration( 7 * 24 * 60 * 60);
   //set_open_voting(1);
   basic_room::create();
   add_help_file("voting_booth");
   setup_after_load();
} /* create() */

/**
 * This method prints some useful information about the election status
 * of the current area.
 * @return 1 on success, 0 on failure
 */
int do_status() {
   string person;
   string* nominated;
   string* magistrates;
   string ret;

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                     this_player()->query_name())) {
      add_failed_mess("You must be a citizen to see this information.\n");
      return 0;
   }

   magistrates = NOMIC_HANDLER->query_magistrates(query_council_area());
   if (!sizeof(magistrates)) {
      ret = "There are no current magistrates.\n\n";
   } else {
      ret = "The current magistrates of " + query_council_area() +
            " are " +
            query_multiple_short(map(magistrates, (: capitalize($1) :))) +
            ".\n\n";
   }

   if (NOMIC_HANDLER->is_nomination_phase(query_council_area())) {
      ret += "The citizens of " + query_council_area() + " are currently "
            "accepting nominations for the " +
            query_num(NOMIC_HANDLER->query_number_of_magistrates(query_council_area())) +
            " magistrate positions.  The "
            "currently nominated people are:\n";
      nominated = sort_array(NOMIC_HANDLER->query_people_nominated(query_council_area()), 0);
      if (!sizeof(nominated)) {
         ret += "No one is currently nominated.\n";
      } else {
         foreach (person in nominated) {
            ret += "$I$6=   " + capitalize(person) + " nominated by " +
                   query_multiple_short(map(NOMIC_HANDLER->query_nominated_by(
                                          query_council_area(), person),
                                          (: capitalize($1) :)));
            if (NOMIC_HANDLER->is_nomination_accepted(query_council_area(),
                                                      person)) {
               ret += " (accepted)\n";
            } else {
               ret += "\n";
            }
         }
      }
      ret += "\n\n$I$0=The phase ends at " +
         ctime(NOMIC_HANDLER->query_end_of_phase_time(query_council_area())) +
             "\n";
   } else if (NOMIC_HANDLER->is_election_phase(query_council_area())) {
      ret += "The citizens of " + query_council_area() + " are currently "
            "voting for people in the " +
            query_num(NOMIC_HANDLER->query_number_of_magistrates(query_council_area())) +
            " magistrate positions.  The people "
            "you can vote for are:\n";

      nominated = sort_array(NOMIC_HANDLER->query_can_vote_for(query_council_area()), 0);
      foreach (person in nominated) {
         ret += "   * " + capitalize(person) + "\n";
      }
      ret += "\n\n$I$0=The phase ends at " +
         ctime(NOMIC_HANDLER->query_end_of_phase_time(query_council_area())) +
             "\n";
      if (NOMIC_HANDLER->has_voted(query_council_area(),
                                this_player()->query_name())) {
         ret += "You have already voted.\n";
      } else {
         ret += "You have not yet voted.\n";
      }
   } else {
      ret += "\n\nThe next election is at " +
         ctime(NOMIC_HANDLER->query_next_election_time(query_council_area())) +
             "\n";
   }
   this_player()->more_string(ret, "Status");
   return 1;
} /* do_status() */

/**
 * This method allows you to vote for someone.
 */
int do_vote(string person, int force) {
   string* bits;

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                     this_player()->query_name())) {
      add_failed_mess("You must be a citizen to vote.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_election_phase(query_council_area())) {
      add_failed_mess("There is no election at the moment.\n");
      return 0;
   }

   if (NOMIC_HANDLER->has_voted(query_council_area(),
                                this_player()->query_name())) {
      add_failed_mess("You have already voted.  You cannot vote twice!\n");
      return 0;
   }

   bits = explode(replace_string(lower_case(person), " ", ""), ",");
   bits = uniq_array(bits);
   foreach (person in bits) {
      if (!NOMIC_HANDLER->is_eligible_to_vote_for(query_council_area(),
                                                  person)) {
         add_failed_mess("You cannot vote for " + person + " as they are not "
                         "running or not a citizen.\n");
         return 0;
      }
   }

   if (!force &&
       sizeof(bits) <
            NOMIC_HANDLER->query_number_of_magistrates(query_council_area())) {
      add_failed_mess("You must vote for at least " +
            query_num(NOMIC_HANDLER->query_number_of_magistrates(query_council_area())) +
            " magistrates or use the 'force' option to vote for less.\n");
      return 0;
   }

   if (sizeof(bits) >
            NOMIC_HANDLER->query_number_of_magistrates(query_council_area())) {
      add_failed_mess("You can only vote for " +
            query_num(NOMIC_HANDLER->query_number_of_magistrates(query_council_area())) +
            " magistrates, not more.\n");
      return 0;
   }

   if (NOMIC_HANDLER->vote_for_people(query_council_area(), bits,
                                      this_player()->query_name())) {
      add_succeeded_mess(({ "You successfully vote for " +
                            query_multiple_short(bits) + ".\n",
                            "$N votes for someone.\n" }));
      return 1;
   }

   add_failed_mess("Unable to vote for some reason.\n");
   return 0;
} /* do_vote() */

/**
 * This method allows you to nominate someone for a position.
 */
int do_nominate(string person) {
   person = lower_case(person);
   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(),
                                     this_player()->query_name())) {
      add_failed_mess("You must be a citizen to nominate someonej.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_citizen_of(query_council_area(), person)) {
      add_failed_mess("You can only nominate someone who is a citizen.\n");
      return 0;
   }

   if (!NOMIC_HANDLER->is_nomination_phase(query_council_area())) {
      add_failed_mess("There is no election at the moment.\n");
      return 0;
   }

   if (PLAYER_HANDLER->test_creator(person)) {
      add_failed_mess("You cannot nominate a creator.\n");
      return 0;
   }

   if (member_array(this_player()->query_name(),
                    NOMIC_HANDLER->query_nominated_by(query_council_area(),
                                                      person)) != -1) {
      add_failed_mess("You have already nominated " + person + ".\n");
      return 0;
   }

   if (NOMIC_HANDLER->nominate_magistrate(query_council_area(), person,
                                          this_player()->query_name())) {
      add_succeeded_mess(({ "You successfully nominated " +
                            upper_case( person ) + ".\n",
                            "$N nominates " + person + ".\n" }));
      return 1;
   }

   add_failed_mess("Unable to nominate for some reason.\n");
   return 0;
} /* do_nominate() */

/**
 * This lists all the new rules up for discussion or whatever by the
 * magistrates.
 * @return 1 on success, 0 on failure
 */
int do_list_motions() {
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
      ret += NOMIC_HANDLER->motion_as_string(0, query_council_area(), rule);
      ret += "\n";
   }
   write("$P$New Nomic Rules$P$" + ret);
   return 1;
} /* do_list_motions() */

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

/** @ignore yes */
void post_results(string type, string desc, string* choices,
                  mixed* votes, string* voted) {
   string mess;
   int i;

   mess = "The results of the poll on:\n" + desc + "\n.\n";
   for (i = 0; i < sizeof(choices); i++) {
      if (arrayp(votes[i])) {
         mess += choices[i] + ":  " + sizeof(votes[i]) + "\n";
      } else {
         mess += choices[i] + ":  " + votes[i] + "\n";
      }
   }

   mess += "\n\nPlease note: This is not a new law, just an opinion poll.\n";

   NOMIC_HANDLER->post_citizen_message(query_council_area(), "Results of Poll",
                                          mess);
} /* post_results() */

/** @ignore yes */
void add_vote(string type, string description, string *choices, int ending) {
   string mess;

   ::add_vote(type, description, choices, ending);

   mess = "New vote on:\n" + description + "\nWith choices of " +
          query_multiple_short(choices) + ".\n",
   NOMIC_HANDLER->post_citizen_message(query_council_area(), "New vote by " +
                    this_player()->query_cap_name(),
                                          mess);
} /* add_vote() */


/** ignore yes */
void init() {
   basic_room::init();
   voting_room::init();
   add_command("list", "elections", (: do_status() :));
   add_command("list", "motions", (: do_list_motions() :));
   add_command("information", "", (: do_status() :));
   add_command("status", "", (: do_status() :));
   add_command("vote", "{yes|no|abstain} in [motion] <number'id'>",
               (: do_vote_on($4[1], $4[0]) :));
   add_command("vote", "for <string'people'>", (: do_vote($4[0], 0) :));
   add_command("vote", "force for <string'people'>", (: do_vote($4[0], 1) :));
   add_command("nominate", "<string'person'>", (: do_nominate($4[0]) :));
   if (NOMIC_HANDLER->is_magistrate_of(query_council_area(),
                                      this_player()->query_name())) {
      init_add_vote();
   }
} /* init() */
