inherit "/std/races/bird_base";

void setup() {
   set_name( "penguin" );
   set_long( "Penguins are always rather formal.\n" );
   set_weight( 600 );

   remove_bit( "gizzard" );

   set_stats( ({ 2, 8, -4, -4, -6 }) );
} /* setup() */

string query_desc( object ob ) {
   return "Penguins are always rather formally dressed.\n";
} /* query_desc() */

