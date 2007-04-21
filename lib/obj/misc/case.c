/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: case.c,v 1.1 1998/10/14 06:53:20 terano Exp $
 *
 *
 */

/*
 * $Locker:  $
 * $Id: case.c,v 1.1 1998/10/14 06:53:20 terano Exp $
 *
 *
 */

/**
 * Auction room display case
 * @author Terano
 * @started Late 1997
 */

#define READ "/cmds/living/r_ead"
inherit "/obj/baggage";

void setup() {

  set_name("case");
  set_long("This is a nice glass display case that looks out of place "
           "here, you could \"peruse\" things in it perhaps.\n");
  add_alias("container");
  add_adjective("display");
  set_transparent();
  set_weight(1000);
  set_main_plural("cases");
  set_max_weight(500000);
  reset_get();
  reset_drop();

}// end setup()

void init() {
 
  this_player()->add_command( "peruse", this_object(), 
                              "<indirect:object:direct-obs> in <direct:object>");
}

int do_peruse( object *obs ) {

  obs = filter( obs, (: environment( $1 ) == this_object() :) );

  if ( !sizeof( obs ) ) {
    add_failed_mess( "You have to browse something in the display case.\n" );
    return 0;
  }

  if ( sizeof( obs ) > 1 ) {
    add_failed_mess( "You can only browse 1 thing at a time.\n" );
    return 0;
  }
 
  write( "You peruse the "+ obs[0]->query_short() +".\n" );
  write( obs[0]->long() );
  READ->cmd( ({ obs[0] }) );
  this_player()->add_succeeded_mess( this_object(), "" );
  return 1;

}

int test_add( object item, int flag ) {

  if ( base_name( previous_object( 1 ) )[0..4] == "/cmds" ) {
    if ( this_player()->query_creator() )
      return 1;
    else return 0;
  }
  return 1;
}//end test_add


int test_remove( object item, int flag ) {
  if ( base_name( previous_object( 1 ) )[0..4] == "/cmds" ) {
    if ( this_player()->query_creator() )
      return 1;
    else return 0;
  }
  return 1;
}//end test_remove

void setup_case( string name, string long ) {
  add_alias( name );
  set_long( long + "You feel you could \"peruse\" things in it.\n" );
}