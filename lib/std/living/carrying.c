/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: carrying.c,v 1.17 2003/04/15 20:56:09 ceres Exp $
 * $Log: carrying.c,v $
 * Revision 1.17  2003/04/15 20:56:09  ceres
 * Fixed possible runtime if they have no limbs
 *
 * Revision 1.16  2001/08/10 09:01:15  pinkfish
 * Make it fix all the corpse errors.
 *
 * Revision 1.15  2001/05/23 00:28:12  pinkfish
 * Fix up all the warnings (really).
 *
 * Revision 1.14  2001/05/23 00:16:06  pinkfish
 * Fix up a warning.
 *
 * Revision 1.13  2001/04/20 18:51:00  ceres
 * Fixed runtime if you do an 'i' while logging in
 *
 * Revision 1.12  1999/11/18 09:23:11  pinkfish
 * Fix it up to move the hidden determination code out of the armour_list
 * code.
 *
 * Revision 1.11  1999/10/25 23:09:45  pinkfish
 * Some changes to make dealing with transports a little better.
 *
 * Revision 1.10  1999/07/20 23:44:57  presto
 * Fixed so that when it's dark, when you do an inventory, you get
 * "Your purse is ... " instead of "His/Her purse is ..."
 *
 * Revision 1.9  1999/05/18 22:21:25  taffyd
 * Fixed small bug with light levels doing weird descriptions for players.
 *
 * Revision 1.8  1999/05/18 21:28:06  pinkfish
 *  Forcibly unlocked by rywfol
 *
 * Revision 1.7  1998/08/25 10:09:06  pinkfish
 * Reverse the order of the inventory display.
 *
 * Revision 1.6  1998/07/24 05:04:07  pinkfish
 * Fix this up to work with something I then changed back...
 *
 * Revision 1.5  1998/03/29 16:31:42  pinkfish
 * Revese the order of the inventory stuff.
 *
 * Revision 1.4  1998/03/28 17:37:25  pinkfish
 * Documetnaiotn and changes to support the new clothing handler.
 *
 * Revision 1.3  1998/03/05 11:37:17  pinkfish
 * changed the find_inv_match() call to use all_inventory()
 *
 * Revision 1.2  1998/02/04 02:41:21  presto
 * Fixed problem with getting limb list from race objects
 *
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 * 
*/
/**
 * This file contains all the code need to handle the types of carrying
 * which npcs tend to involve themselves in.
 * @author Pinkfish
 */
#include <clothing.h>
#include <money.h>

object* query_wearing();

/**
 * This method returns the list of types of things which are passed into
 * the array.
 * <p>
 * Two elements in an array is returned, the first element is the list of
 * all of the different types which are worn and the second is the list
 * of types corresponding to each of the inputs.
 * @param things the things to check
 * @return a two element array
 * @see armour_list()
 */
protected varargs string *query_types_worn( object *things ) {
   int i;
   string *types;
   mixed *stuff;
   
   types = ({ });
   if ( !things ) {
      things = (object *)this_object()->query_wearing();
   }
   stuff = (mixed *)things->query_type();
   for ( i = sizeof( stuff ) - 1; i > -1; i-- ) {
      if ( stringp( stuff[ i ] ) ) {
         stuff[ i ] = ({ stuff[ i ] });
      }
      if ( !pointerp( stuff[ i ] ) ) {
         printf( "%O (%s) has an invalid type.  Please bug report it.\n",
                 things[ i ], (string)things[ i ]->query_short() );
         stuff[ i ] = ({ "broken" });
      }
      types += stuff[ i ];
   }
   return ({ types, stuff });
} /* query_types_worn() */

/**
 * This method returns the items that are hidden by something on top of
 * them.
 * @param looker the person looking at the items
 * @param things an optional set of things
 * @return the hidden worn types
 */
object* query_wearing_hidden(object looker, object *things) {
   mixed* bing;
   string* types;
   mixed* obj_types;
   object* worn;
   object* ret;
   int i;
   int found_under;
   string fluff;
   string under_type;
   string* under;

   if (!things) {
      worn = query_wearing();
   } else {
      worn = things;
   }
   bing = query_types_worn( worn );
   types = bing[0];
   obj_types = bing[1];
   ret = ({ });
   for (i = 0; i < sizeof(worn); i++) {
      if (worn[i]->query_visible(looker)) {
         found_under = 0;
         foreach (fluff in obj_types[i]) {
            under = CLOTHING_HANDLER->query_clothing_hiding( fluff );
            if ( under && sizeof(under)) {
               foreach (under_type in under) {
                  if (member_array(under_type, types) != -1) {
                     ret += ({ worn[i] });
                     found_under = 1;
                     break;
                  }
               }
            }
            if (found_under) {
               break;
            }
         }
      } else {
         ret += ({ worn[i] });
      }
   }
   return ret;
} /* query_wearing_hidden() */

