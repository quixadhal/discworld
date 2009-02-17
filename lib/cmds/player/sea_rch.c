/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sea_rch.c,v 1.20 2003/07/15 08:01:01 pinkfish Exp $
 * $Log: sea_rch.c,v $
 * Revision 1.20  2003/07/15 08:01:01  pinkfish
 * Fix up some bits.
 *
 * Revision 1.19  2003/04/12 21:33:44  ceres
 * Added pk_assist() check
 *
 * Revision 1.18  2003/01/01 23:14:32  drakkos
 * Fixed up the pluralisation of the messages and also replaced the 'ennui' part with something that makes more sense and doesn't make assumptions of the player.
 *
 * Revision 1.17  2002/10/24 14:24:40  danbala
 * Changed the rather ungainly fail-due-to-lack-of-gp-message.
 * (Thanks, Carmine)
 *
 * Revision 1.16  2001/12/08 14:30:04  drakkos
 * Changed the GP fail message to be a little more IC
 *
 * Revision 1.15  2001/11/08 07:03:34  pinkfish
 * Make it use up some gp.
 *
 * Revision 1.14  2001/03/09 22:50:22  pinkfish
 * Fix up a typo.
 *
 * Revision 1.13  2000/02/10 05:16:22  pinkfish
 * Make a check for combat after they have searched.
 *
 * Revision 1.12  2000/01/21 00:33:20  pinkfish
 * Fix up a syntax error.
 *
 * Revision 1.11  2000/01/21 00:32:47  pinkfish
 * Allow just a search to work.
 *
 * Revision 1.10  2000/01/21 00:32:04  pinkfish
 * Add in the here stuff...
 *
 * Revision 1.9  2000/01/21 00:21:12  pinkfish
 * Hopefully make search give back correct error messages.
 *
 * Revision 1.8  1999/10/25 23:06:09  pinkfish
 * Some fixes to make things look better.
 *
 * Revision 1.7  1999/08/29 06:48:07  pinkfish
 * Make it so you cannot search a room while fighting someone.
 *
 * Revision 1.6  1999/08/16 01:12:26  presto
 * Fixed message problem when multiple people search at the same time.  Used to say "Womble and frog searches around the place."
 *
 * Revision 1.5  1999/07/26 14:40:00  wodan
 * check if hide_invis is a mapping before taking the keys.
 *
 * Revision 1.4  1999/05/07 09:16:24  pinkfish
 * Stop ghosts from searching.
 *
 * Revision 1.3  1999/04/05 23:06:41  ceres
 * Modified to use perform_task
 *
 * Revision 1.2  1999/01/14 23:20:26  ceres
 * made it give the results _after_ the search
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <tasks.h>
#include <player.h>

inherit "/cmds/base";

#define SKILL "other.perception"

#define GP_COST 15

nosave mapping callouts = ([]);

int cmd(string str) {
   if (this_player()->query_specific_gp("other") < GP_COST) {
      add_failed_mess("You are too weary to complete a "
                      "proper search of your surroundings.\n");
      return 0;
   }

   if (this_player()->query_property("dead")) {
      add_failed_mess("Ghosts cannot search!\n");
      return 0;
   }

   if (sizeof(filter(this_player()->query_attacker_list(),
                     (: environment($1) == environment(this_player()) :)))) {
      add_failed_mess("You cannot search, you are in combat!\n");
      return 0;
   }

   /* This command takes four rounds. */
   this_player()->adjust_time_left( -ROUND_TIME * 4 );
   
   if (callouts[this_player()->query_name()]) {
      remove_call_out(callouts[this_player()->query_name()]);
   }
   
   callouts[this_player()->query_name()] =
      call_out("search_callout", 4*1, this_player(), str);
  
   this_player()->adjust_gp(-GP_COST); 
   write("You start to search around.\n");
   say(this_player()->the_short() + " $V$0=starts,start$V$ to search around.\n");
   return 1;
} /* cmd() */

void search_callout(object who, string str) {
   mapping hide_invis;
   int i;
   string *types;
   string* ground;
   mixed see;
   object *found, ob;

   if (who->query_fighting()) {
      write("Oops!  You appear to have entered combat and cannot "
            "finish searching.\n");
      return ;
   }
   
   if ( function_exists( "do_search", environment( who ) ) ) {
      i = (int)environment( who )->do_search( str );
      switch(i) {
      case 1:
         return;
      case 0:
         if(query_notify_fail())
            write(query_notify_fail());
         return;
      default:
         break;
      }
   }
   
   if(!str) {
      found = ({ });
      
      foreach(ob in all_inventory(environment(who))) {
         if(ob->query_visible(who) ||
            ob->query_creator() ||
            pk_assist(this_player(), ob))
           continue;

         hide_invis =  ob->query_hide_invis();
         if(mapp(hide_invis)) {
            types = m_indices( hide_invis );
         } else {
            types = ({});
         }
         if ( !sizeof( types ) ) {
            continue;
         }
         for ( i = 0; i < sizeof( types ); i++ ) {
            if ( hide_invis[ types[ i ] ][ 0 ] == who ) {
               continue;
            }
            see = hide_invis[ types[ i ] ][ 1 ];
            if ( intp( see ) ) {
               // coz they're searching its made easier to find stuff.
               see -= random(environment(who)->query_light());
               see /= 2;
               switch(TASKER->perform_task(who, SKILL, see, TM_FREE)) {
               case AWARD:
                  write("%^YELLOW%^"+
                        ({"You feel very perceptive", "You realise "
                             "something new about searching"})[random(2)]+
                             "%^RESET%^.\n");
               case SUCCEED:
                  found += ({ ob });
                  ob->remove_hide_invis(types[i]);
                  break;
               }
               break;
            }
         }
      }
   }
   
   if(sizeof(found)) {
      write("You search around and find "+query_multiple_short(found)+".\n");
   } else {
      ground = environment(who)->query_default_search_description();
      if (!ground) {
         write( ({
         "You search around for a while, but don't find anything.\n",
         "You scrounge around.  The ground does look interesting, "
            "you decide.\n",
         "You look carefully at everything, but you find nothing.\n",
         "After an intense search, you find nothing.\n" })[ random( 4 ) ] );
      } else {
         write(ground[random(sizeof(ground))]);
      }
   }
   say("$one_short:" + file_name(who) + "$ $V$0=searches,search$V$ around the "
       "place a bit.\n" );
   event(environment(who), "player_search");
   map_delete(callouts, who->query_name());
   return;
} /* search_callout() */

void interrupt_search(object who) {
   if(callouts[who->query_name()]) {
      remove_call_out(callouts[who->query_name()]);
      tell_object(who, "You stop searching.\n");
      map_delete(callouts, who->query_name());
   }
} /* interrupt_search() */

mixed* query_patterns() {
   return ({ "<string>", (: cmd($4[0]) :),
             "here", (: cmd(0) :),
             "", (: cmd(0) :) });
} /* query_patterns() */
