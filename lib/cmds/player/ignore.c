/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ignore.c,v 1.15 2003/07/13 04:31:59 pinkfish Exp $
 * 
 */
#include <drinks.h>
#include <language.h>
#include <player.h>
#include <player_handler.h>

#define TP this_player()

inherit "/cmds/base";

int cmd_list() {
   string *ignoring;

   ignoring = TP->query_property("ignoring");
   if(!ignoring) {
      ignoring = ({ });
   }
   if ( !sizeof(ignoring) )  {
      write( "You are not ignoring anyone.\n" );
   } else  {
      ignoring = sort_array(ignoring, 1);
      write( "You are currently ignoring " +
              query_multiple_short( map(ignoring, (: capitalize($1) :)), 
                                    0, 0, 1, 0 ) + ".\n" );
   }
   return 1;
} /* cmd_list() */

int cmd( string str) {
   string *ignoring;
 
   ignoring = TP->query_property("ignoring");
   if(!ignoring) {
      ignoring = ({ });
   }

   str = lower_case( TP->expand_nickname(str) );
   if ( !(PLAYER_HANDLER->test_user(str))  &&  !find_player(str)) {
      if (find_living(str)) {
         add_failed_mess("You cannot ignore an npc.\n");
         return 0;
      } else {
         add_failed_mess( "Ignoring someone that doesn't exist?  Is this some "
                           "kind of zen?\n" );
         return 0;
      }
   }
   if (TP->query_name() == str) {
      add_failed_mess("Ignoring yourself.  Novel.\n");
      return 0;
   }
   /**
     -- Why shouldn't players be able to ignore creators?
         -- We have tell! for official business, and if a player
         -- is ignoring a creator, there shouldn't be any 'non-official'
         -- contact. -- Terano.

     -- If you need a better reason, imagine Lobula as a creator, and not
         -- being able to ignore him.

         else if( !TP->query_creator() && PLAYER_HANDLER->test_creator(str)) {
     add_failed_mess("You cannot ignore creators.\n");
     return 0;
   }
   */
   if ( member_array( str, ignoring ) == -1 )  {
      ignoring += ({ str });
      TP->add_property("ignoring", ignoring, 604800);
   } else  {
      write( "You are already ignoring " + capitalize(str) + ".\n" );
   }

   write("You are now ignoring "+capitalize(str)+".\n");
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<string'person'>", (: cmd($4[0]) :),
             "", (: cmd_list() :) });
} /* query_patterns() */
