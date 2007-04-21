/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dungeon_maker.c,v 1.2 1999/08/17 17:46:06 wodan Exp $
 * $Log: dungeon_maker.c,v $
 * Revision 1.2  1999/08/17 17:46:06  wodan
 * changed to add_command
 *
 * Revision 1.1  1998/01/06 05:06:10  ceres
 * Initial revision
 * 
*/
#include <dirs.h>
#include <terrain.h>

#define END 0
#define TWO 1
#define FOUR 2
#define ROOM 3

#define HALF 0
#define FULL 1

#define DIRS ({ "north", "northeast", "east", "southeast", \
      "south", "southwest", "west", "northwest" })
inherit "/std/object";

int max, size, *clear, *c_probs, *p_probs, *r_probs, *start;
string terrain_name, *passage_files, *room_files;
object easel;
mixed *passages, *room_entries;

void setup() {
   set_name( "box" );
   set_short( "green box" );
   add_adjective( "green" );
   set_long( "This is a green box.  It's quite featureless but, strangely "+
         "enough, something tells you it's a dungeon generator.\n" );
   reset_get();
   start = allocate( 3 );
   passages = ({ });
   room_entries = ({ });
} /* setup() */

void dest_me() {
   if ( easel )
      easel->dest_me();
   ::dest_me();
} /* dest_me() */

int query_max_locations() { return max; }
void set_max_locations( mixed arg ) { max = to_int( arg ); }

int *query_junction_clearance() { return clear; }
void set_junction_clearance( mixed *args ) {
   int i;
   clear = allocate( sizeof( args ) );
   for ( i = 0; i < sizeof( args ); i++ )
      clear[ i ] = to_int( args[ i ] );
} /* set_junction_clearance() */

int *query_continue_probabilities() { return c_probs; }
void set_continue_probabilities( mixed *args ) {
   int i;
   c_probs = allocate( sizeof( args ) );
   for ( i = 0; i < sizeof( args ); i++ )
      c_probs[ i ] = to_int( args[ i ] );
} /* set_continue_probabilities() */

int *query_passage_probabilities() { return p_probs; }
void set_passage_probabilities( mixed *args ) {
   int i;
   p_probs = allocate( sizeof( args ) );
   for ( i = 0; i < sizeof( args ); i++ )
      p_probs[ i ] = to_int( args[ i ] );
} /* set_passage_probabilities() */

int *query_room_probabilities() { return r_probs; }
void set_room_probabilities( mixed *args ) {
   int i;
   r_probs = allocate( sizeof( args ) );
   for ( i = 0; i < sizeof( args ); i++ )
      r_probs[ i ] = to_int( args[ i ] );
} /* set_room_probabilities() */

int *query_start_co_ord() { return start; }
void set_start_co_ord( mixed *args ) {
   int i;
   if ( sizeof( args ) != 3 )
      return;
   for ( i = 0; i < sizeof( args ); i++ )
      start[ i ] = to_int( args[ i ] );
} /* set_passage_probabilities() */

string query_terrain_name() { return terrain_name; }
void set_terrain_name( string word ) { terrain_name = word; }

string *query_passage_files() { return passage_files; }
void set_passage_files( string *words ) { passage_files = words; }

string *query_room_files() { return room_files; }
void set_room_files( string *words ) { room_files = words; }

object query_easel() { return easel; }

mixed *query_passages() { return passages; }

mixed *query_room_entries() { return room_entries; }

int do_generate(string);

void init() {
//  ::init();
   add_command( "generate", "<word'terrainname'>", (:do_generate($4[0]):));
} /* init() */

void mark_passage( int *co_ord ) {
   easel->add_shape( ({ ( co_ord[ 1 ] - start[ 1 ] ) / size,
         ( start[ 0 ] - co_ord[ 0 ] ) / size }), ({ "#" }) );
} /* mark_passage() */

void mark_room( int *co_ord, string room ) {
   int width;
   width = (int)room->query_room_size();
   easel->add_shape( ({ ( co_ord[ 1 ] - start[ 1 ] - width - size / 2 ) / size,
         ( start[ 0 ] - co_ord[ 0 ] - width - size / 2 ) / size }),
         (string *)easel->make_block( ( 2 * width ) / size + 2 ) );
} /* mark_room() */

