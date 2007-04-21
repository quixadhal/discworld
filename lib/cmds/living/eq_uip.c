/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: eq_uip.c,v 1.6 2001/11/06 05:20:52 pinkfish Exp $
 * $Log: eq_uip.c,v $
 * Revision 1.6  2001/11/06 05:20:52  pinkfish
 * Fix up the errors with armour.
 *
 * Revision 1.5  2001/11/05 20:48:57  pinkfish
 * Make it only equip weapons.
 *
 * Revision 1.4  2001/11/05 20:38:54  pinkfish
 * Stop it equiping sheilds incorrectly.
 *
 * Revision 1.3  2001/03/08 10:02:29  mansarde
 * Fixed a typo in strnig. (although i think strnig is quite a
 * fun word)
 *
 * Revision 1.2  2001/03/08 09:04:49  pinkfish
 * Fixed up the formatting.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#define TP this_player()

inherit "/cmds/base";

int cmd() {
   int no_limbs;
   string stuff;
   object thing;
   object *equipped;
   object *things;

   equipped = ({ });
   things = all_inventory( TP ) - TP->query_wearing() -
            TP->query_holding();
   foreach ( thing in things ) {
      // DO the limb checkjing first
      no_limbs = thing->query_no_limbs();
      if ( no_limbs > TP->query_free_limbs() ) {
         continue;
      }
      if ( thing->query_wearable() ) {
         if ( !TP->wear_armour( thing, 0 ) ) {
            equipped += ({ thing });
         }
         continue;
      }
      if (thing->query_weapon() && no_limbs) {
         if ( sizeof( TP->set_hold( thing,
               member_array( 0, TP->query_holding() ) ) ) ) {
            equipped += ({ thing });
         }
      }
   }
   if ( !sizeof( equipped ) ) {
      return notify_fail( "You have no equippable items.\n" );
   }
   stuff = query_multiple_short( equipped );
   say( (string)TP->one_short() +" equips "+ stuff +".\n" );
   write( "You equip "+ stuff +".\n" );
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd() :) });
} /* query_patterns() */
