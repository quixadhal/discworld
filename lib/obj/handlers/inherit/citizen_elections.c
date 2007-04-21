/**
 * This is the file used to do elections for magistrate positions.
 * @author Pinkfish
 * @started Mon Oct 23 22:30:19 PDT 2000
 */
inherit "/obj/handlers/inherit/citizens";

#include <player_handler.h>
#include <player.h>
#include <board.h>
#define NOMIC_SYSTEM_NO_CLASSES
#include <nomic_system.h>

#define CITIZEN_ELECTION_PHASE_NONE       0
#define CITIZEN_ELECTION_PHASE_NOMINATION 1
#define CITIZEN_ELECTION_PHASE_ELECTION   2
#define CITIZEN_ELECTION_PHASE_GRACE      3
#define CITIZEN_ELECTION_PHASE_DISABLED 4

#define CITIZEN_ELECTION_TIME   (20 * (7 * 24 * 60 * 60))
#define CITIZEN_ELECTION_PHASE_TIME      (10 * 24 * 60 * 60)
#define CITIZEN_PHASE_TIME      (14 * 24 * 60 * 60)
#define CITIZEN_GRACE_TIME      (14 * 24 * 60 * 60)
#define CITIZEN_ELECTION_DISABLED_TIME (14 * 24 * 60 * 60)

class election_data {
   int last_election;
   int phase;
   int phase_start;
   mapping nominations;
   mapping votes;
   string* voted;
}

protected void finish_election_phase(string area);
int post_citizen_message(string area, string subject, string message);

private mapping _elections;
private mapping _boards;

void create() {
   _elections = ([ ]);
   _boards = ([ ]);
   citizens::create();
   call_out("do_next_event", 10);
} /* create() */

/** @ignore yes */
void create_area(string area) {
   _elections[area] = new(class election_data);
   _elections[area]->phase = CITIZEN_ELECTION_PHASE_GRACE;
   _elections[area]->phase_start = time();
   _elections[area]->nominations = ([ ]);
   _elections[area]->votes = ([ ]);
   _elections[area]->voted = ({ });
   ::create_area(area);
} /* create_area() */

/**
 * This method returns the time of the next election.
 * @return the time of the next election, 0 if on is in progress
 */
int query_next_election_time(string area) {
   if (!_elections[area]) {
      return 0;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_GRACE) {
      return _elections[area]->phase_start + CITIZEN_GRACE_TIME;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NONE) {
      return _elections[area]->last_election + CITIZEN_ELECTION_TIME;
   }
   return 0;
} /* query_next_election_time() */

/**
 * This method returns the end of the phase time.
 * @return the end of the phase time, 0 on failure
 */
int query_end_of_phase_time(string area) {
   if (!_elections[area]) {
      return 0;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NONE) {
      return 0;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_ELECTION) {
      return _elections[area]->phase_start + CITIZEN_ELECTION_PHASE_TIME;
   }

   return _elections[area]->phase_start + CITIZEN_PHASE_TIME;
} /* query_end_of_phase_time() */

/**
 * This method returns the phase of the election.
 * @param area the area to check
 * @return the election phase
 */
protected int query_election_phase(string area) {
   if (!_elections[area]) {
      return CITIZEN_ELECTION_PHASE_NONE;
   }
   return _elections[area]->phase;
} /* query_election_phase() */

/**
 * This method checks to see if we are in the nomination phase.
 * @param area the area to check
 * @return 1 if we are, 0 if not
 */
int is_nomination_phase(string area) {
   return query_election_phase(area) == CITIZEN_ELECTION_PHASE_NOMINATION;
} /* is_nomination_phase() */

/**
 * This method checks to see if we are in the election phase.
 * @param area the area to check
 * @return 1 if we are, 0 if not
 */
int is_election_phase(string area) {
   return query_election_phase(area) == CITIZEN_ELECTION_PHASE_ELECTION;
} /* is_election_phase() */

/**
 * This method checks to see if we are in the grace phase.
 * @param area the area to check
 * @return 1 if we are, 0 if not
 */
int is_grace_phase(string area) {
   return query_election_phase(area) == CITIZEN_ELECTION_PHASE_GRACE;
} /* is_grace_phase() */

/**
 * This method nominates a person for a position.
 * @param area the area to nominate in
 * @param person the being nominated
 * @param nominator the person doing the nomination
 * @return 1 on success, 0 on failure
 */
