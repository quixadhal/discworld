/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wea_r.c,v 1.10 2003/03/25 19:46:47 presto Exp $
 * $Log: wea_r.c,v $
 * Revision 1.10  2003/03/25 19:46:47  presto
 * Forcibly released due to inactivity
 *
 * Revision 1.9  2002/07/10 16:55:23  trilogy
 * Put a taskmaster thingie in for wearing clothes covertly.
 *
 * Revision 1.8  2002/02/03 19:22:25  tannah
 * Changed a "they" to a TP->query_pronoun() so that it agrees with the
 * subject
 *
 * Revision 1.7  2001/11/07 21:54:06  pinkfish
 * Make it so wear automatic removes and wears stuff if you want to wear
 * stuff lower down.
 *
 * Revision 1.6  2001/03/08 04:21:16  presto
 * Fixed so you can't wear things you aren't carrying in your immediate inventory.
 *
 * Revision 1.5  1999/12/08 04:08:54  ceres
 * Modified to use standard can_wear_or_remove() function in /obj/handlers/clothing_handler
 *
 * Revision 1.4  1999/11/15 10:37:17  pinkfish
 * Make some small changes to the wear code.
 *
 * Revision 1.3  1998/08/25 10:26:02  pinkfish
 * Change them to handle ordering the object to remove/wear so that under
 * can be safely handled.
 *
 * Revision 1.2  1998/03/27 20:49:16  pinkfish
 * To the new clothing handler.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 *
*/
#include <clothing.h>
#include <tasks.h>

inherit "/cmds/base";

#define TP this_player()
#define SKILL "covert.manipulation.sleight-of-hand"

mapping add_to_failed(mapping failed, string word, object thing);

int cmd(object *things) {
  int i;
  string result;
  string stuff;
  string *reasons;
  object *okay;
  object *blocking;
  object *total_blocking;
  object ob;
  object tmp;
  mapping failed;
  mapping hide_invis;
  int hiding, sneaking, difficulty, light, my_light;

  okay = ({ });
  failed = ([ ]);
  things = sort_array(things, (: CLOTHING_HANDLER->query_clothing_depth($2->query_type()) -
                                 CLOTHING_HANDLER->query_clothing_depth($1->query_type()) :) );
  total_blocking = ({ });
  foreach (ob in things) {
    if (environment(ob) != this_player())  {
      failed = add_to_failed(failed, "not carrying", ob);
      continue;
    }
    if ( !ob->query_wearable() ) {
      failed = add_to_failed( failed, "cannot wear", ob);
      continue;
    }
    if (member_array(ob, TP->query_wearing()) != -1) {
      failed = add_to_failed(failed, "already wearing", ob);
      continue;
    }
    if (ob->cannot_be_worn()) {
      failed = add_to_failed(failed, "cannot wear", ob);
      continue;
    }

    blocking = CLOTHING_HANDLER->query_items_blocking(ob, this_player());
if (this_player() == find_player("presto"))
printf("blocking == %O\n", blocking);
    if (sizeof(blocking)) {
       // Try and remove them all.
       total_blocking += blocking;
       foreach (tmp in blocking) {
          TP->remove_armour(tmp);
       }
    }
    result = CLOTHING_HANDLER->can_wear_or_remove(ob, this_player());
if (this_player() == find_player("presto"))
printf("ob == %s, result == %s\n", ob->short(), result);
    if (result) {
      failed = add_to_failed(failed, result, ob);
      continue;
    }
    result = TP->wear_armour(ob, 0);
    if (result) {
      failed = add_to_failed(failed, result, ob);
      continue;
    }
    okay += ({ ob });
  }
  if (total_blocking) {
    foreach (tmp in total_blocking) {
      TP->wear_armour(tmp);
    }
  }
  result = "";
  if (sizeof(failed)) {
    reasons = keys(failed);
    for (i = 0; i < sizeof(reasons); i++) {
      stuff = query_multiple_short(failed[reasons[i]], "the");
      switch (reasons[i]) {
      case "cannot wear" :
        if (!sizeof(okay)) {
           result += capitalize(stuff) +" cannot be worn.\n";
        }
        break;
      case "already wearing" :
        result += "You are already wearing "+ stuff +".\n";
        break;
      case "already holding" :
        result += "You cannot wear "+ stuff +" as you do not "
          "have enough free limbs.\n";
        break;
      case "oh dear" :
        result += "I'm not sure why you can't wear "+ stuff +
          "; something has probably gone wrong, so please "
          "contact a creator.\n";
        break;
      case "not carrying":
        result += "You aren't carrying " + stuff + ".\n";
        break;
      default :
        result += "You cannot put on "+ stuff +" "+ reasons[i] +".\n";
      }
    }
  }
  if (!sizeof(okay)) {
    return notify_fail(result);
  }
  stuff = query_multiple_short(okay);
  if (sizeof(total_blocking)) {
     tell_room(environment(TP), TP->one_short() + " removes " +
               query_multiple_short(total_blocking) + " so " +
               TP->query_pronoun() + " can wear "+
               stuff +".\n" + TP->one_short() + " wears " +
               query_multiple_short(total_blocking) + ".\n",
               TP);
     write("You remove " +
               query_multiple_short(total_blocking) + " so you can wear "+
               stuff +".\nYou wear " +
               query_multiple_short(total_blocking) + ".\n",
               TP);
  } else {
    hide_invis = ( mapping )this_player()->query_hide_invis();
    hiding = hide_invis[ "hiding" ] ? 1 : 0;
    sneaking = this_player()->query_sneak_level() ? 1 : 0;
  
    if( hiding || sneaking ) {
      my_light = this_player()->query_light();
      light = environment( this_player() )->query_light();
    
      difficulty = light + ( 4 * my_light ) / ( light + 1 );
  
      if( sizeof( total_blocking ) ) {
        difficulty += 50 * sizeof( total_blocking );
      }
  
      difficulty += okay[0]->query_complete_weight();

      debug_printf( "Difficulty = %d.\n Skill = %s\n Bonus = %d\n",
                    difficulty, SKILL, this_player()->
                    query_skill_bonus( SKILL ) );
      switch( TASKER->perform_task( this_player(), SKILL, difficulty,
        TM_FREE ) ) {
        case AWARD :
          write( "%^YELLOW%^" + ({
            "You discover something that lets your fingers move more "
              "nimbly.",
            "You find yourself capable of deceiving the eye with greater "
              "ease than before.",
            "You realise how to deceive the eye more effectively."
          })[ random(3) ] + "%^RESET%^\n" );
        case SUCCEED :
          add_succeeded_mess( ({ "$N $V " + stuff + ", managing to stay "
            "unnoticed.\n",
            "" }) );
          break;
        default :
          this_player()->add_succeeded_mess( this_object(), "$N "
            "unsuccessfully tr$y to " + query_verb() + " " + stuff +
            " while staying unnoticed.\n", ({ }) );
          break;
      }
    } else {
      this_player()->add_succeeded_mess( this_object(), "$N $V " + stuff +
        ".\n", ({ }) );
    }
  }
  return 1;
} /* cmd() */

mapping add_to_failed(mapping failed, string word, object thing) {
  if (!failed[word]) {
    failed += ([ word : ({ thing }) ]);
  } else {
    failed[word] += ({ thing });
  }
  return failed;
} /* add_to_failed() */

mixed *query_patterns() {
  return ({ "<indirect:object:me>", (: cmd($1) :) });
}
