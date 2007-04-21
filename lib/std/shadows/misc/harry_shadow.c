/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: harry_shadow.c,v 1.6 2002/05/24 17:54:29 pinkfish Exp $
 * $Log: harry_shadow.c,v $
 * Revision 1.6  2002/05/24 17:54:29  pinkfish
 * Remove all the pker property stuff.
 *
 * Revision 1.5  2002/05/24 17:41:11  pinkfish
 * Make the shadow remove the player killer function.
 *
 * Revision 1.4  2001/06/11 03:00:20  wobin
 * Added Perform, to the list of 'remove npk' status commandsdded
 *
 * Revision 1.3  2000/04/26 14:17:23  rhinehold
 * modified the shadowed adjust_hp to make sure the new arguments were caught and passed.
 *
 * Revision 1.2  1999/03/05 10:33:10  ceres
 * MOdified only to log in debug mode
 *
 * Revision 1.1  1998/01/06 04:39:04  ceres
 * Initial revision
 * 
*/
#include <player.h>

int protecting;
string protector;
object player;

void setup_shadow( object thing, string word ) {
   shadow( thing, 1 );
   player = thing;
   protector = word;
   protecting = 1;
   call_out( "dest_harry_shadow", 60 * 20 );
} /* setup_shadow() */

string query_harry_protector() { return protector; }

void dest_harry_shadow() { destruct( this_object() ); }

void zap_harry_shadow() {
   protecting = 0;
   tell_object( player, "You lose "+ protector +"'s protection.\n" );
   call_out( "dest_harry_shadow", 1 );
} /* zap_harry_shadow() */

int adjust_hp( int number, object thing, object weapon, string attack ) {
   if ( ( number > 0 ) || !protecting || 
       !thing || !pk_check(thing, player ) || player == this_player() )
      return (int)player->adjust_hp( number, thing, weapon, attack );
   return (int)player->query_hp();
} /* adjust_hp() */

int set_hp( int number, object thing ) {                     
   int current;
   current = (int)player->query_hp();
   if ( ( number > current ) || !protecting || !thing || !thing->query_player_killer() )
      return (int)player->set_hp( number, thing );
   return current;
} /* set_hp() */
 
/*
 * Hopefully there's no way a PKer can call these directly...
 *
 * void do_death() { return; }
 * int second_life() { return 1; }
*/

int attack_by( object thing ) {
   if ( !protecting || !thing->query_player_killer( ) )
      return (int)player->attack_by( thing );
   tell_object( thing, (string)player->the_short() +
         " are protected from combat by "+ protector +".\n" );
   tell_object( player, "You are protected from combat by "+
         protector +".\n" );
   thing->stop_fight( player );
   return 0;
} /* attack_by() */

int attack_ob( object thing ) {
   if ( protecting )
      zap_harry_shadow();
   return (int)player->attack_ob( thing );
} /* attack_ob() */

int query_unambushable() { return 1; }

void do_ambushed() {
   write( protector +" stops you from harming "+
         (string)player->the_short() +".\n" );
   tell_object( player, protector +" stops "+
         (string)this_player()->the_short() +" from harming you.\n" );
} /* do_ambushed() */

int no_offense() {
   if ( protecting )
      zap_harry_shadow();
   return (int)player->no_offense();
} /* no_offense() */

int query_player_killer() {
   return 0;
}

int command_shadowed( string verb, string args ) {
   if ( member_array( verb, ({ "cast", "scathe", "perform" }) ) == -1 )
      return (int)player->command_shadowed( verb, args );
   if ( protecting )
      zap_harry_shadow();
   return (int)player->command_shadowed( verb, args );
} /* command_shadowed() */

varargs int adjust_xp( int number, int shared ) {
#ifdef DEBUG
  log_file( "PROTECTED", ctime( time() ) +": "+ (string)player->query_name() +
         " gained "+ number +"xp ["+ implode( map_array( previous_object( -1 ),
         (: file_name( $1 ) :) ), ", " ) +"]\n" );
#endif  
   return (int)player->adjust_xp( number, shared );
} /* adjust_xp() */

varargs int add_skill_level( string skill, int lvl, mixed exp ) {
   string what;
   if ( !exp )
      exp = previous_object();
   if ( objectp( exp ) )
      what = " from "+ file_name( exp );
   else
      what = " for "+ exp +"xp";
#ifdef DEBUG
   log_file( "PROTECTED", ctime( time() ) +": "+ (string)player->query_name() +
         " gained "+ lvl +" in "+ skill + what +" ["+
         implode( map_array( previous_object( -1 ), (: file_name( $1 ) :) ),
         ", " ) +"]\n" );
#endif   
   return (int)player->add_skill_level( skill, lvl, exp );
} /* add_skill_level() */
