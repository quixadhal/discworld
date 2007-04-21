/* Race object for small lights */

inherit "/std/races/base";

void setup() {
   set_name( "light" );
   set_long( "A small sphere of magical blue light.\n" );
   set_weight( 1 );
   set_height( 1 );
   set_desc( "a small ball of blue light hovering in the air." );
   
   /* throw away the human bits and replace totally */
   bits = ({ });
   inedible = ({ });
   unrottable = ({ });

   add_ac( "fire", "fire", 1000 );
   add_ac( "sound", "sound", 1000 );
   add_ac( "acid", "acid", 1000 );
   add_ac( "gas", "gas", 1000 );
   add_ac( "mental", "mental", 100 );

   set_stats( ({ 0, 14, -6, -6, -6 }) );
}  /* setup() */

void start_player( object thing ) {
   ::start_player( thing );
   thing->tactics( "response dodge" );
} /* start_player() */

int lives_in_water() { return 1; }
