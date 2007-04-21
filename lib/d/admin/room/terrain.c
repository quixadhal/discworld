#include "path.h"
#include <terrain.h>

#define COLS (int)this_player()->query_cols()


inherit "/std/room/basic_room";

int do_help();
int do_terrain(string name);
int do_list(string what);
int do_member(string type, string params);
int do_clear();
int do_add(int floating, string file, int x1, int y1, int z1,
	   int x2, int y2, int z2, int level);
int do_remove(int floating, string file, int x1, int y1, int z1,
	   int x2, int y2, int z2);

string terrain_name;
object generator;

void setup() {
   set_short( "terrain-maker room" );
   add_property( "determinate", "the " );
   set_light( 100 );
   set_long( "This room is quite bare, save a notice that has been "
         "taped on the door but its walls have been painted with "
         "scenes of deserts and forests, oceans and mountains, caves and "
         "swamps, and all manner of other types of terrain.\n"
         "Use \"help room\" to list the room's commands.\n" );
   add_exit( "northwest", ROOM +"development", "door" );
   add_sign("The large notice has furled corners and has been taped "
           "and re-taped to the door.\n",
           "Due to the incomprehensible help in this room, this \n"
           "notice was put up to help with the confusion.\n"
           "\n"
           "1.  To get information on a certain terrain, you\n"
           "    need to select that terrain by typing \n"
           "    \"terrain <terrain_name>\".\n"
           "\n"
           "2.  To remove floating or fixed locations within the terrain,\n"
           "    type \"list floating\" or \"list fixed\".\n"
           "\n"
           "    remove floating <path of terrain> <all 6 co ordinates> <level>\n"
           "\n"
           "    remove fixed <path of room>\n"
           "\n"
           "3.  To add a floating location, same as above, only use add.\n"
           "    For both removing and adding, no commas and no .c at the end\n"
           "    of path names.  When removing/adding fixed locations, all\n"
           "    locations must be done one by one.\n", 0, "notice");
} /* setup() */

void reset() {
   if ( !generator ) {
      generator = clone_object( "/obj/handlers/terrain_things/dungeon_maker" );
      generator->move( this_object() );
   }
} /* reset() */

void init() {
  ::init();
  add_command("help", "room", (: do_help() :) );
  add_command("terrain", "<string>", (: do_terrain($4[0]) :) );
  add_command("list", "fixed", (: do_list("fixed") :) );
  add_command("list", "floating", (: do_list("floating") :) );
  add_command("member", "", (: do_member(0,0) :) );
  add_command("member", "fixed <string>",
    (: do_member("fixed", $4[0]) :) );
  add_command("member", "floating <string>",
    (: do_member("floating", $4[0]) :) );
  add_command("add",
    "fixed <word'file'> <number'x'> <number'y'> <number'z'>",
    (: do_add(0, $4[0], $4[1], $4[2], $4[3], 0, 0, 0, 0) :) );
  add_command("add",
    "floating <word'file'> <number'x1'> <number'y1'> <number'z1'> "
      "<number'x2'> <number'y2'> <number'z2'> <number'level'>",
    (: do_add(1, $4[0], $4[1], $4[2], $4[3],
                        $4[4], $4[5], $4[6],
                 $4[7]) :) );
  add_command("add",
    "floating <word'file'> <number'x'> <number'y'> <number'z'> "
      "<number'level'>",
    (: do_add(2, $4[0], $4[1], $4[2], $4[3],
                        0, 0, 0,
                 $4[4]) :) );
  add_command("remove",
    "fixed <word'file'>",
    (: do_remove(0, $4[0], 0, 0, 0, 0, 0, 0) :) );
  add_command("remove",
    "floating <word'file'> <number'x1'> <number'y1'> <number'z1'> "
      "<number'x2'> <number'y2'> <number'z2'>",
    (: do_remove(1, $4[0], $4[1], $4[2], $4[3],
		 $4[4], $4[5], $4[6]) :) );
  add_command("remove",
    "floating <word'file'> <number'x'> <number'y'> <number'z'>",
    (: do_remove(2, $4[0], $4[1], $4[2], $4[3],
		 0, 0, 0 ) :) );
  add_command("clear", "", (: do_clear() :) );
} /* init() */

int do_help () {
  write( "Available commands are:\n\n"+
      "     terrain  -  set terrain name\n"+
      "     list     -  list locations in current terrain\n"+
      "     member   -  find locations matching coordinates\n"+
      "     add      -  add a new location\n"+
      "     remove   -  remove an old location\n"+
      "     clear    -  clear all clones and connections\n" );
  return 1;
} /* do_help() */

int do_terrain( string name ) {
  if ( !name ) {
    if ( stringp( terrain_name ) )
      write( "Terrain name is "+ terrain_name +".\n" );
    else
      write( "There is no terrain name set.\n" );
    return 1;
  }
  write( "Terrain name set to "+ name +".\n" );
  terrain_name = name;
  return 1;
} /* do_terrain() */

