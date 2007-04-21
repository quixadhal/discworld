/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: drop.c,v 1.13 2001/11/05 21:49:55 pinkfish Exp $
 * $Log: drop.c,v $
 * Revision 1.13  2001/11/05 21:49:55  pinkfish
 * Make them now drop kept items.
 *
 * Revision 1.12  2001/08/27 18:57:16  pinkfish
 * Fix up the stuff.
 *
 * Revision 1.11  2001/08/27 18:55:48  pinkfish
 * Fix up multipleayer defines.
 *
 * Revision 1.10  2001/08/17 04:05:58  ceres
 * Improved the multiplayer checks
 *
 * Revision 1.9  2000/07/31 23:36:33  taffyd
 * Added a few things for custom messages when dropping items.
 *
 * Revision 1.8  2000/04/15 03:51:53  ceres
 * Made a cockup with the timed property
 *
 * Revision 1.7  2000/04/09 12:01:00  ceres
 * Modified to track who dropped what.
 *
 * Revision 1.6  1999/08/14 15:27:34  presto
 * rewrote the if-else sieve after calling do_drop on the object
 *
 * Revision 1.5  1999/06/11 18:42:14  ceres
 * Removed list of discarded objects when dropping too many -- was causing over evaluation
 *
 * Revision 1.4  1999/03/24 10:19:05  taffyd
 * Fixed small bug with failure messages not being displayed.
 *
 * Revision 1.3  1998/10/01 07:05:43  pinkfish
 * Fix to work correctly with add_command and put in a cap on the number
 * of objects dealt with at once.
 *
 * Revision 1.2   1998/03/06 14:48:30   pinkfish
 * Changes to handle the extended inventory code.
 *
 * Revision 1.1   1998/01/06 05:28:43   ceres
 * Initial revision
 * 
*/
#include <move_failures.h>
#include <player.h>

#define MAX_DROP_NUMBER 20

inherit "/cmds/base";

#define TP this_player()

mixed cmd(object *obs, string str) {
   int num, max;
   object bing;
   string env_sh;
   object ob;
   object *fail;
   object *too_many;
   object *keep;
   mapping ret;
   object env;
   string drop_mess;
   //string sh;
   
   ret = ([ ]);
   fail = ({ });
   too_many = ({ });
   keep = ({ });
   if (sizeof(obs) > MAX_DROP_NUMBER) {
      too_many = obs[MAX_DROP_NUMBER..];
      obs = obs[0..MAX_DROP_NUMBER-1];
   }
   foreach(ob in obs) {
      if (!ob->short()) {
         continue;
      }
      if (ob->query_keep()) {
         keep += ({ ob });
         continue;
      }
      env = environment(ob);
      if (function_exists("do_drop", ob) ||
            function_exists("command_control", ob)) {
         if (function_exists("do_drop", ob)) {
            max = (int)call_other(ob, "do_drop", ({ }), str, 0, ({ str }),
                                             "<direct:object:me>");
         } else {
            max = ob->command_control("drop", ({ }), str, 0, ({ str }),
                                                   "<direct:object:me>");
         }
         if (max == 0)  {
            fail += ({ ob });
            continue;
         }
      }
      if (ob->move(environment(TP)) == MOVE_OK) {
         if (ret[env]) {
            ret[env] += ({ ob });
         } else {
            ret[env] = ({ ob });
         }
#ifndef __DISTRUBUTION_LIB__
         if(interactive(TP))
           PLAYER_MULTIPLAYER_HANDLER->record_object("dropped", TP, ob);
#endif         
      } else {
        fail += ({ ob });
      }
   }
   if (sizeof(ret)) {
      foreach (env, bing in ret) {
         //sh = query_multiple_short(bing);
         if (env != this_player()) {
            env_sh = " from inside " + inside_the_short(env);
         } else {
            env_sh = "";
         }
         
         /* If you want to define your own message for when you
          * drop an item... return the string here.
          */
         if ( drop_mess = this_player()->query_drop_mess() ) {
            add_succeeded_mess( drop_mess, bing );
         }
         else {
            add_succeeded_mess("$N $V $I" + env_sh + ".\n", bing);
         }
      }
      if (sizeof(too_many)) {
         add_succeeded_mess(({ "You can only drop " +
                         query_num(MAX_DROP_NUMBER) + " objects at one time; "
                         "discarding.\n", "" }));
      }
   } else {
      if (sizeof(keep)) {
         add_failed_mess("You are keeping $I.\n", keep);
      }
      if (sizeof(fail)) {
         add_failed_mess("You cannot drop $I.\n", fail);
         return 0;
      }
      if (sizeof(too_many)) {
         add_failed_mess("You can only drop " +
                         query_num(MAX_DROP_NUMBER) + " objects at one time; "
                         "discarding $I.\n", too_many);
         return 0;
      }
   }


   num = sizeof(fail)+sizeof(ret);

   if (!num) {
//      return notify_fail("Cannot drop "+str+".\n");
        return 0;
   }

   return 1;
}

mixed *query_patterns() {
   return ({ "<indirect:object:me>", (: cmd($1, $3) :) });
}
