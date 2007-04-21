/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: get.c,v 1.63 2002/02/22 00:48:08 ceres Exp $
 */
#include <obj_parser.h>
#include <move_failures.h>
#include <player.h>

// These were determined empirically (with my hand :) -- Jeremy
#define DROP_H 40
#define PINCH_H 120

#define MAX_GET_NUMBER 20

inherit "/cmds/base";

#define TP this_player()

object* find_matching_obs(string match_str, object dob) {
   class obj_match result;

   result = (class obj_match)match_objects_in_environments(match_str, dob);
   if (result->result != OBJ_PARSER_SUCCESS) {
      add_failed_mess(match_objects_failed_mess(result));
      return 0;
   }
   return result->objects;
}

mixed cmd(object * obs, string dir, string indir, mixed *args) {
   object *dest;
   object ob;
   object dob;
   object *fail_dest;
   object *too_many;
   mixed *fail;
   mixed *ret;
   mixed amt;
   string sh;
   string hand_unit;
   string match_str;
   int i;
   int num;
   int cap;
   int perc;
   int handful;
   int total_num;
   mixed we;
   object env;
   object *bing;

   fail_dest = ({ });
   cap = (int) TP->query_max_weight();

   if (indir && sizeof(obs) > 10) {
      add_failed_mess("Please be more specific as to what you want "
                      "to get from.\n");
      return 0;
   }

   if (indir) {
      match_str = args[0];
      dest = obs;
   } else {
      if (environment(TP)->query_mirror_room()) {
         dest = ({ environment(TP), environment(TP)->query_mirror_room() });
      } else {
         dest = ({ environment(TP) });
      }
   }

   too_many = ({ });
   foreach(dob in dest) {
     if (!is_in_me_or_environment(dob, this_player())) {
         fail_dest += ({ dob });
         continue;
      }

      if (dob->cannot_get_stuff() || dob->query_closed()) {
         fail_dest += ({ dob });
         continue;
      }

      if (living(dob) && !dob->allowed_to_loot(this_player())) {
        fail_dest += ({ dob });
        continue;
      } else if (indir) {
         obs = find_matching_obs(match_str, dob);
      }
      if (!obs) {
         continue;
      }

      if (total_num + sizeof(obs) > MAX_GET_NUMBER) {
         if (total_num > MAX_GET_NUMBER) {
            too_many += obs;
            obs = ({ });
         } else {
            too_many += obs[MAX_GET_NUMBER - total_num..];
            obs = obs[0..MAX_GET_NUMBER - total_num - 1];
         }
      }
      total_num += sizeof(obs);
      ret = ({ ([ ]), ([ ]), ([ ]), ([ ]), ([ ]), });
      fail = ({ });
      foreach(ob in obs) {
        env = environment(ob);
        // If the environment of the object & the direct object are different
        // something is wrong. It'll either be that it's a collective object
        // that is in the wrong place or someone's using 'get xxx in yyy'
        if(env != dob) {
          if(ob->query_collective())
            ob->move(dob);
          else
            dob = env;
        }

        if ((living(dob) && !dob->allowed_to_loot(this_player(), ob)) ||
            (ob->query_liquid() && ob->query_food_object())) {
          fail += ({ ob });
          num++;
          continue;
        }

         if (ob->query_continuous()) {
            // Can only get a handful at a time.
            // Id like to make it so you need to have a free hand,
            // but I'm not sure if it's worth it...
            //if (!TP->query_free_limbs())
            //   continue;
            amt = ob->query_amount_types();
            if (amt["handful"]) {
               hand_unit = "handful";
               handful = amt["handful"][0];
            } else if ((amt["drop"]) && (ob->query_liquid())) {
               hand_unit = "drops";
               handful = amt["drop"][0] * DROP_H;
            } else if (amt["pinch"]) {
               hand_unit = "pinches";
               handful = amt["pinch"][0] * PINCH_H;
            } else {            // Dunno what to do here - I guess give it to them
               handful = ob->query_amount();
            }
            if (ob->query_amount() > handful) {
               // Knock it down to a carryable amount
               // (I hope this is right...)
               if (!match_str) {
                  match_str = ob->query_name();
               }
               if (i = strsrch(match_str, "of") >= 0) {
                  i += 3;
               }
               if (hand_unit == "handful") {
                  handful = 1;
               }
               match_str = handful + " " + hand_unit + " of " +
                  match_str[i..];
               fail += ({ "all of the " + ob->short() });
               ob = ob->query_parse_id(({ handful, match_str }));
               if (!ob) {
                  continue;
               }
               num++;
            }
         }

         if (function_exists("do_get", ob) ||
             function_exists("command_control", ob)) {
            if (function_exists("do_get", ob)) {
               we = (int) call_other(ob, "do_get", dob, 0, match_str,
                                     ({ 0, match_str }),
                                     "<direct:object> from <indirect:object>");
            } else {
               we = ob->command_control("get", dob, 0, match_str,
                                        ({ 0, match_str }),
                                        "<direct:object> from <indirect:object>");
            }
            if (!objectp(we)) {
               if (we) {
                  we = (int) ob->query_weight();
                  perc = (we * 100) / cap;
                  if (perc >= 95) {
                     i = 4;
                  } else {
                     i = perc / 25;
                  }
                  if (!ret[i][env]) {
                     ret[i][env] = ({ ob });
                  } else {
                     ret[i][env] += ({ ob });
                  }
               } else {
                  if (member_array(ob, this_player()->query_succ_mess_indir())
                      == -1 && !query_notify_fail() && !living(ob)) {
                     fail += ({ ob });
                  }
                  num++;
               }
               continue;
            } else {
               ob = we;
            }
         }
#ifndef __DISTRIBUTION_LIB__
         if(interactive(TP) &&
            PLAYER_MULTIPLAYER_HANDLER->check_multiplayers("get", TP, ob)) {
           fail += ({ ob });
           num++;
         } else 
#endif
         if ((int) ob->move(TP) == MOVE_OK) {
            we = (int) ob->query_weight();
            perc = (we * 100) / cap;
            if (perc >= 95) {
               i = 4;
            } else {
               i = perc / 25;
            }
            if (!ret[i][env]) {
               ret[i][env] = ({ ob });
            } else {
               ret[i][env] += ({ ob });
            }
         } else {
            fail += ({ ob });
            num++;
         }
      }

      /* Print out the different weight messages */
      for (i = 0; i < sizeof(ret); i++) {
         if (sizeof(ret[i])) {
            /* Print out the different environments for the weights. */
            foreach(env, bing in ret[i]) {
               if (this_player()->query_succ_mess_dir()) {
                  bing -= this_player()->query_succ_mess_dir();
               }
               if (sizeof(bing)) {
                  sh = query_multiple_short(bing);
                  write("You " + ({ "get", "get with a bit of difficulty",
                                    "struggle somewhat to get",
                                    "find it very difficult to get",
                                    "use all your strength and just barely manage to get" })
                        [i] + " " + sh + " from " + inside_the_short(dob) +
                        ".\n");
                  say(capitalize((string) TP->one_short()) + " " +
                      ({ "gets", "gets with a bit of difficulty",
                         "struggles somewhat to get",
                         "finds it very difficult to get",
                         "uses all " + TP->query_possessive() +
                         " strength and just barely manages to get" })[i] +
                      " " + sh + " from " + inside_a_short(dob) + ".\n");
               }
            }
         }
      }

      if (sizeof(fail)) {
         add_failed_mess("You cannot get $I.\n", fail);
      }
   }

   //
   // This means everything failed.
   //
   if (num == total_num) {
      if (query_notify_fail()) {
         return 0;
      }
      if (indir) {
         //if (!num) {
            //add_failed_mess("There is no " + match_str + " in $I.\n", dest);
         //}
         if (sizeof(fail_dest)) {
            add_failed_mess("You cannot get anything from $I.\n", fail_dest);
         }
      } else {
         add_failed_mess("You cannot get $I.\n", obs);
      }
      return -1;
   } else {
      //
      // Success!
      //
      if (sizeof(too_many)) {
         add_succeeded_mess(({ "You cannot pick up more than " +
                               query_num(MAX_GET_NUMBER) +
                               " objects at a time; " "discarding $I.\n",
                               "" }), too_many);
      }
   }
   return 1;
}                               /* cmd() */

mixed *
query_patterns()
{
   return ({ "<indirect:object:here>", (: cmd($1, 0, 0, 0) :),
             "<string> from <indirect:object>", (: cmd($1, $2, $3, $4) :) });
}                               /* query_patterns() */
