inherit "/std/room";

void setup() {
   set_short( "rock-walled tunnel" );
   set_light( 20 );
   set_room_size( 5 );
   set_long( "You are tunnel with rough-hewn rock walls.  It looks as though "+
         "it has been deserted for a long time.\n" );
   add_item( ({ "rough-hewn rock wall", "rock" }), "The walls are rough, "+
         "where the tunnel was hewn out of the rock." );
} /* setup() */

string query_exit_type( string word ) { return "corridor"; }
