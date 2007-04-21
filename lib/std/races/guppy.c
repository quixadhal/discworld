inherit "/std/races/fish_base";

void setup() {
   set_name( "guppy" );
   set_long( "Fish,  yes.  A fish.  Nice generic standard fish thing.\n" );
   set_height( 15 );
   set_weight( 40 );
   set_desc( "a beautiful looking guppy" );

   set_stats( ({ -2, 14, -4, -4, -6 }) );
} /* setup() */

/*
* Need to create a shadow to handle multipleing the armour class by
* an obscene value when underwater...
*/
void start_player( object thing ) {
   ::start_player( thing );
//   thing->add_effect( "/std/effects/other/fish_effect", ({ }) );
   thing->add_property( "gills", 1 );
   thing->tactics( "response dodge" );
} /* start_player() */

int lives_in_water() { return 1; }
