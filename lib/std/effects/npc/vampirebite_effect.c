/* Blood Loss Effect for use with vampire bites.                       *
 * based on the drunk effect by ceres.                                 *
 *                                                                     *
 * Archana 4/20/97                                                     *
 *                                                                     *
 * Updated to cause vampirism on death, various other modifications as *
 * well to make is simpler and add more descriptions.                  *
 *                                                                     *
 * Arguements are: int duration                                        *
 * See also "vampirebite_shadow.c" and "vampirecorpse_effect.c"        *
 *                                                                     *
 * Lemming, 03/07/2000                                                 */

#include <effect.h>
#include <living.h>
#include <move_failures.h>
#include <player.h>

mapping compass = ([ "north" : 1, "northeast" : 1, "east" : 1, "southeast" : 1,
                     "south" : 1, "southwest" : 1, "west" : 1, "northwest" : 1
                    ]);

void come_around( object person );

/** @ignore yes */
string query_classification() { return "body.bite.vampire"; }

/** @ignore yes */
string query_shadow_ob() { return "/std/shadows/npc/vampirebite_shadow"; }

/** @ignore yes */
int survive_death() { return 1; }

/** @ignore yes */
int beginning( object person, int extent, int id ) {
   int con, time;
 
   tell_object( person, "Pain shoots through your veins, exploding inside "
      "your head, making you scream in agony as the effect of the vampire's "
      "bite fills your body.\n" );
   person->add_extra_look( this_object() );
   person->adjust_bonus_int(-2);
   person->adjust_bonus_wis(-2);
   person->adjust_bonus_str(-2);
   person->adjust_bonus_dex(-2);

   person->submit_ee( "bite_pain", ({ 10, 60 }), EE_CONTINUOUS );

   // The duration should be similar to the old /global/player drunk
   // effect, ie, time = 2*extent/query_con().

   con = person->query_con();
   if( con < 1 ) con = 1;

   time = 2 * extent / con;
   person->submit_ee( 0, time, EE_REMOVE );

   return time;
} /* beginning() */

/** @ignore yes */
int merge_effect( object person, int old_time, int new_extent, int id ) {
   int con, new_time;

   con = person->query_con();
   if( con < 1 ) con = 1;

   new_time = ( 2 * new_extent / con ) + person->expected_tt();
   person->submit_ee( 0, new_time, EE_REMOVE );

   return new_time;
} /* merge_effect() */

/** @ignore yes */
void restart( object person, int time, int id ) {
   person->adjust_bonus_int(-2);
   person->adjust_bonus_wis(-2);
   person->adjust_bonus_str(-2);
   person->adjust_bonus_dex(-2);
} /* restart() */

/** @ignore yes */
void end( object person, int time, int id ) {
   if( person->query_property( PASSED_OUT_PROP ) )
      come_around( person );

   person->adjust_bonus_int(2);
   person->adjust_bonus_wis(2);
   person->adjust_bonus_str(2);
   person->adjust_bonus_dex(2);

   tell_object( person, "The firey pain screaming through your veins and "
      "incessant pounding inside your head finally wear off, leaving you "
      "with a mild headache.\n" );
} /* end() */