/**
 * This method returns a string list of the armours which the player
 * is wearing.  It will show also modify the list to show only hidden
 * items (items under other items are considered hidden).
 * @param things the things to get the list of armours from
 * @param hidden_only only show the list of hidden objects
 * @see query_types_worn()
 * @see query_living_contents()
 */
protected string armour_list( object *things, int hidden_only, int self) {
   string word;
   object* hidden;

   hidden = query_wearing_hidden( this_player(), things); 
   if (hidden_only) {
      things = filter(hidden, (: $1->query_visible( this_player() ) :));
   } else {
      things = things - hidden;
   }
   if ( !sizeof( things ) ) {
      return 0;
   }
   if (!self) {
      /* Order them in a reasonable way */
      things = sort_array(things,
               (: CLOTHING_HANDLER->query_clothing_depth($2->query_type())  -
                  CLOTHING_HANDLER->query_clothing_depth($1->query_type()) :));
   }
   word = query_multiple_short( things );
   return word;
} /* armour_list() */

/**
 * This method does the inventory of the living objects we all know and
 * love.  It is used in the long description and the inventory commands.
 * @param self are we showing our own inventory
 * @see armour_list()
 * @index inventory
 */
string query_living_contents( int self ) {
   string s;
   string fish;
   string *limbs;
   string *bits;
   string *strs;
   object *holding;
   object *arms;
   object *carry;
   object *conts;
   object money;
   object ob;
   int i;
   int coins;
   int pos;

   s = "";
   strs = ({ });
   conts = ({ });
   limbs = this_object()->query_limbs();
   if (!limbs) {
      limbs = ( this_object()->query_race_ob() )->query_limbs();
   }
   /* I wonder why this uses find_inv_match? */
   conts = filter(all_inventory(), (: $1->short(0) :) );
   //conts = (object *)this_object()->find_inv_match();
   if(!limbs || !sizeof(limbs))
     holding = ({ });
   else
     holding = (object *)this_object()->query_holding() + ({ });
   
   arms = (object *)this_object()->query_armours() - holding;
   money = present(MONEY_ALIAS, this_object());
   if(!arms) {
      arms = ({ });
   }
   carry = conts - arms - holding - ({ money });
   for (i = 0; i < sizeof(holding); i++) {
     if (holding[i] && environment(holding[i]) == this_object()) {
       bits = ({ limbs[i] });
       ob = holding[i];
       holding[i] = 0;
       while ((pos = member_array(ob, holding)) != -1) {
         bits += ({ limbs[pos] });
         holding[pos] = 0;
       }
       strs += ({  ob->a_short() + " (" +
                     query_multiple_short(bits, "a", 1)+")" });
     }
   }
   if (sizeof(strs)) {
      s += "$I$+10,+0=%^RED%^Holding%^RESET%^ : " + 
         query_multiple_short( strs ) +
         ".\n$I$-10,-0=";
   }
   if ( sizeof( arms ) ) {
      if ( fish = armour_list( arms, 0, self) ) {
         s += "$I$+10,+0=%^YELLOW%^Wearing%^RESET%^ : " + fish + 
            ".\n$I$-10,-0=";
      }
      if ( self && (fish = armour_list(arms, 1, self) ) ) {
         s += "$I$+10,+0=%^YELLOW%^(under)%^RESET%^ : " + fish + 
            ".\n$I$-10,-0=";
      }
   }
   if ( sizeof( carry ) ) {
      fish = this_object()->query_contents(0, carry, 1);
      if ( fish ) {
         if (pointerp(fish)) {
            s += "$I$+10,+0=%^GREEN%^" + fish[1] + "%^RESET%^: " + fish[0] +
               "$I$-10,-0=";
         } else {
            s += "$I$+10,+0=%^GREEN%^Carrying%^RESET%^: " + fish +
               "$I$-10,-0=";
         }
      }
   }
   if ( self && environment() &&
        this_object()->check_dark(environment()->query_light()) > -1) {
      if ( money ) {
         fish = query_multiple_short( (string *)money->half_short( 1 ) );
      } else {
         fish = 0;
      }
      if ( s == "" ) {
         s = "You are empty handed.\n";
      }
      s += "Your purse contains " + (fish?fish:"only moths") + ".\n";
   } else if(money) {
      coins = (int)money->query_number_coins();    
      if (self) {
         s += "Your purse is ";
      } else {
         s += capitalize( (string)this_object()->query_possessive() ) +
              " purse is ";
      }
      switch(coins) {
      case 0..10:    
         s += "home to only moths!\n"; 
         break;
      case 11..100:  
         s += "tinkling with coins.\n"; 
         break;
      case 101..300: 
         s += "bulging with coins.\n"; 
         break;
      default:       
         s += "fit to burst!\n";
         break;
      }
   }
   return s;
} /* query_living_contents() */
