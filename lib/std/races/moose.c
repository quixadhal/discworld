inherit "/std/races/ruminant";

void setup() {
   mixed *new_head;

   set_name( "moose" );
   set_long( "The moose is a large woodland animal.\n" );
   set_height( 200 );
   set_weight( 7000 );
   set_desc( "a large, hairy quadruped reminding you very much of a moose" );

   new_head = query_bit( "head" );
   new_head[ 2 ] += ({ "left horn", "right horn" });
   remove_bit( "head" );
   add_bit( "head", "head", new_head[ 2 ] );
   add_bit( "left horn", "horn", ({ "head", 20, 0 }) );
   add_bit( "right horn", "horn", ({ "head", 20, 0 }) );
   add_bit( "tail", "tail", ({ 0, 15, 0 }) );

   inedible += ({"left horn", "right horn", "tail" });  
   unrottable += ({ "left horn", "right horn" });

   add_ac( "blunt", "blunt", 50 );
   add_ac( "pierce", "pierce", 20 );
   add_ac( "sharp", "sharp", 20 );

   set_stats( ({ 4, -2, -4, 8, -4 }) );
}  /* setup() */

void set_unarmed_attacks( object thing ) {
   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );
   thing->remove_attack( "standard" );

   thing->remove_attack( "chew" );
   thing->remove_attack( "hoof" );

   thing->add_attack( "chew", 20, ({ 40, 8, 10 }), 
      "sharp", "unarmed", 0 );
   thing->add_attack( "hoof", 50, ({ 60, 10, 40 }), 
      "blunt", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
