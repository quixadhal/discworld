/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: owned_weapon.c,v 1.2 1999/02/03 03:38:55 gruper Exp $
 * $Log: owned_weapon.c,v $
 * Revision 1.2  1999/02/03 03:38:55  gruper
 * Rodion added query_owner().
 *
 * Revision 1.1  1998/01/06 04:40:23  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

mixed set_holder( object thing, int pos ) {
   this_object()->remove_hide_invis( "concealed" );
   if ( objectp( thing ) )
      if ( !thing->query_corpse() &&
            ( (string)thing->query_name() != arg() ) )
         return 0;
   return (mixed)player->set_holder( thing, pos );
} /* set_holder() */

string query_owner(){
   return (string) arg();
}/*query_owner()*/