int nominate_magistrate(string area, string person, string nominator) {
   if (!_elections[area] ||
       !is_citizen_of(area, person) ||
       !is_citizen_of(area, nominator)) {
      return 0;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NOMINATION) {
      if (!_elections[area]->nominations[person]) {
         _elections[area]->nominations[person] = ({ });
      } else if (member_array(nominator,
                              _elections[area]->nominations[person]) != -1) {
         return 0;
      }
      _elections[area]->nominations[person] += ({ nominator });
      save_me();
      return 1;
   }

   return 0;
} /* nominate_magistrate() */

/**
 * This method checks to see who has nominated a specified person.
 * @param area the area to check
 * @param person the person to check
 * @return the array of people who have nominated the person
 */
string* query_nominated_by(string area, string person) {
   if (!_elections[area]) {
      return ({ });
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NOMINATION) {
      if (_elections[area]->nominations[person]) {
         return copy(_elections[area]->nominations[person]);
      }
   }

   return ({ });
} /* query_nominated_by() */

/**
 * This method returns the list of people who have been nominated.
 * @param area the area to get the list of people who are nominated
 * @return the list of people who have been nominated
 */
string* query_people_nominated(string area) {
   if (!_elections[area]) {
      return ({ });
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NOMINATION) {
      return keys(_elections[area]->nominations);
   }

   return ({ });
} /* query_people_nominate() */

/**
 * This method checks to see if the specifiedf person has accepted their
 * nomination or not.
 * @param area the area to check in
 * @param person the person to check to see if they have accepted
 * @return 1 if they are accepted, 0 if not
 */
int is_nomination_accepted(string area, string person) {
   string* nominations;

   nominations = query_nominated_by(area, person);
   if (sizeof(nominations) >= 2 &&
       member_array(person, nominations) != -1) {
      return 1;
   }
   return 0;
} /* is_nomination_accepted() */

/**
 * This method checks to see if the specified person has voted.
 * @param area the area to check in
 * @param person the person the check
 * @return 1 if they have voted, 0 if not
 */
int has_voted(string area, string person) {
   if (!_elections[area]) {
      return 0;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_ELECTION) {
      return member_array(person, _elections[area]->voted) != -1;
   }
   return 0;
} /* has_voted() */

/**
 * This method checks to see if the specified person is an eligible
 * person to vote for.
 * @param area the area to checlk
 * @param person the person to check
 * @return 1 if they are, 0 if not
 */
int is_eligible_to_vote_for(string area, string person) {
   if (!_elections[area]) {
      return 0;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_ELECTION) {
      return member_array(person, keys(_elections[area]->votes)) != -1;
   }

   return 0;
} /* is_eleigble_to_vote_for() */

/**
 * This method votes for a specified person in the area.
 * @param area the area to vote for someone in
 * @param person the person to vote for
 * @param voter the person voting
 * @return 1 on success, 0 on failure
 */
int vote_for_people(string area, string* people, string voter) {
   string person;

   if (!_elections[area] ||
       !is_citizen_of(area, voter)) {
      return 0;
   }

   foreach (person in people) {
      if (!is_citizen_of(area, person)) {
         return 0;
      }
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_ELECTION) {
      if (has_voted(area, voter)) {
         return 0;
      }

      foreach (person in people) {
         if (is_eligible_to_vote_for(area, person)) {
            _elections[area]->votes[person]++;
         }
      }
      _elections[area]->voted += ({ voter });
      save_me();
      return 1;
   }

   return 0;
} /* vote_for_people() */

/**
 * This method retuns the list of people that can be voted for.
 * @param area the area to check in
 * @return the array of people that can be voted for
 */
string* query_can_vote_for(string area) {
   if (!_elections[area]) {
      return ({ });
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_ELECTION) {
      return keys(_elections[area]->votes);
   }

   return ({ });
} /* query_can_vote_for() */

/**
 * This method starts the election.
 * @param area the area to do the election for
 */
protected void start_election(string area) {
   if (!_elections[area]) {
      return ;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NONE ||
       _elections[area]->phase == CITIZEN_ELECTION_PHASE_GRACE) {
      _elections[area]->votes = ([ ]);
      _elections[area]->voted = ({ });
      _elections[area]->nominations = ([ ]);
      _elections[area]->phase_start = time();
      _elections[area]->phase = CITIZEN_ELECTION_PHASE_NOMINATION;
      save_me();
      post_citizen_message(area, "Nomination phase has started.",
                           "Please go to your council offices and "
                           "nominate\nthe magistrates for office.\n");
   }
} /* start_election() */

