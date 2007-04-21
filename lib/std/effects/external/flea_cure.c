/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: flea_cure.c,v 1.2 1999/10/24 16:12:47 gruper Exp $
 * $Log: flea_cure.c,v $
 * Revision 1.2  1999/10/24 16:12:47  gruper
 * Did I change this one?
 *
 * Revision 1.1  1998/01/06 04:10:22  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "body.smell.insecticide".
 * <p>
 * Describe the arguments in here.
 * @classification body.smell.insecticide
 * @see help::effects
 */
/*
** flea_cure - for curing the flea effect.
**
** Written by: Jeremy - 6-Oct-95
**
** Deutha added smell_string(), capitalised Grflx and put a "u" in odour :)
*/

#include <effect.h>

int infested;

int cleanse( object thing, int bonus ) {
    int i, flag, *effnums;
    class effect *effs;
    
    if ( !thing )
      return 0;
    if ( !living( thing ) )
      return 0;
    effnums = (int *)thing->effects_matching(
              "disease.parasite.external.flea");
    if ( !sizeof( effnums ) )
      return 0;
    effs = (class effect *)thing->query_effs();
    for ( i = sizeof( effnums ) - 1; i >= 0; i-- )
      if ( effs[ effnums[ i ] ]->ob_name->test_remove( thing,
          effs[ effnums[ i ] ]->arg, effnums[ i ], bonus ) ) {
          thing->delete_effect( effnums[ i ] );
          flag = 1;
      }
    infested = flag;
    return flag;
} /* cleanse() */

/** @ignore yes */
void beginning( object player, int time ) {
   player->submit_ee( "make_smell", ({ 20, 30 }), EE_CONTINUOUS );
   player->submit_ee( "kill_flea", ({ 30, 40 }), EE_CONTINUOUS );
   call_out("cleanse", 0, player, time/10);
   //cleanse(player, time/10);
   player->submit_ee( 0, time, EE_REMOVE );
} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2 ) {
   player->submit_ee( 0, time1 + time2 - (int)player->expected_tt(),
         EE_REMOVE );
   if (!infested)
       call_out("cleanse", 0, player, (time1+time2)/10);
   return time1 + time2;
} /* merge_effect() */
 
/** @ignore yes */
void end( object player, int time ) {
   tell_object( player, "The odd odour is gone now.\n" );
} /* end() */
 
void kill_flea( object player ) {
    if (infested && random(2)) {
        tell_object( player, "You hear a faint scream and see a small dot " +
                    "fall past your eyes.\n" );
        tell_room( environment( player ), "You see a tiny speck leap from " +
                  (string)player->short() + "'s head, screaming pitifully.\n",
                  player );
    }
} /* kill_flea() */

void make_smell( object player ) {
    tell_object( player, "You notice a strange odour, like wet Grflx "
              "and rotting cabbage, in the air.\n" );
    tell_room( environment( player ),  "A smell reminiscent of a wet Grflx "+
              "and rotting cabbages seems to emanate from "+
              (string)player->short() + ".\n", player );
} /* make_smell() */

/** @ignore yes */
string query_classification() { return "body.smell.insecticide"; }

string smell_string( object player, int time ) {
   return "the odd odour of wet Grflx and rotting cabbage";
} /* smell_string() */
