/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: stop.c,v 1.9 2003/07/16 20:28:33 pinkfish Exp $
 * $Log: stop.c,v $
 * Revision 1.9  2003/07/16 20:28:33  pinkfish
 * Increas eht random delay.
 *
 * Revision 1.8  2002/09/08 20:57:09  ceres
 * Modified to make stopping combat slightly variable
 *
 * Revision 1.7  2002/08/03 19:28:58  ceres
 * New combat system version
 *
 * Revision 1.5  1999/02/01 01:14:58  presto
 * Removed checks for things on attacker list being living.  It was stuffing
 * up for things like training dummys.  This is a test to see if anything else
 * gets broken.
 *
 * Revision 1.4  1999/01/14 23:20:32  ceres
 * Mods to support delayed searching.
 *
 * Revision 1.3  1998/04/14 02:28:16  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.2  1998/03/26 06:13:04  ceres
 * Made it so when you stop fighting you don't stop fighting things in
 * your environment.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

int no_okay;
object tmp;

mixed cmd( string word ) {
   object *atts;

   switch( word ) {
      case "debate" :
      case "debating" :
         this_player()->interrupt_debate();
         break;
      case "spell" :
         this_player()->interrupt_spell();
         break;
      case "ritual" :
         this_player()->interrupt_ritual();
         break;
      case "fight" :
      case "fighting" :
         atts = this_player()->query_attacker_list();
         if ( !sizeof( atts ) ) {
            write( "You are not fighting anyone.\n" );
            return 1;
         }
         if ( (int)this_player()->query_hp() < 0 ) {
            write( "You have already died; stopping now is pointless.\n" );
            return 1;
         }
         write( "Stopping fight.  Please wait.\n" );
         call_out( "stop_fight", 5+random(10), this_player() );
         return 1;
      default:
         no_okay = 1;
         cmd( "debate" );
         cmd( "spell" );
         cmd( "ritual" );
         cmd( "fight" );
         no_okay = 0;
/*
 * This gets called anyway, on its way through.
         this_player()->drunk_check( "stop" );
*/
   }

   "/cmds/player/sea_rch"->interrupt_search(this_player());
   if ( !no_okay )
      write( "Okay.\n" );
   return 1;
} /* cmd() */

void stop_fight( object who ) {
  object *atts, ob;
  
  if ( !who )
    return;

  atts = who->query_attacker_list();
  atts -= ({ 0 });
  tmp = who;
  if(atts == ({ }))
    return;
  
  tell_object( who, "You stop fighting "+
               query_multiple_short( map_array( atts,
         (: $1->query_property( "dead" ) ? (string)$1->a_short() + " (dead anyway)" : $1 :) ) ) +".\n" );

  foreach(ob in atts)
    who->stop_fight(ob);
  
} /* stop_fight() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "{debate|debating|spell|ritual|fight|fighting}", (: cmd($4[0]) :) });
} /* query_patterns() */
