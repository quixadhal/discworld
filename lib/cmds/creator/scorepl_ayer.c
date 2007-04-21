inherit "/cmds/base";

#define SCORE_CMD "/cmds/player/score"

int score_quests(object *obs) {
   object play;

   foreach(play in obs) {
      write("Quests score for " + play->the_short() + ":\n");
      SCORE_CMD->score_quests(play);
   }
   return 1;
} /* score_quests() */

int score_stats(object *obs) {
   object play;
 
   foreach(play in obs) {
      write("Stats score for " + play->the_short() + ":\n");
      SCORE_CMD->score_stats(play, "verbose");
   }
   return 1;
} /* score_stats() */

int score_normal(object *obs) {
   object play;
 
   foreach(play in obs) {
      write("Score for " + play->the_short() + ":\n");
      SCORE_CMD->score_normal(play, "verbose");
   }
   return 1;
} /* score_normal() */

mixed *query_patterns() {
   return ({ "quests <indirect:player>", (: score_quests($1) :), 
             "stats <indirect:player>", (: score_stats($1) :),
             "<indirect:any-living>", (: score_normal($1) :) });
} /* query_patterns() */