int do_generate( string word ) {
   int i;
   string data, direc, *lines;
   mixed *parameters;
   if ( easel )
      return notify_fail( "The generator is working on "+
            terrain_name +".\n" );
   if ( file_size( word ) < 0 )
      return notify_fail( "File "+ word +" does not exist.\n" );
   lines = explode( read_file( word ), "\n" );
   for ( i = 0; i < sizeof( lines ); i++ ) {
      if ( lines[ i ] == "" )
         continue;
       if ( lines[ i ][ 0 .. 0 ] == "#" )
         continue;
      parameters = explode( lines[ i ], " " );
      if ( !stringp( parameters[ 0 ] ) ) {
         printf( "%s line %d: %O is not a string.\n", word, i,
               parameters[ 0 ] );
         continue;
      }
      if ( sizeof( parameters ) == 2 )
         call_other( this_object(), "set_"+ parameters[ 0 ],
               parameters[ 1 ] );
      else
         call_other( this_object(), "set_"+ parameters[ 0 ],
               parameters[ 1 .. sizeof( parameters ) - 1 ] );
      printf( "Set %s to %O.\n", parameters[ 0 ], call_other( this_object(),
            "query_"+ parameters[ 0 ] ) );
   }
   if ( file_size( RESTORE_PATH + terrain_name +".o" ) > 0 )
      return notify_fail( "Terrain "+ terrain_name +" already exists.\n" );
   easel = clone_object( "/obj/misc/easel" );
   set_long( "This is a green box.  It's quite featureless but, strangely "+
         "enough, something tells you it's a dungeon generator.\nIt seems "+
         "to be humming quietly but in a very powerful way.\n" );
   write( "Working on "+ terrain_name +".\n" );
   direc = DIRS[ random( 8 ) ];
   for ( i = 0; i < 4; i++ )
      passages += ({ ({ DIRS[ ( member_array( direc, DIRS ) + 2 * i ) % 8 ],
            passage_files[ random( sizeof( passage_files ) ) ],
            start, 0, 0, 1 + clear[ 0 ] + random( clear[ 1 ] ) }) });
   size = 2 * (int)passage_files[ 0 ]->query_room_size();
   TERRAIN_MAP->add_floating_location( terrain_name, passage_files[ 0 ],
         start, 0 );
   call_out( "process_passages", 2 );
   return 1;
} /* do_generate() */

void stop_working() {
   tell_room( environment(), "The box stops humming as it extrudes an "+
         "easel from a slot in one side.\n" );
   easel->add_shape( ({ 0, 0 }), ({ "@" }) );
   write_file( "/open/"+ terrain_name +".map",
         implode( (string *)easel->query_lines(), "\n" ) );
   easel->move( environment() );
   easel = 0;
   passages = ({ });
   room_entries = ({ });
   set_long( "This is a green box.  It's quite featureless but, strangely "+
         "enough, something tells you it's a dungeon generator.\n" );
} /* stop_working() */

int *shift_in( int *co_ord, string direc, string from, string to ) {
   int i, delta, *new_co_ord, *vector;
   delta = (int)from->query_room_size() + (int)to->query_room_size();
   new_co_ord = allocate( 3 );
   vector = STD_ORDERS[ member_array( direc, STD_ORDERS ) + 1 ];
   for ( i = 0; i < 3; i++ )
      new_co_ord[ i ] = co_ord[ i ] - delta * vector[ i ];
   return new_co_ord;
} /* shift_in() */

