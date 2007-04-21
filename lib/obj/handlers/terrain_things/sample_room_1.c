inherit "/std/room";

void setup() {
   set_short( "small room" );
   set_light( 20 );
   set_room_size( 15 );
   set_long( "You are in a small room.  The walls, floor and ceiling are "+
         "comprised of large stone blocks.  The room is quite bare.\n" );
   add_item( ({ "wall", "floor", "ceiling" }), "The walls, floor and "+
         "ceiling of the room are formed from large stone blocks." );
   add_item( "large stone block", "The blocks of stone that make up the "+
         "surfaces of this room are not too rough, but they were never "+
         "polished smooth either." );
} /* setup() */

string query_exit_type( string word ) { return "corridor"; }
