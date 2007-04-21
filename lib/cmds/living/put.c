/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: put.c,v 1.23 2003/04/30 19:43:46 ceres Exp $
 *
 */

#include <move_failures.h>
#include <player.h>

#define MAX_PUT_NUMBER 20

inherit "cmds/base";

#define TP this_player()
string con;

mixed cmd(mixed *indir,
          string *indir_match,
          string *args)
{
   string person;
   string thing;
   string sh;
   int tot;
   int max;
   object *per;
   object *ret;
   object *fail;
   object pobj;
   object ob;
   object *obs;
   object *succ;
   object *too_many;
   int num;

   con = args[1];
   per = indir[1];
   thing = indir_match[0];
   person = indir_match[1];
   succ = ({ });
   too_many = ({ });
   
   // Check to see if they have too many drated objects.
   //if (sizeof(indir[0]) > MAX_PUT_NUMBER) {
   //too_many = indir[0][MAX_PUT_NUMBER..];
   //indir[0] = indir[0][0..MAX_PUT_NUMBER - 1];
   //}

   foreach(pobj in per) {
      if (living(pobj) && !pobj->query_living_container()) {
         tell_object(TP,
                     "You cannot put things " + con + " living things.\n");
         continue;
      }
      obs = indir[0];
      obs -= per;
      if (!sizeof(obs)) {
         tell_object(TP, "Nothing to " + query_verb() + " " + con + " " +
                     inside_the_short(pobj) + ".\n");
         continue;
      }
      if (!is_in_me_or_environment(pobj, this_player())) {
         add_failed_mess("Cannot $V $I " + con + " " + pobj->the_short() +
                         ".\n", obs);
         continue;
      }

      ret = ({ });
      fail = ({ });
      foreach(ob in obs) {
         if (member_array(ob, succ) > -1) {
            continue;
         }
         if (num > MAX_PUT_NUMBER) {
            too_many += ({ ob });
            continue;
         }
         if (function_exists("do_put", ob) ||
             function_exists("command_control", ob)) {
            if (function_exists("do_put", ob)) {
               max = (int) call_other(ob, "do_put", pobj, thing, person,
                                      ({ thing, person }),
                                      "<direct:object> " + con +
                                      " <indirect:object>");
            } else {
               max = ob->command_control("put", pobj, thing, person,
                                         ({ thing, person }),
                                         "<indirect:object> " +
                                         con + " <indirect:object>");
            }
            if (max == 0) {
               fail += ({ ob });
               continue;
            }
         }
         if (pobj->query_closed() || ob->move(pobj) != MOVE_OK) {
            fail += ({ ob });
         } else {
            ret += ({ ob });
            tot += (int) ob->query_weight();

#ifndef __DISTRUBUTION_LIB__
            if(interactive(TP))
              PLAYER_MULTIPLAYER_HANDLER->record_object("dropped", TP, ob);
#endif         
            num++;
         }
      }
      if (sizeof(ret)) {
         succ += ret;
         sh = query_multiple_short(ret);
         add_succeeded_mess("$N $V $I " + con + " " +
                            inside_the_short(pobj) + ".\n", ret);
      }
      if (sizeof(fail)) {
         add_failed_mess("You cannot $V $I " + con + " " +
                         inside_the_short(pobj) + ".\n", fail);
      }
   }
   if (sizeof(too_many)) {
      if (sizeof(succ) > 0) {
         add_succeeded_mess(({ "You cannot handle more than " +
                               query_num(MAX_PUT_NUMBER) +
                               " objects with $V; discarding $I.\n", "" }),
                            too_many);
      } else {
         add_failed_mess("You cannot handle more than " +
                         query_num(MAX_PUT_NUMBER) +
                         " objects with $V; discarding $I.\n", too_many);
      }
   }
   return sizeof(succ) > 0;
}                               /* cmd() */

string query_con() {
  return con;
}

mixed *query_patterns()
{
   return ({ "<indirect:object:me> {in|on|into} <indirect:object>",
             (: cmd($1, $3, $4) :) });
}                               /* query_patterns() */
