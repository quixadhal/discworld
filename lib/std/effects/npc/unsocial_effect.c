/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unsocial_effect.c,v 1.2 2002/09/19 10:15:36 tilly Exp $
 * $Log: unsocial_effect.c,v $
 * Revision 1.2  2002/09/19 10:15:36  tilly
 * Typo!
 *
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "curse.magic.social".
 * <p>
 * Describe the arguments in here.
 * @classification curse.magic.social
 * @see help::effects
 */
#include <effect.h>

/** @ignore yes */
void beginning( object player,mixed *arg, int id ) {
   tell_object( player, "The spectral image of a "+arg[0]+" appears in front "
         "of you and looks sadly at you through pale eyes.  You shudder "
         "in horror as the image passes through you with tears in its "
         "eyes.  You find your yourself less able to communicate from the "
         "shock.\n");
   tell_room( environment( player ), player->the_short()+" suddenly looks "
         "straight ahead and shudders for no visible reason.\n", player );
   player->submit_ee( "reminder_mess", 20, EE_CONTINUOUS );
   player->submit_ee( 0, arg[1], EE_REMOVE );
   player->set_max_sp( 0 );
} /* beginning() */


/** @ignore yes */
int merge_effect( object player, int time1, int time2, int id ) {
   player->submit_ee( 0, time1 + time2 - (int)player->expected_tt(),
         EE_REMOVE );
   return time1 + time2;
} /* merge_effect() */
 
/** @ignore yes */
void end( object player, int time, int id ) {
   int number;
   tell_object( player, "You feel you have recovered and are ready to "
         "communicate with the world again.\n");
   number= player->query_real_max_sp();
   player->adjust_max_sp( number );
} /* end() */
 
void reminder_mess( object player, mixed *arg, int id ) {
   string message;
   if (!random(4))
       switch(random(2)){
         case 0 : 
           message = "The spectral image of a "+arg[0]+" just like you had "
            "as a "
            "child appears in front of you.  Before you know what you are "
            "doing you strike out at the "+arg[0]+" spilling spectral blood "
            "everywhere.\n";
           break;       
         default :
           message = "You have a vision of yourself walking down a street "
            "when suddenly a masked "+arg[0]+" jumps out of the shadows "+
            "and cuts you into pieces with its natural weapons.\n";
           break;
       } /* switch() */
   else message = "The pitiful looking ghost of a "+arg[0]+
         " suffering from horrifying wounds drifts past you.\n";
            

   tell_object( player, message);
 
   tell_room( environment( player ),player->the_short()+" shudders and "
         "looks worried.\n", player );
   player->run_away();
   if (sizeof(player->effects_matching("priest.paralysis"))){
     tell_object(player,"You look around in panicked confusion, unsure "
                 "of exactly where you are.\n");
     tell_room( environment( player ),player->the_short()+ " stares dazedly "
                "at their surroundings.\n",player);
     tell_object(player,((environment(player)->query_long() )+"\n"));
  }
} /* reminder_mess() */

/** @ignore yes */
string query_classification() { return "curse.magic.social"; }
