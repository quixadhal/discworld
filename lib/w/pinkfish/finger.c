#include "path.h"
#include "wizard.h"
inherit BASIC;

/*
 * This spell creates a nice bunch of flowers.
 */

void setup() {
  set_power_level(3);
  set_name("finger");
  set_ritual(({
   ({
      ({
        "You throw out your arms.\n",
        "$tp_name$ throws out his arms.\n",
      }),
      ({
        "You gesticulate wildly.\n",
        "$tp_name$ gesticulates wildly.\n",
      }),
    }),
    ({
      ({
        "You point off into the sky.\n",
        "$tp_name$ points off into the sky.\n",
      }),
    }),
    ({
      ({
        "You exclaim: Irble de flub womble!\n",
        "$tp_name$ exclaims: Irble de flub womble!\n",
      }),
    }),
  }));
  set_failure(10);
  set_skill_threshold(10);
  set_fixed_time(1);
  set_teach_lvl(4)
  set_learn_lvl(1);
  set_skill_used("magic.spells.misc.target");
  set_info(
"Allows you to get information about other players from other muds, "+
"Useful huh?\n");
  set_consumables(({ }));
  set_needed(({ }));
  set_spell_type("silly");
  set_point_cost(10);
  set_duration(1); /* Please note this is the duration in the player's
                    * spell effects array */
  set_directed(0);
  set_multiple(0);
  set_casting_time(120);
} /* setup() */

/* Returns the xp gained from casting the spell. */
int effect_heart_beat() {
  object demon;

  demon = clone_object(CHARS+"finger_demon");
  demon->set_caster(this_player());
  tell_room(environment(this_player()),
"A demon wanders casuly into the room.\n");
  demon->init_command("whisper "+this_player()->query_name()+
                      " Just say the name of the person you "+
                      "wish me to finger, something like pinkfish@discworld\n");
  write("A demon wanders casualy into the room.\n");
  return 10;
} /* effect_heart_beat() */
 
void do_spell_failure() {
  write("We failed...\n");
} /* do_spell_failure() */
