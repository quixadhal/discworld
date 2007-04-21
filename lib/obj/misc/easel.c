#define DIRS ([ \
      "north" : ({ ({ 0, -1 }), ({ "|" }) }), \
      "northeast" : ({ ({ 1, -1 }), ({ "/" }) }), \
      "east" : ({ ({ 1, 0 }), ({ "-" }) }), \
      "southeast" : ({ ({ 1, 1 }), ({ "\\" }) }), \
      "south" : ({ ({ 0, 1 }), ({ "|" }) }), \
      "southwest" : ({ ({ -1, 1 }), ({ "/" }) }), \
      "west" : ({ ({ -1, 0 }), ({ "-" }) }), \
      "northwest" : ({ ({ -1, -1 }), ({ "\\" }) }), \
   ])
#define PRIMES ({ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 })

inherit "/std/object";

int *origin;
string *lines;

void setup() {
   set_name( "easel" );
   set_short( "small easel" );
   add_adjective( "small" );
   set_main_plural( "small easels" );
   set_value( 800 );
   origin = ({ 0, 0 });
   lines = ({ });
} /* setup() */

int *query_origin() { return origin; }

string *query_lines() { return lines; }

string long( string word, int dark ) {
   int i, max, min, width;
   string part;
   if ( !sizeof( lines ) )
      return "The easel is blank.\n";
   width = ( (int)this_player()->query_cols() - 2 ) / 2;
   min = origin[ 0 ] - width;
   max = origin[ 0 ] + width;
   if ( min < 0 ) {
      max -= min;
      min = 0;
   }
   if ( max >= strlen( lines[ 0 ] ) )
      max = strlen( lines[ 0 ] ) - 1;
   part = "";
   for ( i = 0; i < sizeof( lines ); i++ )
      part += " "+ lines[ i ][ min .. max ] +"\n";
   return part;
} /* long() */

int query_at( int *place ) {
   if ( !sizeof( lines ) )
      return 32;
   if ( ( origin[ 0 ] + place[ 0 ] ) < 0 )
      return 32;
   if ( ( origin[ 0 ] + place[ 0 ] ) >= strlen( lines[ 0 ] ) )
      return 32;
   if ( ( origin[ 1 ] + place[ 1 ] ) < 0 )
      return 32;
   if ( ( origin[ 1 ] + place[ 1 ] ) >= sizeof( lines ) )
      return 32;
   return lines[ origin[ 1 ] + place[ 1 ] ][ origin[ 0 ] + place[ 0 ] ];
} /* query_at() */

void add_shape( int *start, string *pieces ) {
   int i, number, max, width;
   string gap, front, back;
   for ( i = 0; i < sizeof( pieces ); i++ )
      if ( strlen( pieces[ i ] ) > max )
         max = strlen( pieces[ i ] );
   if ( sizeof( lines ) )
       width = strlen( lines[ 0 ] );
   else
      lines = ({ "" });
   if ( ( origin[ 0 ] + start[ 0 ] ) < 0 ) {
      gap = "";
      for ( i = origin[ 0 ] + start[ 0 ]; i < 0; i++ )
         gap += " ";
      for ( i = 0; i < sizeof( lines ); i++ )
         lines[ i ] = gap + lines[ i ];
      origin[ 0 ] = -start[ 0 ];
   }
   if ( ( origin[ 0 ] + start[ 0 ] + max ) > width ) {
      gap = "";
      for ( i = width; i < ( origin[ 0 ] + start[ 0 ] + max ); i++ )
         gap += " ";
      for ( i = 0; i < sizeof( lines ); i++ )
         lines[ i ] += gap;
   }
   max = sizeof( pieces );
   width = sizeof( lines );
   if ( ( origin[ 1 ] + start[ 1 ] ) < 0 ) {
      gap = "";
      for ( i = 0; i < strlen( lines[ 0 ] ); i++ )
         gap += " ";
      for ( i = origin[ 1 ] + start[ 1 ]; i < 0; i++ )
         lines = ({ gap }) + lines;
      origin[ 1 ] = -start[ 1 ];
   }
   if ( ( origin[ 1 ] + start[ 1 ] + max ) > width ) {
      gap = "";
      for ( i = 0; i < strlen( lines[ 0 ] ); i++ )
         gap += " ";
      for ( i = width; i < ( origin[ 1 ] + start[ 1 ] + max ); i++ )
         lines += ({ gap });
   }
   for ( i = origin[ 1 ] + start[ 1 ]; i < ( origin[ 1 ] + start[ 1 ] + max );
         i++ ) {
      number = origin[ 0 ] + start[ 0 ];
      if ( number > 0 )
         front = lines[ i ][ 0 .. number - 1 ];
      else
         front = "";
      number = origin[ 0 ] + start[ 0 ] +
            strlen( pieces[ i - origin[ 1 ] - start[ 1 ] ] );
      if ( number < strlen( lines[ i ] ) )
         back = lines[ i ][ number .. strlen( lines[ i ] ) - 1 ];
      else
         back = "";
      lines[ i ] = front + pieces[ i - origin[ 1 ] - start[ 1 ] ] + back;
   }
} /* add_shape() */

