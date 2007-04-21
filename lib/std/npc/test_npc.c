#include <npc/goals.h>
#include <npc/plan.h>

inherit "/std/npc/npc_intelligent";

void setup() {
   string goal;

   set_name("fluff");
   set_short("bit of fluff");
   set_long("A small bit of fluff.");
   set_race("human");
   set_guild("warrior");
   set_level(10);
   goal = GOALS_DIR + "basic/stay_alive_basic";
   goal->add_goal_to_npc(this_object(), ([ ]), 100);
   load_chat(100, ({ 1, "'Scruff!",
                     1, "'Chickens?",
			}));
} /* setup() */
