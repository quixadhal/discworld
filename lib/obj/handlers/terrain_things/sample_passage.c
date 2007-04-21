inherit "/std/room";

void setup() {
   set_short( "dusty corridor" );
   set_light( 20 );
   set_room_size( 5 );
   set_long( "You are in a dry, dusty corridor.  It looks as though it has "+
         "been deserted for a long time.\n" );
   add_item( "dust", "There's quite a bit of dust in here, collecting along "+
         "the bottom of the walls." );
} /* setup() */

string query_exit_type( string word ) { return "corridor"; }
