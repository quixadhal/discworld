/*
* $Id: perm_it.c,v 1.3 2001/06/01 20:58:43 ceres Exp $
*/

/* Thanks to Turrican, Gruper, and Mansarde, without whom I would have
 * given up on this project.
 * - Aragorn
 */

inherit "/cmds/base";

int cmd( object player ) {
  
  object *corpses, corpse;
  int i;
  
  /* Obviously we'll only want to search player corpses */
  corpses = filter( children( "/obj/corpse" ),
                    (: $1 && $1->query_property("player" ) :) );
  
  /* Now we look for corpses they own */
  corpses = filter( corpses,
                    (: $1 && $1->query_ownership() ==
                     $(this_player()->query_name()) :) );
  
  if( sizeof( corpses ) ) { 
    foreach( corpse in corpses ){
       /*Let's see if any of these corpses have already been permitted*/
      if(member_array(player->query_name(), corpse->query_permitted()) != -1) {
        i++;
      } else {
        /*
         * The heart of the command, this adds the name to the corpses
         * array of permitted players
         */
        corpse->give_permission( player->query_name() );
      }
    }
    
  } else { 
    /* Whoops, we don't have any corpses loaded */
    add_failed_mess("You don't seem to have a corpse!  Why not die and try "
                    "again?%^RESET%^\n");
    return 0;
  }           
  
  /* Let them know if we were successful or if we just wasted our time */
  if( i == sizeof( corpses ) ){
    write(capitalize(player->query_name()) + " is already permitted to "
          "remove items from your corpse.%^RESET%^\n" );
  } else { 
    write(capitalize(player->query_name()) + " is now permitted to "
          "remove items from your corpse.%^RESET%^\n" );
    tell_object(player, "%^BOLD%^%^GREEN%^You now have permission to get "
                "items from " + this_player()->query_cap_name() +
                "'s corpse.%^RESET%^\n");
  }
  return 1;
} 

mixed *query_patterns() {
  return ({ "<indirect:player>", (: cmd($1[0]) :) });
} /* query_patterns() */
