/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bury.c,v 1.21 2003/07/08 07:20:31 pinkfish Exp $
 * 
 */
#include <move_failures.h>
#include <cmds/bury.h>
#include <playtesters.h>

inherit "/cmds/base";

#define TP this_player()
#define GP_INCREMENT 10

#define MAX_BURY_NUMBER 20

mixed cmd(object *obs) {
   string person;
   mixed effs, *xp;
   string* messages;
   object ob, tmp;
   object *sobs;
   object *fobs;
   object *too_many;
   int gp;

#ifdef PT
   if (!PLAYTESTER_HAND->query_playtester(this_player()->query_name()) &&
       !this_player()->query_creator()) {
      add_failed_mess("This command is in play testing at the moment.\n");
      return 0;
   }
#endif

   sobs = ({ });
   fobs = ({ });
   too_many = ({ });
   if (TP->query_property("dead")) {
      add_failed_mess("You are a disembodied spirit, how do you expect to bury "
                      "anything at all?\n");
      return 0;
   }
   if(sizeof(filter_array(TP->query_attacker_list(),
                          (: environment($1) == environment(this_player()) :)))) {
      add_failed_mess("You cannot bury items while in combat.\n");
      return 0;
   }

   if(environment(TP)->query_property("no burial")) {
      add_failed_mess("You cannot bury things here.\n");
      return 0;
   }

   fobs = filter(obs, (: !is_in_me_or_environment($1, this_player()) :));
   obs -= fobs;
     
   foreach (ob in obs) {
      if (sizeof(sobs) >= MAX_BURY_NUMBER) {
         too_many += ({ ob });
      } else if ((ob->query_owner() == this_player()->query_cap_name()) ||
                 ((ob->get() == MOVE_OK) ||
                  (ob->get() == MOVE_TOO_HEAVY)) &&
                 (ob->query_property("no burial") != 1) &&
                 !ob->ok_to_bury(this_player())) {

        if (ob->query_property("player") != 1 ||
            ob->query_owner() == this_player()->query_name()) {
            // If the weapon is owned, record its burial.
            effs = ob->effects_matching("mudlib.owned.weapon");
            if(sizeof(effs)) {
               person = ob->arg_of(effs[0]);
               log_file("BURIAL", "%s: %s's %s buried by %s\n", ctime(time()),
                        person, ob->query_short(), TP->query_name());
            }
                                    
            environment(this_player())->add_effect(BURY_EFFECT, ob);
            //ob->move("/room/rubbish");
            sobs += ({ ob });
            if (ob->query_property("corpse bit") || 
                ob->query_property("money")) {
               gp--;
            } else if(base_name(ob) == "/obj/corpse" &&
                      !ob->query_property("already buried")) {
               gp += GP_INCREMENT;
               ob->add_property("already buried", 1);
               
               // This hands out the rest of the death Xp when the object
               // is buried.
               xp = ob->query_property("XP");
               if(xp && sizeof(xp) == 2) {
                 foreach(tmp in xp[0]) {
                   if(tmp)
                     tmp->adjust_xp(xp[1], 1);
                 }
               }
               ob->remove_property("XP");
            }
         } else {
            fobs += ({ ob });
         }
      } else {
         fobs += ({ ob });
      }
   }
  
   if (!sizeof(sobs)) {
      if (sizeof(fobs)) {
         if (member_array(TP, fobs) == -1) {
            add_failed_mess("You cannot bury " + 
                            query_multiple_short(fobs)+".\n");
            return 0;
         } else {
            add_failed_mess("You cannot bury " +
                            query_multiple_short(fobs - ({TP}) + 
                                                 ({"yourself"}))+
                            ".\n");
            return 0;
         }
      } else {
         add_failed_mess("You cannot find anything here to bury!\n");
         return 0;
      }
   }

   if (sizeof(too_many)) {
      write("You can only bury up to " + MAX_BURY_NUMBER + 
            " items at a time, not burying " + 
            query_multiple_short(too_many) + ".\n");
   }
 
   messages = environment(TP)->query_burial_message(); 
   if (messages) {
      write(replace_string(messages[0], "$objs$", query_multiple_short(sobs)));
      say(replace(messages[1], ({ "$N", TP->the_short(), "$objs$", query_multiple_short(sobs) })));
   } else if (environment(TP)->query_property("location") != "outside") {
      write("You tidy up the place, clearing away "+
            query_multiple_short(sobs)+".\n");
      say( (string)this_player()->one_short() +
           " tidies up the place, clearing away "+
           query_multiple_short( sobs ) +".\n" );
   } else {
      write("You bury "+query_multiple_short(sobs)+" deep within the earth.\n");
      say( (string)this_player()->one_short() +
           " buries "+ query_multiple_short( sobs ) +
           " deep within the ground.\n" );
   }
   
   TP->adjust_gp(gp);
   
   return 1;
} /* cmd() */

/**
 * The default bury is to bury corpses.
 */
mixed *query_patterns() {
  return ({ "<indirect:object:here>", (: cmd($1) :),
            "",
            (: cmd(match_objects_for_existence("corpse", environment(TP))) :) });
} /* query_patterns() */