/**
 * This method finishes the nomination phase.
 * @param area the area to finish the phase for
 */
protected void finish_nomination_phase(string area) {
   string person;
   string* nominators;

   if (!_elections[area]) {
      return ;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_NOMINATION) {
      //
      // Ok, zip through and remove all badies, non-citizens or 
      // people that were not nominated enough.
      //
      _elections[area]->votes = ([ ]);
      _elections[area]->voted = ({ });
      foreach (person, nominators in _elections[area]->nominations) {
         if (is_citizen_of(area, person) &&
             is_nomination_accepted(area, person) &&
             !PLAYER_HANDLER->test_creator(person)) {
            _elections[area]->votes[person] = 0;
         }
      }
      _elections[area]->phase = CITIZEN_ELECTION_PHASE_ELECTION;
      _elections[area]->phase_start = time();
      _elections[area]->nominations = ([ ]);
      save_me();
      post_citizen_message(area, "Nomination phase finished.",
                           "The people nominated for election are:\n" +
                           implode(map(keys(_elections[area]->votes),
                                       (: "   " + $1 :)), "\n"));
      if (sizeof(keys(_elections[area]->votes)) <=
          query_number_of_magistrates(area)) {
          //
          // Turn them all into magistrates.
          //
          finish_election_phase(area);
      }
   }
 
} /* finish_nomination_phase() */

/**
 * This method finishes the election phase.
 * @param area the area to finish the phase for
 */
protected void finish_election_phase(string area) {
   string* voted;
   string* sorted_voted;
   string high_pk;
   string high_non_pk;
   string person;
   string results;
   int needed;
   int num;

   if (!_elections[area]) {
      return ;
   }

   if (_elections[area]->phase == CITIZEN_ELECTION_PHASE_ELECTION) {
      if (sizeof(keys(_elections[area]->votes)) <=
          query_number_of_magistrates(area)) {
         voted = sort_array(keys(_elections[area]->votes),
                            (: $3[$2] - $3[$1] :), _elections[area]->votes);
         sorted_voted = voted;
      } else {
         //
         // Sort the people in order of votes.
         //
         voted = sort_array(keys(_elections[area]->votes),
                            (: $3[$2] - $3[$1] :), _elections[area]->votes);
         sorted_voted = voted;

         //
         // Ok, see if we have one pk and one npk.
         //
         needed = query_number_of_magistrates(area);
         foreach (person in voted) {
            if (PLAYER_HANDLER->test_player_killer(person)) {
               if (!high_pk) {
                  needed--;
                  high_pk = person;
               }
            } else {
               if (!high_non_pk) {
                  needed--;
                  high_non_pk = person;
               }
            }
            if (high_pk && high_non_pk) {
               break;
            }
         }

         voted -= ({ high_pk, high_non_pk });
         voted = voted[0..needed - 1];
         voted += ({ high_pk, high_non_pk });
         voted -= ({ 0 });
      }
      //
      // Post the results.
      //
      results = "Votes:\n";
      foreach (person in sorted_voted) {
         results += sprintf("%20s %d\n", capitalize(person),
                            _elections[area]->votes[person]);
      }
      results += "\nElected officals are " + query_multiple_short(voted) + "\n";
      results += "\nNumber of people who voted " +
                 sizeof(_elections[area]->voted);
      num = query_number_of_citizens(area);
      if (num == 0) {
         results += " (100% of the citizens)\n";
      } else {
         results += " (" + (sizeof(_elections[area]->voted) * 100 / num) +
                    "% of the citizens)\n";
      }
      _elections[area]->votes = ([ ]);
      _elections[area]->voted = ({ });
      _elections[area]->phase = CITIZEN_ELECTION_PHASE_NONE;
      _elections[area]->phase_start = time();
      _elections[area]->last_election = time();
      _elections[area]->nominations = ([ ]);

      post_citizen_message(area, "Election results", results);

      setup_magistrates(area, voted);
      save_me();
   }
} /* finish_election_phase() */

