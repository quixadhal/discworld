/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: drink_silly.c,v 1.1 1998/01/06 04:16:50 ceres Exp pinkfish $
 * $Log: drink_silly.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.drink".
 * <p>
 * Describe the arguments in here.
 * @classification npc.drink
 * @see help::effects
 */
/**
 * This is the drink yourself silly effect...
 * <p>
 * Keeps drinking until we run out of money.  Then gets depressed...
 * <p>
 * The argument will be the call back function to be used when the
 * effect ends.  ie: ran out of money :)  If the arguemet is a 
 * function pointer, it will be evaluated.  If it is a str thing that
 * function will be called on the player, if it is an array of two elements..
 * the first contains the object and the second the function.
 */
#include <effect.h>
#include <pub.h>
#include <shops/pub_shop.h>

#include "path.h"

/** @ignore yes */
mixed normalise(mixed arg, object player) {
   if (stringp(arg)) {
      arg = ({ arg, player });
   } else if (functionp(arg)) {
      arg = ({ arg, player });
   } else if (!pointerp(arg)) {
      arg = ({ });
   }
   return arg;
} /* normalise() */

/*
 * This effect will make the cute little critters eat things.
 */
/** @ignore yes */
void beginning( object player, mixed arg, int id ) {
   player->submit_ee("buy_something", 5, EE_CONTINUOUS);
   player->do_command("stand at bar");
   return normalise(arg, player);
} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2, int id ) {
   return normalise(time1, player) + normalise(time2, player);
} /* merge_effect() */

/** @ignore yes */
void end(object player, mixed arg, int id) {
   int i;

   for (i = 0; i < sizeof(arg); i += 2) {
      if (functionp(arg[i])) {
         evaluate(arg[i]);
      } else {
         call_other(arg[i], arg[i+1]);
      }
   }
} /* end() */
 
/** @ignore yes */
string query_classification() { return "npc.drink"; }

/** @ignore yes */
void buy_something(object player, mixed arg, int id) {
   mapping items;
   mixed *stuff;
   string *rabbit;
   int i;
   int value;
   mixed ret;
   string place;
   string name;
   class menu_item menu;

   /* Ok.  Now we try and buy something... */
   items = environment(player)->query_menu_items();
   rabbit = ({ });
   if (mapp(items)) {
      stuff = keys(items);

      place = environment(player)->query_property( "place" );
      if ( !place || ( place == "" ) ) {
         place = "default";
      }
      value = (int)player->query_value_in( place );
      if ( place != "default" ) {
         value += (int)player->query_value_in( "default" );
      }

      /* Now, we only want stuff with an alcohol rating :) */
      if (environment(player)->query_pub()) {
         foreach (name, menu in items) {
            if (menu->intox > 0 &&
                menu->cost < value) {
               rabbit += ({ name });
            }
         }
      } else {
         foreach (name, stuff in items) {
            if (stuff[DR_INTOX] > 0 &&
                stuff[DR_COST] < value) {
               rabbit += ({ name });
            }
         }
      }
   }

   if (!sizeof(rabbit)) {
      player->delete_effect(player->sid_to_enum(id));
   } else {
      do {
         i = random(sizeof(rabbit));
         ret = player->do_command("buy "+rabbit[i]);
         rabbit = rabbit[0..i-1] + rabbit[i+1..];
      } while (ret <= 0 && sizeof(rabbit));
      if (ret <= 0) {
         /* Ran out of usable money :( */
         player->delete_effect(player->sid_to_enum(id));
      }
   }
} /* buy_something() */
