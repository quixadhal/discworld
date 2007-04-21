/**
 * Gagger object originally coded by Obilix 9 April 1997.
 * Turned into gag and ungag commands by Gruper 20 March 2000.
 */

#include <playerinfo.h>

inherit "/cmds/base";


int cmd( object victim ) {

   // Ungag them and log it
   if( !victim->query_property( "gagged" ) )
      return notify_fail( victim->query_short() +" is not gagged.\n" );
   
   victim->remove_property("gagged");
   log_file("GAG", ctime(time()) + ": " + this_player()->
         query_short() + " ungagged " + victim->query_short() + ".\n");
   //PLAYERINFO_HANDLER->add_entry( this_player(), victim->query_name(),
   //      "gag", "Ungagged." );

   // Messages...
   tell_object( victim, this_player()->query_short() + " ungags you.\n"
                        "%^YELLOW%^Be more careful with what you say "
                        "in future.%^RESET%^\n" );

   write( "Okay, ungagged " + victim->query_short() + ".\n" );

//   event( users(), "inform", this_player()->query_short()
//          + " ungagged " + victim->query_short(), "gag");

   return 1;

} // cmd()


mixed *query_patterns() {
   return ({ "<indirect:player'player'>",
                (: cmd( $1[0] ) :) });
} /* query_patterns() */