int do_list( string word ) {
  int i, size;
  string list;
  mapping locations;
  mixed indeces, values;
  if ( ( word != "fixed" ) && ( word != "floating" ) ) {
    write( "List what? Options are \"fixed\" and \"floating\".\n" );
    return 1;
  }
  if ( !terrain_name ) {
    write( "There is no current terrain name.\n" );
    return 1;
  }
  if ( !( (int)TERRAIN_MAP->get_data_file( terrain_name ) ) ) {
    write( "There are no locations in the terrain "+ terrain_name +".\n" );
    return 1;
  }
  if ( word == "fixed" ) {
    locations = (mapping)TERRAIN_MAP->query_fixed_locations( terrain_name );
    size = m_sizeof( locations );
    indeces = m_indices( locations );
    values = m_values( locations );
    if ( !size ) {
      write( "There are no fixed locations in the terrain "+ terrain_name
                +".\n" );
      return 1;
    }
    list = "The terrain "+ terrain_name +" has the following fixed location"+
              ( size == 1 ? "" : "s" ) +":\n";
    for ( i = 0; i < size; i++ )
      list += sprintf( "% 5d - %-=*s\n", i + 1, COLS - 9, indeces[ i ] +
          " at ("+ values[ i ][ 0 ] +", "+ values[ i ][ 1 ] +", "+
          values[ i ][ 2 ] +")" );
    this_player()->more_string( list, "Fixed" );
    return 1;
  }
  locations = (mapping)TERRAIN_MAP->query_floating_locations( terrain_name );
  size = sizeof( locations );
  if ( !size ) {
    write( "There are no floating locations in the terrain "+ terrain_name
              +".\n" );
    return 1;
  }
  list = "The terrain "+ terrain_name +" has the following floating "+
      "location"+ ( size == 1 ? "" : "s" ) +":\n";
  for ( i = 0; i < size; i++ )
    if ( sizeof( locations[ i ][ 1 ] ) == 3 )
      list += sprintf( "% 5d - %-=*s\n", i + 1, COLS - 9, locations[ i ][ 0 ] +
          " at ("+ locations[ i ][ 1 ][ 0 ] +", "+ locations[ i ][ 1 ][ 1 ] +
          ", "+ locations[ i ][ 1 ][ 2 ] +"), level "+ locations[ i ][ 2 ] );
    else
      list += sprintf( "% 5d - %-=*s\n", i + 1, COLS - 9, locations[ i ][ 0 ] +
          " from ("+ locations[ i ][ 1 ][ 0 ] +", "+ locations[ i ][ 1 ][ 1 ] +
          ", "+ locations[ i ][ 1 ][ 2 ] +") to ("+ locations[ i ][ 1 ][ 3 ] +
          ", "+ locations[ i ][ 1 ][ 4 ] +", "+ locations[ i ][ 1 ][ 5 ] +
          "), level "+ locations[ i ][ 2 ] );
  this_player()->more_string( list, "Floating" );
  return 1;
} /* do_list() */

int do_member( string type, string parameters ) {
  int i, size, *co_ords;
  string list, location;
  mixed *locations;
  if (!type || !parameters) {
    write( "Syntax: member <fixed|floating> <coord1> ...\n" );
    return 1;
  }
  if ( ( type != "fixed" ) && ( type != "floating" ) ) {
    write( "Syntax: member <fixed|floating> ...\n" );
    return 1;
  }
  if ( !terrain_name ) {
    write( "There is no current terrain name.\n" );
    return 1;
  }
  co_ords = allocate( 3 );
  if ( type == "fixed" ) {
    if ( sscanf( parameters, "%d %d %d", co_ords[ 0 ], co_ords[ 1 ],
        co_ords[ 2 ] ) != 3 ) {
      write( "Syntax: member fixed <coord1> <coord2> <coord3>\n" );
      return 1;
    }
    TERRAIN_MAP->get_data_file( terrain_name );
    location = (string)TERRAIN_MAP->member_fixed_locations( co_ords );
    if ( !location ) {
      write( "There is no fixed location in the terrain "+ terrain_name +
          " at ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +", "+ co_ords[ 2 ] +
          ").\n" );
      return 1;
    }
    write( "The terrain "+ terrain_name +" has "+ location +" as a fixed "+
        "location at ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +", "+ co_ords[ 2 ] +
        ").\n" );
    return 1;
  }
  if ( sscanf( parameters, "%d %d %d", co_ords[ 0 ], co_ords[ 1 ], co_ords[ 2 ] )
      != 3 ) {
    write( "Syntax: member floating <coord1> <coord2> <coord3>\n" );
    return 1;
  }
  TERRAIN_MAP->get_data_file( terrain_name );
  locations = (mixed *)TERRAIN_MAP->member_floating_locations( co_ords );
  size = sizeof( locations ) / 2;
  if ( !size ) {
    write( "There are no floating locations in the terrain "+ terrain_name +
        " at ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +", "+ co_ords[ 2 ] +
        ").\n" );
    return 1;
  }
  list = "There "+ ( size == 1 ? "is one floating location" : "are "+
      query_num( size ) +" floating locations " ) +" in the terrain "+
      terrain_name +" at ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +", "+
      co_ords[ 2 ] +"):\n";
  for ( i = 0; i < size; i++ )
    list += sprintf( "% 5d - %-=*s\n", i + 1, COLS - 9, locations[ 2 * i ] +
        ", level "+ locations[ 2 * i + 1 ] );
  this_player()->more_string( list, "Member" );
  return 1;
} /* do_member() */

