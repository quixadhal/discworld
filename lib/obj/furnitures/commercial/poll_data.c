// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/inherit/voting_room";
inherit "/std/room/furniture/commercial";

#include <money.h>

#define BOARD_TAG "poll data"
#define POLL_DATA_MAXIMUM_RESULTS 10

private int _allow_open_voting;
private string* _finished_votes;

void finish_vote(string type, string description, string* choices,
                 mapping votes, string* voted);


void create() {
   voting_room::create();
   commercial::create();
   _finished_votes = ({ });
} /* create() */

void setup() {
   set_name( "poll" );
   set_short( "opinion poll" );
   add_adjective( ({ "opinion" } ));
   set_long( "A box with a bunch of pieces paper sitting on it.  It seems "
             "to be keeping a tally of something.\n");
   set_allowed_positions(({"sitting", "lying", "kneeling", "meditating"}));
   set_allowed_room_verbs((["sitting" : "sits" ]));
   set_weight(2000);
   set_value(240000);
   set_commercial_size(2);
   set_commercial_type("misc");
   set_shop_use_types(({ "poll add", "poll vote" }));

   set_completion_action((: finish_vote :));
   set_no_elections(1);
   set_vote_conditions((: is_open_for("poll vote", $1->query_name()) :));

   add_help_file("poll_data");
}

/** @ignore yes */
int load_me() {
   if (!query_commercial_id()) {
      setup_new_commercial_id();
   }
   set_save_file(query_save_dir() + query_commercial_id());
   if (!_finished_votes) {
      _finished_votes = ({ });
   }
} /* do_load() */

int do_show_results() {
   string ret;
   string result;

   ret = "";
   foreach (result in _finished_votes) {
      ret += result + "\n\n";
   }
   write("$P$Vote Archive$P$" + ret);
   add_succeeded_mess("");
   return 1;
}

int do_create_vote(string choices) {
   if (!is_open_for("poll add", this_player()->query_name())) {
      add_failed_mess("Unable to add a vote.\n");
      return 0;
   }

  return ::do_create_vote(choices);
} /* do_create_vote() */

void finish_vote(string type, string description, string* choices,
                 int* votes, string* voted)
{
   string ret;
   int i;

   ret = "Vote finished at " + ctime(time()) + ":\n";
   ret += "$I$3=   " + description + "\n\n";
   ret += "$I$0=Results:\n$I$3=   ";
   for (i = 0; i < sizeof(choices); i++) {
      ret += choices[i] + ": " + votes[i] + "\n";
   }
   if (!_finished_votes) {
      _finished_votes = ({ ret });
   } else {
      _finished_votes += ({ ret });
   }
   if (sizeof(_finished_votes) > POLL_DATA_MAXIMUM_RESULTS) {
      _finished_votes = _finished_votes[0..POLL_DATA_MAXIMUM_RESULTS];
   }
   save_room();
}

int do_set_duration(int hours) {
   if (hours <= 0) {
      add_failed_mess("The duration must be positive.\n");
      return 0;
   }

   set_vote_duration(hours * 60 * 60);
   add_succeeded_mess("Set the vote duration to " + hours + " hours.\n");
   return 1;
} /* do_set_duration() */

int do_delete_vote(int id) {
   if (!::delete_vote(id)) {
      add_failed_mess("Cannot delete the vote id #" + id + ".\n");
      return 0;
   }

   add_succeeded_mess("Deleted the vote id#" + id + ".\n");
   return 1;
} /* do_delete_vote() */

int do_end_vote(int id) {
   if ( !query_our_votes()[ id ] ) {
      add_failed_mess("Cannot end the vote id #" + id + ".\n");
      return 0;
   }

   end_vote(id);
   add_succeeded_mess("Finished the vote id#" + id + ".\n");
   return 1;
} /* do_delete_vote() */

string query_main_status() {
   string ret;

   ret = "Vote duration:    " + (query_vote_duration() / (60 * 60)) +
         " hours\n";
   return ret;
} /* query_main_status() */

void init() {
   commercial::init();
   voting_room::init();
   voting_room::init_add_vote();
   add_command("results", "of votes", (: do_show_results() :));
   if (environment()->is_allowed(this_player()->query_name())) {
      add_command("set", "vote duration <number'hours'> on <direct:object>",
                  (: do_set_duration($4[0]) :));
      add_command("delete", "vote <number'vote id'>",
                  (: do_delete_vote($4[0]) :));
      add_command("finish", "vote <number'vote id'>",
                  (: do_end_vote($4[0]) :));
   }
} /* init() */