void process_passages() {
   int i, j, k, prob, straight, *co_ord, *new_co_ord;
   string direc, new_direc, type, new_type;
   mixed *last;
   if ( !sizeof( passages ) ) {
      tell_room( environment(), "No passages to process.\n" );
      return stop_working();
   }
   if ( max < 1 ) {
      tell_room( environment(), "Reached maximum.\n" );
      return stop_working();
   }
   max--;
   direc = passages[ 0 ][ 0 ];
   type = passages[ 0 ][ 1 ];
   co_ord = passages[ 0 ][ 2 ];
   last = passages[ 0 ][ 3 .. 4 ];
   straight = passages[ 0 ][ 5 ];
   passages = delete( passages, 0, 1 );
   call_out( "process_passages", 2 );
   for ( i = 0; i < 3; i++ ) {
      new_direc = DIRS[ ( member_array( direc, DIRS ) + 7 + i ) % 8 ];
      new_co_ord = shift_in( co_ord, new_direc, type, type );
      if ( sizeof( (mixed *)TERRAIN_MAP->
            member_floating_locations( new_co_ord ) ) ) {
         tell_room( environment(), "Passage already at "+ new_co_ord[ 0 ] +
               ", "+ new_co_ord[ 1 ] +", "+ new_co_ord[ 2 ] +"; ending "+
               "passage at "+ co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+
               co_ord[ 2 ] +".\n" );
         return;
      }
      for ( j = 0; j < sizeof( room_entries ); j++ ) {
         for ( k = 0; k < 3; k++ )
            if ( new_co_ord[ k ] != room_entries[ j ][ k ] )
               break;
         if ( k < 3 )
            continue;
         tell_room( environment(), "Room entry at "+ new_co_ord[ 0 ] +
               ", "+ new_co_ord[ 1 ] +", "+ new_co_ord[ 2 ] +"; "+
               "joining passage at "+ co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+
               co_ord[ 2 ] +".\n" );
         TERRAIN_MAP->add_floating_location( terrain_name, type, new_co_ord,
               0 );
         mark_passage( new_co_ord );
         room_entries = delete( room_entries, j, 1 );
         return;
      }
      if ( (int)easel->query_at( ({ ( new_co_ord[ 1 ] - start[ 1 ] ) / size,
            ( start[ 0 ] - new_co_ord[ 0 ] ) / size }) ) != 32 ) {
         tell_room( environment(), "Something at "+ new_co_ord[ 0 ] +
               ", "+ new_co_ord[ 1 ] +", "+ new_co_ord[ 2 ] +"; ending "+
               "passage at "+ co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+
               co_ord[ 2 ] +".\n" );
         return;
      }
   }
   prob = random( 100 );
   if ( prob < p_probs[ END ] ) {
      tell_room( environment(), "Passage going "+ direc +" ends at "+
            co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+ co_ord[ 2 ] +".\n" );
      return;
   }
   prob -= p_probs[ END ];
   if ( ( prob < p_probs[ TWO ] ) && !straight ) {
      if ( random( 100 ) >= c_probs[ HALF ] + c_probs[ FULL ] ) {
         tell_room( environment(), "Passage branching "+ direc +" and " );
         new_co_ord = shift_in( co_ord, direc, type, type );
         TERRAIN_MAP->add_floating_location( terrain_name, type,
               new_co_ord, 0 );
         passages += ({ ({ direc, type, new_co_ord, co_ord, last[ 0 ],
               clear[ 0 ] + random( clear[ 1 ] ) }) });
         mark_passage( new_co_ord );
         if ( random( c_probs[ HALF ] + c_probs[ FULL ] ) < c_probs[ HALF ] )
            direc = DIRS[ ( member_array( direc, DIRS ) + 1 +
                  6 * random( 2 ) ) % 8 ];
         else
            direc = DIRS[ ( member_array( direc, DIRS ) + 2 +
                  4 * random( 2 ) ) % 8 ];
         type = passage_files[ random( sizeof( passage_files ) ) ];
         new_co_ord = shift_in( co_ord, direc, type, type );
         TERRAIN_MAP->add_floating_location( terrain_name, type, new_co_ord,
               0 );
         passages += ({ ({ direc, type, new_co_ord, co_ord, last[ 0 ],
               clear[ 0 ] + random( clear[ 1 ] ) }) });
         mark_passage( new_co_ord );
         tell_room( environment(), direc +" from "+
               co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+ co_ord[ 2 ] +".\n" );
         return;
      }
      if ( random( c_probs[ HALF ] + c_probs[ FULL ] ) < c_probs[ HALF ] )
         new_direc = DIRS[ ( member_array( direc, DIRS ) + 1 ) % 8 ];
      else
         new_direc = DIRS[ ( member_array( direc, DIRS ) + 2 ) % 8 ];
      type = passage_files[ random( sizeof( passage_files ) ) ];
      new_co_ord = shift_in( co_ord, new_direc, type, type );
      TERRAIN_MAP->add_floating_location( terrain_name, type, new_co_ord, 0 );
      passages += ({ ({ new_direc, type, new_co_ord, co_ord, last[ 0 ],
            clear[ 0 ] + random( clear[ 1 ] ) }) });
      mark_passage( new_co_ord );
      tell_room( environment(), "Passage branching "+ new_direc +" and " );
      if ( random( c_probs[ HALF ] + c_probs[ FULL ] ) < c_probs[ HALF ] )
         new_direc = DIRS[ ( member_array( direc, DIRS ) + 7 ) % 8 ];
      else
         new_direc = DIRS[ ( member_array( direc, DIRS ) + 6 ) % 8 ];
      type = passage_files[ random( sizeof( passage_files ) ) ];
      new_co_ord = shift_in( co_ord, new_direc, type, type );
      TERRAIN_MAP->add_floating_location( terrain_name, type, new_co_ord, 0 );
      passages += ({ ({ new_direc, type, new_co_ord, co_ord, last[ 0 ],
            clear[ 0 ] + random( clear[ 1 ] ) }) });
      mark_passage( new_co_ord );
      tell_room( environment(), new_direc +" from "+
            co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+ co_ord[ 2 ] +".\n" );
      return;
   }
   prob -= p_probs[ TWO ];
   if ( ( prob < p_probs[ FOUR ] ) && !straight ) {
      tell_room( environment(), "Crossroads from "+ direc +" at "+
            co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+ co_ord[ 2 ] +".\n" );
      for ( i = 0; i < 3; i++ ) {
         new_direc = DIRS[ ( member_array( direc, DIRS ) + 6 + 2 * i ) % 8 ];
         type = passage_files[ random( sizeof( passage_files ) ) ];
         new_co_ord = shift_in( co_ord, new_direc, type, type );
         TERRAIN_MAP->add_floating_location( terrain_name, type, new_co_ord,
               0 );
         passages += ({ ({ new_direc, type, new_co_ord, co_ord, last[ 0 ],
               clear[ 0 ] + random( clear[ 1 ] ) }) });
         mark_passage( new_co_ord );
      }
      return;
   }
   prob -= p_probs[ FOUR ];
   if ( ( prob < p_probs[ ROOM ] ) && !straight ) {
      prob = random( 100 );
      new_type = room_files[ 0 ];
      for ( i = 1; i < sizeof( room_files ); i++ )
         if ( prob < r_probs[ FULL + i ] ) {
            new_type = room_files[ i ];
            break;
         } else
            prob -= r_probs[ FULL + i ];
      new_co_ord = shift_in( co_ord, direc, type, new_type );
      tell_room( environment(), "Room at "+ new_co_ord[ 0 ] +", "+
            new_co_ord[ 1 ] +", "+ new_co_ord[ 2 ] +".\n" );
      TERRAIN_MAP->add_floating_location( terrain_name, new_type,
            new_co_ord, 0 );
      mark_room( new_co_ord, new_type );
      mark_passage( co_ord );
      for ( i = 1; i < 8; i++ ) {
         new_direc = DIRS[ ( member_array( direc, DIRS ) + 4 + i ) % 8 ];
         type = passage_files[ random( sizeof( passage_files ) ) ];
         co_ord = shift_in( new_co_ord, new_direc, new_type, type );
         if ( i % 2 ) {
            if ( random( 100 ) < r_probs[ HALF ] ) {
               tell_room( environment(), "Passage leaving room to "+
                     new_direc +".\n" );
               TERRAIN_MAP->add_floating_location( terrain_name, type, co_ord,
                     0 );
               passages += ({ ({ new_direc, type, co_ord, new_co_ord,
                     last[ 0 ], clear[ 0 ] + random( clear[ 1 ] ) }) });
               mark_passage( co_ord );
            } else
               room_entries += ({ co_ord });
         } else {
            if ( random( 100 ) < r_probs[ FULL ] ) {
               tell_room( environment(), "Passage leaving room to "+
                     new_direc +".\n" );
               TERRAIN_MAP->add_floating_location( terrain_name, type, co_ord,
                     0 );
               passages += ({ ({ new_direc, type, co_ord, new_co_ord,
                     last[ 0 ], clear[ 0 ] + random( clear[ 1 ] ) }) });
               mark_passage( co_ord );
            } else
               room_entries += ({ co_ord });
         }
      }
      return;
   }
   prob = random( 100 );
   if ( ( prob < c_probs[ HALF ] ) && !straight ) {
      direc = DIRS[ ( member_array( direc, DIRS ) + 1 +
             6 * random( 2 ) ) % 8 ];
      straight = clear[ 0 ] + random( clear[ 1 ] );
   } else {
      prob -=  c_probs[ HALF ];
      if ( ( prob < c_probs[ FULL ] ) && !straight ) {
         direc = DIRS[ ( member_array( direc, DIRS ) + 2 +
               4 * random( 2 ) ) % 8 ];
         straight = clear[ 0 ] + random( clear[ 1 ] );
      }
   }
   if ( straight > 0 )
      straight--;
   new_co_ord = shift_in( co_ord, direc, type, type );
   tell_room( environment(), "Passage goes "+ direc +" from "+
         co_ord[ 0 ] +", "+ co_ord[ 1 ] +", "+ co_ord[ 2 ] +".\n" );
   TERRAIN_MAP->add_floating_location( terrain_name, type, new_co_ord, 0 );
   passages += ({ ({ direc, type, new_co_ord, co_ord, last[ 0 ],
         straight }) });
   mark_passage( new_co_ord );
} /* process_passages() */
