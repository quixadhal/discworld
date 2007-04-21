/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: basilisk_stare.c,v 1.3 2000/06/07 21:28:35 pinkfish Exp $
 * $Log: basilisk_stare.c,v $
 * Revision 1.3  2000/06/07 21:28:35  pinkfish
 * FIx it up to use correct find_match code.
 *
 * Revision 1.2  2000/06/07 21:27:17  sin
 * Stuff.
 *
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
#include <effect.h>
#include "path.h"

void do_damage(object to, int amount, object from, int id)
{
  if (!to) return;
  if (to->query_hp() <= amount)
    to->delete_effect(to->sid_to_enum(id));
  to->attack_by(this_object());
  to->adjust_hp(-amount, from);
}

void stop_fighting(object who, mixed arg, int id)
{
  int *fights, i;

  /* I should really be calling test_remove() here, but none of the
   * fighting effects have a test_remove() defined, so I'm kinda 
   * screwed.  -- Sin */
  fights = who->effects_matching("fighting");
  if (sizeof(fights))
    for (i = sizeof(fights) - 1; i >= 0; i--)
      who->delete_effect(fights[i]);
}

void doit(object them, int arg, int id)
{
  int dam;
  object basilisk;

  arg--;
  them->set_arg_of(them->sid_to_enum(id), arg);
  if (!arg) {
    them->delete_effect(them->sid_to_enum(id));
    return;
  }
  if (sizeof(match_objects_for_existence("basilisk", environment(them))) == 0) {
    tell_object(them, "You suddenly notice that the basilisk is no "
                "longer here.\n");
    them->delete_effect(them->sid_to_enum(id));
    return;
  }
  tell_object(them,
         ({
      "You cannot stand the deadly stare.\n",
      "Your soul is being drilled through and out the other side.\n",
      "Your eyeballs are being ripped apart.\n",
      "Your brain is being flayed.\n"
      })[random(4)]);
  tell_room(environment(them),
       ({
         them->one_short()+" cannot bear the deadly gaze.\n",
         them->one_short()+"'s soul is being torn apart.\n",
         them->one_short()+"'s brain is afire.\n",
         them->one_short()+"'s eyeballs are being drilled through.\n"
         })[random(4)], them);
  
  basilisk = find_living("basilisk");
  if (basilisk) {
    dam = random(50)+50;
    call_out("do_damage", 1, them, dam, basilisk, id);
  }
} /* mixed_effect_heart_beat() */

int beginning(object them, int arg, int id)
{
  tell_object(them, "You feel a pair of mental diamond drills penetrating "
         "your brain.\n");
  tell_room(environment(them), them->one_short() +
            "'s soul is about to be ripped apart.\n", them) ;
  them->submit_ee("stop_fighting", 1, EE_ONCE);
  them->submit_ee("doit", 10, EE_CONTINUOUS);
  them->add_property("passed out", 1);
  return arg;
} /* beginning() */

void end(object them)
{
  tell_object(them, "You are freed from the basilisk's deadly stare.\n");
  them->remove_property("passed out");
}

string query_shadow_ob() {
  return SHADOWS+"basilisk_stare";
} /* query_shadow_ob() */

void merge_effect(object them, int arg1, int arg2, int id)
{
  them->set_arg_of(them->sid_to_enum(id), arg1 + arg2);
} /* merge_effect() */

string query_classification() { return "magic.offensive.paralysis.stare"; }

string query_name()
{
  return "a horrible sight";
}
