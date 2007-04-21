#include <dirs.h>
#include <terrain.h>

inherit "/std/object";

void setup() {
   set_name( "shovel" );
   set_short( "ceremonial shovel" );
   add_adjective( "ceremonial" );
   set_long( "This is a small ceremonial shovel, carried by those "+
         "responsible for the care, upkeep and expansion of "+
         "dungeons.\n" );
} /* setup() */

void init() {
   if ( this_player()->query_creator() ) {
      add_action( "do_add", "add" );
      add_action( "do_remove", "remove" );
      add_action( "do_visit", "visit" );
   }
} /* init() */

int do_add( string words ) {
   int i, distance, level, *new_co_ord, *old_co_ord, *vector;
   string direc, file, terrain, type;
   object room, *things;
   if ( !words )
      return notify_fail( "Add what?\n" );
   if ( sscanf( words, "%s %s %s", type, file, direc ) != 3 )
      return notify_fail( "Syntax: add fixed|floating <file> <direction>\n" );
   i = member_array( direc, STD_ORDERS );
   if ( i == -1 )
      return notify_fail( capitalize( direc ) +" is not a valid "+
            "direction.\n" );
   room = environment( this_player() );
   if ( room->query_exit( direc ) )
      return notify_fail( "There is already an exit to the "+
            direc +".\n" );
   vector = STD_ORDERS[ i + 1 ];
   if ( file_size( file +".c" ) < 0 )
      return notify_fail( capitalize( file ) +" does not exist.\n" );
   file->force_load();
   if ( !find_object( file ) )
      return notify_fail( capitalize( file ) +" will not load.\n" );
   distance = (int)file->query_room_size() +
         (int)room->query_room_size();
   old_co_ord = (int *)room->query_co_ord();
   if ( sizeof( old_co_ord ) != 3 )
      return notify_fail( "Environment does not have coordinates.\n" );
   new_co_ord = allocate( 3 );
   for ( i = 0; i < 3; i++ )
      new_co_ord[ i ] = old_co_ord[ i ] - distance * vector[ i ];
   terrain = (string)room->query_property( "terrain name" );
   if ( !terrain )
      return notify_fail( "Environment is not in the terrain "+
            "handling system.\n" );
   switch( type ) {
      case "fixed" :
         i = (int)TERRAIN_MAP->add_fixed_location( terrain,
               file, new_co_ord );
         break;
      case "floating" :
         i = (int)TERRAIN_MAP->add_floating_location( terrain,
               file, new_co_ord, level );
         break;
      default :
         return notify_fail( "Type must be fixed or floating.\n" );
   }
   if ( !i )
      return notify_fail( "Failed to add "+ file +" ("+ type +
            ") to "+ terrain +" at "+ new_co_ord[ 0 ] +", "+
            new_co_ord[ 1 ] +", "+ new_co_ord[ 2 ] +".\n" );
   write( "Added "+ file +" ("+ type +") to "+ terrain +" at "+
         new_co_ord[ 0 ] +", "+ new_co_ord[ 1 ] +", "+
         new_co_ord[ 2 ] +".\n" );
   if ( type == "fixed" )
      file->dest_me();
   TERRAIN_MAP->clear_connections( terrain );
   TERRAIN_MAP->delete_cloned_location( terrain, file_name( room ) );
   things = all_inventory( room );
   things->move( "/room/void" );
   room->dest_me();
   room = (object)TERRAIN_MAP->find_location( terrain, old_co_ord );
   things->move( room );
   return 1;
} /* do_add() */

int do_remove() {
   write( "This function has not been written yet.\n" );
   return 1;
} /* do_remove() */

int do_visit( string words ) {
   int x, y, z;
   string terrain;
   object room;
   if ( !words )
      return notify_fail( "Visit where?\n" );
   if ( sscanf( words, "%s %d %d %d", terrain, x, y, z ) != 4 )
      return notify_fail( "Syntax: visit <terrain> <x co-ord> "+
            "<y co-ord> <z co-ord>\n" );
   room = (object)TERRAIN_MAP->find_location( terrain, ({ x, y, z }) );
   if ( !objectp( room ) )
      return notify_fail( "There is no location at ("+ x +","+ y +","+ z +
            ") in \""+ terrain +"\".\n" );
   this_player()->move_with_look( room, "There is a strange smell, like "+
         "freshly broken earth, and $N appear$s.", "With a squelch, "+
         "$N vanish$s." );
   return 1;
} /* do_visit() */