int do_add(int floating,
	   string file, int x1, int y1, int z1,
	   int x2, int y2, int z2, int level)
{
  int *co_ords;

  if ( !terrain_name ) {
    write( "There is no current terrain name.\n" );
    return 1;
  }
  if (floating == 0 || floating == 2) {
    co_ords = allocate(3);
    co_ords[0] = x1;
    co_ords[1] = y1;
    co_ords[2] = z1;
  } else {
    co_ords = allocate(6);
    co_ords[0] = x1;
    co_ords[1] = y1;
    co_ords[2] = z1;
    co_ords[3] = x2;
    co_ords[4] = y2;
    co_ords[5] = z2;
  }
  if (floating == 0) {
    if ( !TERRAIN_MAP->add_fixed_location( terrain_name, file, co_ords ) ) {
      write( "The fixed location "+ file +" in the terrain "+ terrain_name
                +" already exists.\n" );
      return 1;
    }
    write( "The fixed location "+ file +" has been added to the terrain "+
        terrain_name +" at coordinates ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +
        ", "+ co_ords[ 2 ] +").\n" );
    return 1;
  }
  TERRAIN_MAP->add_floating_location( terrain_name, file, co_ords, level );
  if ( sizeof( co_ords ) == 6 )
    write( "A floating location using "+ file +" has been added to the "+
        "terrain "+ terrain_name +" from coordinates ("+ co_ords[ 0 ] +", "+
        co_ords[ 1 ] +", "+ co_ords[ 2 ] +") to coordinates ("+ co_ords[ 3 ] +
        ", "+ co_ords[ 4 ] +", "+ co_ords[ 5 ] +") at level "+ level +".\n" );
  else
    write( "A floating location using "+ file +" has been added to the "+
        "terrain "+ terrain_name +" at coordinates ("+ co_ords[ 0 ] +", "+
        co_ords[ 1 ] +", "+ co_ords[ 2 ] +") and level "+ level +".\n" );
  return 1;
} /* do_add() */

int do_remove(int floating,
	   string file, int x1, int y1, int z1,
	   int x2, int y2, int z2)
{
  int *co_ords;
  if ( !terrain_name ) {
    write( "There is no current terrain name.\n" );
    return 1;
  }
  if (floating == 2) {
    co_ords = allocate(3);
    co_ords[0] = x1;
    co_ords[1] = y1;
    co_ords[2] = z1;
  } else if (floating == 1) {
    co_ords = allocate(6);
    co_ords[0] = x1;
    co_ords[1] = y1;
    co_ords[2] = z1;
    co_ords[3] = x2;
    co_ords[4] = y2;
    co_ords[5] = z2;
  }
  if ( floating == 0 ) {
    if ( !TERRAIN_MAP->delete_fixed_location( terrain_name, file ) ) {
      write( "There is no fixed location "+ file +" in the terrain "+
          terrain_name +" to remove.\n" );
      return 1;
    }
    write( "The fixed location "+ file +" has been removed from the terrain "+
        terrain_name +".\n" );
    return 1;
  }
  if ( !TERRAIN_MAP->delete_floating_location( terrain_name, file, co_ords ) ) {
    if ( sizeof( co_ords ) == 6 )
      write( "There is no floating location using "+ file +" in the terrain "+
          terrain_name +" from coordinates ("+ co_ords[ 0 ] +", "+
          co_ords[ 1 ] +", "+ co_ords[ 2 ] +") to coordinates ("+ co_ords[ 3 ] +
          ", "+ co_ords[ 4 ] +", "+ co_ords[ 5 ] +") to remove.\n" );
    else
      write( "There is no floating location using "+ file +" in the terrain "+
          terrain_name +" at coordinates ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +
          ", "+ co_ords[ 2 ] +") to remove.\n" );
    return 1;
  }
  if ( sizeof( co_ords ) == 6 )
    write( "The floating location using "+ file +" in the terrain "+
        terrain_name +" from coordinates ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +
        ", "+ co_ords[ 2 ] +") to coordinates ("+ co_ords[ 3 ] +", "+
        co_ords[ 4 ] +", "+ co_ords[ 5 ] +") has been removed.\n" );
  else
    write( "The floating location using "+ file +" in the terrain "+
      terrain_name +" at coordinates ("+ co_ords[ 0 ] +", "+ co_ords[ 1 ] +
      ", "+ co_ords[ 2 ] +") has been removed.\n" );
  return 1;
} /* do_remove() */

int do_clear() {
  if ( !terrain_name ) {
    write( "There is no current terrain name.\n" );
    return 1;
  }
  TERRAIN_MAP->clear_cloned_locations( terrain_name );
  TERRAIN_MAP->clear_connections( terrain_name );
  write( "Clones and connections cleared for the terrain "+ terrain_name +
      ".\n" );
  return 1;
} /* do_clear() */