/** @ignore yes */
protected void do_next_event() {
   int next_event;
   int time_to_next_event;
   string area;
   class election_data data;

   foreach (area, data in _elections) {
      switch (data->phase) {
      case CITIZEN_ELECTION_PHASE_NONE :
         next_event = (data->last_election - time()) + CITIZEN_ELECTION_TIME;
         if (next_event <= 0) {
            start_election(area);
         }
         break;
      case CITIZEN_ELECTION_PHASE_NOMINATION :
         next_event = (data->phase_start - time()) + CITIZEN_PHASE_TIME;
         if (next_event <= 0) {
            finish_nomination_phase(area);
         }
         break;
      case CITIZEN_ELECTION_PHASE_ELECTION :
         next_event = (data->phase_start - time()) + CITIZEN_ELECTION_PHASE_TIME;
         if (next_event <= 0) {
            finish_election_phase(area);
         }
         break;
      case CITIZEN_ELECTION_PHASE_GRACE :
         next_event = (data->phase_start - time()) + CITIZEN_GRACE_TIME;
         if (next_event <= 0) {
            start_election(area);
         }
         break;

      case CITIZEN_ELECTION_PHASE_DISABLED : 
         next_event = (data->phase_start - time()) + 
            CITIZEN_ELECTION_DISABLED_TIME;
         if (next_event <= 0) {
             /*  Because this phase is disabled, we're not going to do to
                 trigger any event here at all. */ 
             next_event = next_event;
         }
         break;
      }

      if (next_event > 0 &&
          (next_event < time_to_next_event || !time_to_next_event)) {
         time_to_next_event = next_event;
      }
   }
   if(time_to_next_event) {
     call_out("do_next_event", time_to_next_event);
   }
} /* do_next_event() */

/**
 * This method sets the board for the specified area.
 * @param area the area to set the board for
 * @param name the name of the board
 */
void set_citizen_board(string area, string name) {
   if (!pointerp(_boards[area])) {
      _boards[area] = ({ 0, 0 });
   }
   _boards[area][0] = name;
   save_me();
} /* set_citizen_board() */

/**
 * This method returns the board for the specified area.
 * @param area the area to look for the board on
 * @return the board name for the area, 0 for none
 */
string query_magistrate_board(string area) {
   if (pointerp(_boards[area])) {
      return _boards[area][1];
   }
   return 0;
} /* query_magistrate_board() */

/**
 * This method sets the board for the specified area.
 * @param area the area to set the board for
 * @param name the name of the board
 */
void set_magistrate_board(string area, string name) {
   if (!pointerp(_boards[area])) {
      _boards[area] = ({ 0, 0 });
   }
   _boards[area][1] = name;
   save_me();
} /* set_magistrate_board() */

/**
 * This method returns the board for the specified area.
 * @param area the area to look for the board on
 * @return the board name for the area, 0 for none
 */
string query_citizen_board(string area) {
   if (pointerp(_boards[area])) {
      return _boards[area][0];
   }
   return 0;
} /* query_citizen_board() */

/**
 * This method will post a message to the citizen board from the
 * administration.
 * @param area the area to post the message to 
 * @param subject the subject it has
 * @param message the message to post
 */
int post_citizen_message(string area, string subject, string message) {
   string board;

   board = query_citizen_board(area);
   if (board) {
      return BOARD_HAND->add_message(board,
                           NOMIC_SYSTEM_ADMINISTRATOR_NAME,
                           subject,
                           message);
   }
   return 0;
} /* post_citizen_message() */

/**
 * This method will post a message to the magistrate board from the
 * administration.
 * @param area the area to post the message to 
 * @param subject the subject it has
 * @param message the message to post
 */
int post_magistrate_message(string area, string subject, string message) {
   string board;

   board = query_magistrate_board(area);
   if (board) {
      return BOARD_HAND->add_message(board,
                           NOMIC_SYSTEM_ADMINISTRATOR_NAME,
                           subject,
                           message);
   }
   return 0;
} /* post_magistrate_message() */

/**
 * This method sets the specifed election to "disabled", meaning it will
 * sit indefinitely with no election.
 */
int disable_citizen_election(string area) {
    if (!_elections[area]) {
      return 0;
    }

   _elections[area]->phase = CITIZEN_ELECTION_PHASE_DISABLED;
   _elections[area]->phase_start = time();
   _elections[area]->nominations = ([ ]);
   _elections[area]->votes = ([ ]);
   _elections[area]->voted = ({ });
   save_me();
   return 1;
} /* disable_citizen_election() */ 

