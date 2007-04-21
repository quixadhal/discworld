inherit "/std/races/amphibian";

void setup() {
   set_name( "frog" );
   set_long( "Frogs are amphibians" );
   set_height(10);
   set_weight(10);
   set_desc( "an innocent little froggie" );

   add_ac( "blunt", "blunt", 5 );
   add_ac( "pierce", "pierce", 4 );
   add_ac( "sharp", "sharp", 3 );

   add_attack( "bite", 50, ({ 10, 2, 6 }) );

   set_sight( ({ 5, 20, 200, 300 }) );
   set_stats( ({ -4, 10, -4, -4, -6 }) );
} /* setup() */

void start_player( object thing ) {
   ::start_player( thing );
   thing->tactics( "response dodge" );
} /* start_player() */