/** @ignore yes */
void bite_pain( object person, int time, int id ) {
   int timeleft, selector;

   timeleft = (int)person->expected_tt();
  
   if( person->query_property( PASSED_OUT_PROP ) ) {
      switch( random(3) ) {
      case 0 :
         tell_room( environment(person), person->one_short() + " shakes "
            "slightly in silent agony.\n", person );
         break;
      case 1 :
         tell_room( environment(person), person->one_short() + " moans, "
            "writhing on the ground.\n", person );
         break;
      case 2 :
         tell_room( environment(person), person->one_short() + " opens " +
            person->query_possessive() + " eyes, stares around wildly as if "
            "in a fever, and passes out again with a groan.\n", person );
         break;
      }
      return;
   }

   selector = random( timeleft ) / person->query_con() / 5;

   switch( selector ) {
   case 0 :
      break;
   case 1 :
   case 2 :
      switch( random(4) ) {
      case 0 :
         person->new_parser( "gasp in pain" );
         break;
      case 1 :
         person->new_parser( "moan agonisingly" );
         break;
      case 2 :
         person->new_parser( "stumble" );
         break;
      default :
         person->new_parser( "dloh head in pain" );
         break;
      }
      break;
   case 3 :
   case 4 :
   case 5 :
      switch( random(4) ) {
      case 0 :
         tell_object( person, "You clutch your head and groan in agony as "
            "the effect of the vampires bite burns through you.\n" );
         tell_room( environment( person ), person->the_short() +
            " clutches " + person->query_possessive() + " head and groans "
            "in agony.\n", person );
         break;
      case 1 :
         tell_object( person, "You stagger about, your vision blurring and "
            "fading for a moment while your head throbs with pain.\n" );
         tell_room( environment( person ), person->the_short() +
            " staggers about, " + person->query_possessive() + " eyes "
            "drifting in and out of focus.\n", person );
         break;
      case 2 :
         tell_object( person, "You feel yourself go lightheaded and the "
            "pain fades for just a moment, before slamming back into you "
            "like a hammer to the forehead.\n" );
         tell_room( environment( person ), person->the_short() + " pales "
            "as the blood drains from " + person->query_possessive() +
            " face, then clenches " + person->query_possessive() + " teeth "
            "and groans with pain.\n", person );
         break;
      default :
         tell_object( person, "The fire flooding through your veins causes "
            "you to scream in agony, your body racked with pain.\n" );
         tell_room( environment( person ), person->the_short() + " leans "
            + person->query_possessive() + " head back and screams in "
            "pain.\n", person );
         break;
      }
      break;
   default :
      if( timeleft > 100 ) {
         switch( random(2) ) {
         case 0 :
            tell_object( person, "The world goes black.  You have "+
                "passed out.\n" );
            tell_room( environment( person ), "The blood drains from " +
                person->the_short() + "'s face, and " +
                person->query_pronoun() + " collapses heavily to the "
                "ground.\n", person );
            person->add_property( PASSED_OUT_PROP, 1 );
            person->submit_ee( "come_around", ({ 50, 80 }), EE_ONCE );
            break;
         case 1 :
            person->submit_ee( "wander_about", 3, EE_ONCE );
            break;
         }
      } 
   }
} /* bite_pain() */

/** @ignore yes */
void come_around( object person ) {
   tell_object( person, "You feel a pounding headache coming on and realise "
      "you are finally conscious.\n" );
   tell_room( environment( person ), person->the_short() + " groans "
      "painfully, opens " + person->query_possessive() + " eyes and slowly "
      "picks " + person->query_objective() + "self up.\n", person );
   person->remove_property( PASSED_OUT_PROP );
} /* come_around() */

/** @ignore yes */
void wander_about( object person, int time, int id ) {
   int i, flag;
   string *direcs;

   if( person->query_property( PASSED_OUT_PROP ) || !environment( person ) )
      return;

   if( random( 4 ) )
      person->submit_ee( "wander_about", ({ 8, 8 }), EE_ONCE );

   direcs = (string *)environment( person )->query_direc();
   while( sizeof( direcs ) && !flag ) {
      i = random( sizeof( direcs ) );
      if( /* !environment( person )->query_obvious_exit( direcs[ i ] ) || */
          !environment( person )->query_door_open( direcs[ i ] ) ) {
         direcs = delete( direcs, i, 1 );
         continue;
      }

      person->add_property( UNKNOWN_MOVE, 1 );
      if( compass[direcs[i]] )
         flag = person->exit_command( 0, ({ direcs[ i ], "$N attempt$s to "
            "walk in one direction and instead stumble$s off towards the "
            "$T." }) );
      else
         flag = person->exit_command( 0, ({ direcs[ i ], "$N stumble$s off "
            "towards the $T." }) );

      tell_object( person, "You attempt to get up and walk and just "
         "manage to stumble around a bit.\n" );
      person->remove_property( UNKNOWN_MOVE );

      if( !flag )
         direcs = delete( direcs, i, 1 );
   }
} /* wander_about() */
string extra_look( object player, mixed *args ) {
    return capitalize(player->query_pronoun()+" looks rather pale with cold,"
    " clammy skin and a look of dizziness.\n");
} /* extra_look() */