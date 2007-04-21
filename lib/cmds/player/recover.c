/**
 * This method allows you to recover items from the room.
 * @author Pinkfish
 * @started Mon Feb  7 01:04:04 PST 2000
 * @changed Sun Jul 16 23:43:49 PST 2000 Taffyd 
 * Added MAX_RECOVER.
 */
#include <obj_parser.h>

inherit "/cmds/base";

#include <move_failures.h>
#include <cmds/bury.h>
#include <playtesters.h>
#include <player.h>

/**
 * This is the maximum number of items that can be
 * recovered.
 */
#define MAX_RECOVER 20

int cmd_recover(string name) {
   object* obs;
   object* fail;
   object* ok_me;
   object* ok_here;
   object cont;
   object ob;
   class obj_match omatch;

#ifdef PT
   if (!PLAYTESTER_HAND->query_playtester(this_player()->query_name()) &&
       !this_player()->query_creator()) {
      add_failed_mess("This command is in play testing at the moment.\n");
      return 0;
   }
#endif

   if (this_player()->query_property("dead")) {
      add_failed_mess("You're a disembodied spirit, how do you expect to "
                      "recover anything at all?\n");
      return 0;
   }

   cont = BURY_EFFECT->query_buried_container(environment(this_player()));
   if (!cont) {
      add_failed_mess("There is nothing buried here.\n");
      return 0;
   }
   omatch = (class obj_match)match_objects_in_environments(name, cont);
   if (omatch->result != OBJ_PARSER_SUCCESS) {
      add_failed_mess(match_objects_failed_mess(omatch));
      return 0;
   }
   obs = omatch->objects;
   
   if ( sizeof( obs ) > MAX_RECOVER ) {
      add_failed_mess( "You can only recover " + 
        query_num( MAX_RECOVER ) + " items at a time.\n" );
      return 0;
   }

   fail = ({ });
   ok_me = ({ });
   ok_here = ({ });
   if (sizeof(obs)) {
      //
      // Recover them...
      //
      foreach (ob in obs) {
#ifndef __DISTRIBUTION_LIB__
        if(interactive(this_player()) &&
           PLAYER_MULTIPLAYER_HANDLER->check_multiplayers("recover",
                                                           this_player(),
                                                           ob)) {
          fail += ({ ob });
        } else 
#endif
         if (ob->move(this_player()) == MOVE_OK) {
            ok_me += ({ ob });
         } else if (ob->move(environment(this_player())) == MOVE_OK) {
            ok_here += ({ ob });
         } else {
            fail += ({ ob });
         }
      }
      if (!sizeof(ok_me) && !sizeof(ok_here)) {
         add_failed_mess("Unable to recover $I.\n", fail);
         return 0;
      }
      if (sizeof(ok_me)) {
         add_succeeded_mess("$N $V $I.\n", ok_me);
      }
      if (sizeof(ok_here)) {
         add_succeeded_mess("$N $V $I and leave$s " +
                            (sizeof(ok_here) > 1?"them":"it") +
                            " here.\n", ok_here);
      }
      return 1;
   } else {
      ok_me = all_inventory(cont);
      if (sizeof(ok_me)) {
         add_failed_mess("Unable to recover " + name + ", available objects "
                       "are " + query_multiple_short(ok_me) +
                      ".\n");
      } else {
         add_failed_mess("Unable to recover " + name + ".\n");
      }
      return 0;
   }
} /* cmd_recover() */

mixed* query_patterns() {
   return ({ "<string'buried object'>", (: cmd_recover($4[0]) :) });
} /* query_patterns() */
