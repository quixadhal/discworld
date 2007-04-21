/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: goto_destination.c,v 1.4 1999/03/05 07:21:06 ceres Exp $
 * $Log: goto_destination.c,v $
 * Revision 1.4  1999/03/05 07:21:06  ceres
 * Made it only log during debug
 *
 * Revision 1.3  1999/01/06 20:36:30  ceres
 * Added more error checking.
 *
 * Revision 1.2  1998/02/08 06:54:41  sin
 * Fixed a problem that could cause the goto_destination effect to stop working.
 *
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the goto destination effect, it also handles selling
 * stuff at the destination. This effect
 * has a classification of "npc.move".
 * <p>
 * The argument to this can be one of three things, it can be a
 * string, an object or an array.  If it is a string the string
 * specifies the room which the npc will try to go to.  If it
 * is an object then the npc will try and get to the living object
 * or room specified by the object.  The array consists of three
 * elements:
 * <ol>
 * <li>The destination as given above, string or object
 * <li>The end function, 0 for nothing, "sell" to sell the inventory
 * <li>The delay before heading off to the next destination.
 * </ol>
 * <p>
 * If the thing to do at the end is a function it is called with
 * the following parameters: <br>
 *   <code>function(object player, int found_it, string destination)</code><br>
 * The found_it parameter will be 1 if the room is correct or 0
 * if it is not correct.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.move
 * @see help::effects
 */
#include <effect.h>

#include "path.h"

#define LOG_FILE "EFFECTS"

private string standardize(mixed dest);

/**
 * @ignore
 */
void call_move_to(object player, string dest)
{
  if (!player) {
#ifdef DEBUG
    log_file(LOG_FILE,
             sprintf("%O %s: player became 0 before call_move_to() called\n",
                     this_object(), ctime(time())));
#endif
    return;
  }

  if(!dest) {
#ifdef DEBUG
    log_file(LOG_FILE,
             sprintf("%O %s: dest became 0 before call_move_to() called\n",
                     this_object(), ctime(time())));
#endif
    return;
  }
    
  player->move_me_to(dest);
}

/**
 * @ignore
 */
void remove_effect(object player, int id)
{
  if (player)
    player->remove_effect(id);
}

/*
 * This is used to get a npc to wander somewhere and do something.
 * It takes either a string, being the destination to go to, or a
 * array of 3 elements.  The first is the destination, the second
 * is what to do when you get there and the third is the delay before
 * the next sequence occurs.
 *
 * If the thing to do at the end is a function it is called with
 * the following parameters:
 *   function(object player, string destination, int found_it)
 *
 * The found_it parameter will be 1 if the room is correct or 0
 * if it is not correct.
 */
/** @ignore yes */
void beginning( object player, mixed arg, int id ) {
   mixed dest;

   if (pointerp(arg)) {
      if (sizeof(arg) > 3) {
         arg = arg[0..2];
      } else while (sizeof(arg) < 3) {
         arg += ({ 20 });
      }
   } else {
      arg = ({ arg, 0, 20 });
   }

   arg[0] = standardize(arg[0]);

   if (!dest) {
      while (!arg[0] && sizeof(arg) > 3) {
         arg = arg[3..];
         arg[0] = standardize(arg[0]);
      }
      if (!arg[0]) {
         /* Remove ourselves. */
         call_out("remove_effect", 2, player, id);
         return ;
      }
   }

   if (objectp(arg[0])) {
      call_out("call_move_to", 2, player, file_name(environment(arg[0])));
   } else {
      call_out("call_move_to", 2, player, arg[0]);
   }
} /* beginning() */

/** @ignore yes */
private string standardize(mixed dest) {
   if (objectp(dest)) {
      if (!living(dest)) {
         dest = file_name(dest);
      }
   } else if (stringp(dest)) {
      if (catch(dest->rabbit_well())) {
         dest = 0;
      } else {
         dest = file_name(find_object(dest));
      }
   }

   return dest;
} /* standardize() */

/** @ignore yes */
mixed merge_effect( object player, mixed arg1, mixed arg2, int id ) {
   if (!pointerp(arg1)) {
      arg1 = ({ arg1, 0, 20 });
   }

   if (sizeof(arg1) > 0) {
      while (sizeof(arg1) < 3) {
         arg1 += ({ 20 });
      }
      arg1[0] = standardize(arg1[0]);
   }

   if (!pointerp(arg2)) {
      arg2 = ({ arg2, 0, 20 });
   }

   if (sizeof(arg2) > 0) {
      while (sizeof(arg2) < 3) {
         arg2 += ({ 20 });
      }
      arg2[0] = standardize(arg2[0]);
   }

   arg1 += arg2;

   while (!arg1[0] && sizeof(arg1) > 3) {
      arg1 = arg1[3..];
      arg1[0] = standardize(arg1[0]);
   }
   if (!arg1[0]) {
      /* Remove ourselves. */
      call_out("remove_effect", 2, player, id);
      return ({ });
   }

   /* Stack them up... */
   return arg1;
} /* merge_effect() */

/** @ignore yes */
void effect_womble(object player, mixed *arr) {
  if(!player)
    return;
   player->add_effect(file_name(this_object()), arr);
} /* effect_womble() */

/**
 * This method sells everything the npc currently has and is not
 * marked with the property "start equipment".  This can be called from
 * outside the effect to sell things.
 * @param player the players whose inventory is to be sold
 */
void do_the_sell(object player) { 
   object *obs;
   int i;
   
   /* Sell stuff... */
   obs = all_inventory(player);
   for (i = 0; i < sizeof(obs); i++) {
      if (!obs[i]->query_property("start equipment")) {
         call_out((: $1->do_command("sell "+file_name($2)) :),
                   1, player, obs[i]);
      }
   }
} /* do_the_sell() */

/** @ignore yes */
void end( object player, mixed room, int id ) {
   int ok;

   if (pointerp(room)) {
      if (sizeof(room) > 3) {
         /* 20 seconds between actions... */
         call_out( "effect_womble", room[2], player, room[3..]);
      }

      if (!room[0]) {
         ok = 0;
      } else if (objectp(room[0])) {
         ok = (environment(room[0]) == environment(player));
      } else {
         ok = file_name(environment(player)) == room[0];
      }

      if (functionp(room[1])) {
         evaluate(room[1], player, room[0], ok, room[1]);
      }

      if (stringp(room[1])) {
         if (ok) {
            switch (room[1]) {
               case "sell" :
                  do_the_sell(player);
                  break;
               default :
                  player->do_command(room[1]);
                  break;
            }
         }
      }

      if (pointerp(room[1])) {
         call_other(room[1][0], room[1][1], player, room[0], ok, room[1]);
      }
   }
} /* end() */
 
/** @ignore yes */
string query_classification() { return "npc.move"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "goto_destination"; }