string *make_block( int size, int home ) {
   int i, j;
   string start, end, *blocks;
   if ( size == 1 )
      if ( home )
         return ({ "@" });
      else
         return ({ "#" });
   blocks = allocate( size );
   if ( home )
      blocks[ 0 ] = "@";
   else
      blocks[ 0 ] = "+";
   for ( i = 2; i < size; i++ )
      blocks[ 0 ] += "-";
   if ( home )
      blocks[ 0 ] += "@";
   else
      blocks[ 0 ] += "+";
   for ( i = 2; i < size; i++ ) {
      blocks[ i - 1 ] = "|";
      for ( j = 2; j < size; j++ )
         blocks[ i - 1 ] += ".";
      blocks[ i - 1 ] += "|";
   }
   if ( home )
      blocks[ size - 1 ] = "@";
   else
      blocks[ size - 1 ] = "+";
      for ( i = 2; i < size; i++ )
      blocks[ size - 1 ] += "-";
   if ( home )
      blocks[ size - 1 ] += "@";
   else
      blocks[ size - 1 ] += "+";
   return blocks;
} /* make_block() */

void make_map( mixed *locations, mapping connections ) {
   int i, j, k, x, y, *sizes;
   string *places;
   mapping centres;
   sizes = ({ });
   places = ({ });
   centres = ([ ]);
   for ( i = 0; i < sizeof( locations ); i+= 2 ) {
      sizes += ({ locations[ i + 1 ] });
      places += ({ locations[ i ] });
      reset_eval_cost();
   }
   for ( i = 0; i < sizeof( PRIMES ); i++ ) {
      x = 0;
      y = 0;
      for ( j = 0; j < sizeof( sizes ); j++ ) {
         if ( PRIMES[ i ] > sizes[ j ] ) {
            x = 1;
            break;
         }
         if ( sizes[ j ] % PRIMES[ i ] )
            y = 1;
      }
      if ( x )
         break;
      if ( !y ) {
         for ( j = 0; j < sizeof( sizes ); j++ )
            sizes[ j ] /= PRIMES[ i ];
         i--;
      }
   }
   reset_eval_cost();
   centres[ places[ 0 ] ] = ({ 0, 0 });
   for ( i = 0; i < sizeof( places ); i++ ) {
      tell_object( find_player( "deutha" ), "On "+ i +", "+ places[ i ] +
            ", at "+ centres[ places[ i ] ][ 0 ] +", "+
            centres[ places[ i ] ][ 1 ] +" for "+ sizes[ i ] +"\n" );
      add_shape( ({ centres[ places[ i ] ][ 0 ] - sizes[ i ],
            centres[ places[ i ] ][ 1 ] - sizes[ i ] }),
            make_block( 2 * sizes[ i ] - 1, !i ) );
      if ( undefinedp( connections[ places[ i ] ] ) )
            continue;
      for ( j = 0; j < sizeof( connections[ places[ i ] ] ); j += 2 ) {
         reset_eval_cost();
         add_shape( ({ centres[ places[ i ] ][ 0 ] -1 + sizes[ i ] *
               DIRS[ connections[ places[ i ] ][ j ] ][ 0 ][ 0 ],
               centres[ places[ i ] ][ 1 ] -1 + sizes[ i ] *
               DIRS[ connections[ places[ i ] ][ j ] ][ 0 ][ 1 ] }),
               DIRS[ connections[ places[ i ] ][ j ] ][ 1 ] );
         k = member_array( connections[ places[ i ] ][ j + 1 ], places );
         if ( k < 0 )
            continue;
         x = centres[ places[ i ] ][ 0 ] +
               DIRS[ connections[ places[ i ] ][ j ] ][ 0 ][ 0 ] *
               ( sizes[ i ] + sizes[ k ] );
         y = centres[ places[ i ] ][ 1 ] +
               DIRS[ connections[ places[ i ] ][ j ] ][ 0 ][ 1 ] *
               ( sizes[ i ] + sizes[ k ] );
         centres[ places[ k ] ] = ({ x, y });
         tell_object( find_player( "deutha" ), "Put "+
               connections[ places[ i ] ][ j + 1 ] +" at "+
               x +", "+ y +"\n" );
         reset_eval_cost();
      }
   }
} /* make_map() */
