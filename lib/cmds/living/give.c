/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: give.c,v 1.22 2002/02/22 01:14:48 ceres Exp $
 */

#include <move_failures.h>
#include <player.h>

inherit "cmds/base";

#define TP this_player()
#define SUPPRESS_MESS "suppress give messages"

mixed cmd(mixed *indir, string *indir_match) {
  string sh;
  string s1;
  string failmess;
  int tot;
  int max;
  int ok;
  object *per;
  object *ret;
  object *fail;
  object pobj;
  object ob;
  object *obs;
  object *succ;
  object *keep;

  per = indir[1];
  succ = ({ });
  keep = ({ });
  failmess = "";
  
  foreach(pobj in per) {
    obs = indir[0];
    obs -= per;

    if (pobj->query_property("player") && !interactive(pobj)) {
      failmess = pobj->the_short()+" seems too chalky to accept your "
        "gift.\n";
      continue;
    }

    if (pobj == TP) {
     failmess = "You nag yourself for a while, but can't convince "
                 "yourself to accept things from yourself.\n";
     continue;
    }

    if (!sizeof(obs)) {
      failmess = "Nothing to give to "+ pobj->short() +".\n";
      continue;
    }
    ret = ({ });
    fail = ({ });
    foreach(ob in obs) {
      if ( member_array( ob, succ ) > -1 ) {
        continue;
      }
      if (ob->query_keep()) {
         keep += ({ ob });
         continue;
      }
      if (function_exists("do_give", ob) ||
          function_exists("command_control", ob)) {
        if (function_exists("do_give", ob)) {
          max = call_other(ob, "do_give", pobj, indir_match[0],
                                indir_match[1],
                                ({ indir_match[0], indir_match[1] }),
                                "<direct:object:me> to <indirect:living>");
        } else {
          max = ob->command_control("give", pobj, indir_match[0],
                                    indir_match[1],
                                    ({ indir_match[0], indir_match[1] }),
                                    "<direct:object:me> to <indirect:living>");
        }
        if (max == 0)  {
          fail += ({ ob });
          continue;
        }
      }
#ifndef __DISTRIBUTION_LIB__
      // this_player() drops the object
      PLAYER_MULTIPLAYER_HANDLER->record_object("give", TP, ob);
      
      // pobj picks it up again.
      if(pobj->query_property("player") && interactive(TP) &&
         "/obj/handlers/multiplayer"->check_multiplayers("give", pobj, ob)) {
        fail += ({ ob });
      } else 
#endif
      if (pobj->query_closed()  ||  ob->move(pobj) != MOVE_OK) {
        fail += ({ ob });
      } else  {
        ret += ({ ob });
        tot += ob->query_weight();
      }
    }
    if (sizeof(ret)) {
      ok = 1;
      succ += ret;
      if (pobj->query_clothing()) {
        s1 = pobj->query_pocket_mess();
      } else {
        s1 = "$ob_short$";
      }
      sh = query_multiple_short(ret);

      if ( interactive( pobj ) || !pobj->query_property( SUPPRESS_MESS ) ) {
        tell_object(TP, "You give "+ sh +" to "+
                  replace_string(s1, "$ob_short$", pobj->one_short())+
                  ".\n");
        tell_room(environment(TP), TP->one_short() + " gives " + sh +
               " to " + replace_string(s1, "$ob_short$",
                                      pobj->one_short()) +
               ".\n", ({ TP, pobj }));
        tell_object(pobj, capitalize(TP->one_short() ) + " gives "+
                    sh + " to you.\n" );
      }

      if (living(pobj) && (max = pobj->query_max_weight()))
        if ((max = tot*100/max) > 25)
          if (max >= 95) {
            tell_room(environment(TP),
                      pobj->the_short()+" staggers under a weight "+
                      pobj->query_pronoun()+" can only just carry.\n",
                      ({ pobj }) );
            pobj->event_say(TP, "You stagger under a weight you can only "
                            "just carry.\n");
          } else {
            tell_room(environment(TP), pobj->the_short()+ ({
              " is only mildly discomforted by the additional weight.\n",
              " braces "+pobj->query_objective()+"self to take the load.\n",
              " stumbles as "+pobj->query_pronoun()+" takes the load.\n"
              })[(max/25)-1], ({ pobj }) );
            pobj->event_say(TP, "You"+ ({
              " are only mildly discomforted by the additional weight.\n",
              " brace yourself under the load.\n",
              " stumble as you take the load.\n"
              })[(max/25)-1]);
          }
    }
    if (sizeof(fail)) {
      failmess += "You cannot give "+query_multiple_short(fail)+ " to "+
                  pobj->one_short() +".\n";
    }
  }
  if (!ok) {
    add_failed_mess(failmess);
    if (sizeof(keep)) {
       add_failed_mess("You have $I set to be kept.\n", keep);
    }
  }
  return ok;
}

mixed *query_patterns() {
  return ({ "<indirect:object:me> to <indirect:living>",
            (: cmd($1, $3) :) });
}
