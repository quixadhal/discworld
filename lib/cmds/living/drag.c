#include <player.h>
#include <room.h>
#include <player_handler.h>

inherit "/cmds/base";

mixed cmd( object *indirect_obs, string direction) {
  string owner;
  object owner_ob;
  string *words, word;

  if( this_player()->query_property( "dead" ) )
    return add_failed_mess( "You're dead.  You can't drag anything.\n" );

  if( objectp( (object)this_player()->query_dragging() ) )
    return add_failed_mess( "You're already dragging something.\n" );

  if( sizeof( indirect_obs ) > 1 )
    return add_failed_mess( "You can only drag one thing at a time.\n" );

  if( indirect_obs[0] == this_player() )
    return add_failed_mess( "You try and drag yourself away from the keyboard, "
      "but you cannot.  Oh no!\n");

  /* For the moment, any living thing not passed out cannot be dragged. */
  if( living( indirect_obs[ 0 ] ) ) {
    if( !indirect_obs[ 0 ]->query_property( PASSED_OUT_PROP )
        || pk_check( indirect_obs[ 0 ], this_player() ) )
      return add_failed_mess( (string)indirect_obs[ 0 ]->the_short() +
        " doesn't want to be dragged away, thank you very much.\n" );
  } else
    if( base_name(indirect_obs[0]) == "/obj/corpse" ) {
      owner = indirect_obs[ 0 ]->query_owner();
      words = explode( lower_case( owner ),  " " );
      words = filter( words, (: PLAYER_HANDLER->test_user($1) :) );
      foreach( word in words ) {
          owner_ob = find_player( lower_case( word ) );
          if( owner_ob && userp( owner_ob ) &&
            pk_check( owner_ob, this_player() ) &&
            member_array ( this_player()->query_name(), indirect_obs[0]->
            query_permitted() ) == -1 )
            return add_failed_mess( "For some reason, you can't get hold of "+
              (string)indirect_obs[ 0 ]->the_short() +".\n" );
      }
  }

  if( ( environment( indirect_obs[ 0 ] ) != environment( this_player() ) ) ||
    indirect_obs[ 0 ]->get() )
      return add_failed_mess( "For some reason, you can't get hold of " +
        (string)indirect_obs[ 0 ]->the_short() +".\n" );
    
  write( "You get hold of " + (string)indirect_obs[ 0 ]->the_short() + ".\n" );
  say( (string)this_player()->one_short() + " gets hold of " +
    (string)indirect_obs[ 0 ]->a_short() + ".\n" );

  // Too heavy
  if( (int)indirect_obs[ 0 ]->query_complete_weight() +
    (int)this_player()->query_loc_weight() > 2 *
    (int)this_player()->query_max_weight() ) {
      write( "You struggle and strain, but you can't budge " +
        (string)indirect_obs[ 0 ]->query_objective() + ".\n" );
      say( (string)this_player()->one_short() + " struggles and strains, but " +
        (string)this_player()->query_pronoun() + " can't budge " +
        (string)indirect_obs[ 0 ]->query_objective() + ".\n" );
  } else {
    this_player()->set_dragging( indirect_obs[ 0 ] );
    // No exit that way
    if( !this_player()->exit_command( direction ) )
      write( "You can't drag " + (string)indirect_obs[ 0 ]->the_short() +
        " that way.\n" );
    else
      if( interactive( this_player() ) )
        indirect_obs[0]->add_property( "dropped",
          ({this_player()->query_name(), query_ip_number( this_player() ),
          time()}), 1800 );
    this_player()->reset_dragging();
  }
  write( "You let go of " + (string)indirect_obs[ 0 ]->the_short() + ".\n" );
  say( (string)this_player()->the_short() + " releases " +
    (string)this_player()->query_possessive() + " hold of " +
    (string)indirect_obs[ 0 ]->the_short() + ".\n" );
  
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return 
    ({ "<indirect:object:here> [to] <word'direction'>",
         (: cmd( $1, implode($4[1..], " ") ) :),
       "<indirect:object:here> [to] <string'direction'>",
         (: cmd( $1, implode($4[1..], " ") ) :) });
} /* query_patterns() */
