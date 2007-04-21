/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: watch.c,v 1.3 2001/11/07 21:28:07 pinkfish Exp $
 * $Log: watch.c,v $
 * Revision 1.3  2001/11/07 21:28:07  pinkfish
 * Stop dead people from watching.
 *
 * Revision 1.2  1999/10/15 06:20:36  taffyd
 * Added watch stop.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#define COST 50

inherit "/cmds/base";

mixed cmd() {
   if ( this_player()->query_blinded() ) {
      add_failed_mess( "How can you watch anything?  You're blind.\n" );
      return 0;
   }
   if (this_player()->query_property("dead")) {
      add_failed_mess( "Dead people don't watch. It is rude!\n");
      return 0;
   }
   if ( sizeof( (int *)this_player()->effects_matching( "mind.alertness" ) ) ) {
      add_failed_mess( "You are already alert and watching "
        "your surroundings.\n" );
      return 0;
   }
   if ( this_player()->query_specific_gp( "other" ) < COST ) {
      add_failed_mess( "You can't concentrate enough to watch "
        "your surroundings.\n" );
      return 0;
   }
   this_player()->adjust_gp( -COST );
   this_player()->add_effect( "/std/effects/other/alertness", 60 + 
     (int)this_player()->query_skill_bonus( "other.perception" ) / 2 );
   return 1;
} /* cmd() */

int stop_watching() { 
   int *enums, id;

   enums = this_player()->effects_matching( "mind.alertness" );

   if ( sizeof( enums ) ) {
      foreach( id in enums ) {
         this_player()->delete_effect( id );
      }
   }
   else {
      add_succeeded_mess( ({ "You are not currently watching!\n", "" }) );
   }

   return 1;
} /* stop_watching() */

mixed *query_patterns() {
   return ({ "stop", (: stop_watching() :),
     "", (: cmd() :) });
} /* query_patterns() */

