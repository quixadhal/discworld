/* Bird race object.  Made by Ceres and stolen from the vulture object.
Modified by Dragonkin for new race system (this race shouldn't exist btw) */

inherit "/std/races/bird_base";

void setup() {
   set_name( "bird" );
   set_long( "A general, unspecified bird.\n" );
   set_height( 15 );
   set_weight( 60 );

   set_stats( ({ 0, 14, -4, -4, -6 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int weight;
   object thing;

   thing = args[ 1 ];
   if ( !thing )
     return;

   weight = (thing->query_weight() / 20);
  
   thing->add_ac( "blunt", "blunt", weight / 2 );
   thing->add_ac( "pierce", "pierce", weight );
   thing->add_ac( "sharp", "sharp", weight );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;
  
   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number / 4 ) + ( thing->query_weight() / 20 );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "beak" );
   thing->remove_attack( "claws" );

   thing->add_attack( "beak", 50, ({ number / 2, 2, number / 2 }),
      "pierce", "unarmed", 0 );
   thing->add_attack( "claws", 75, ({ number, 5, number / 2 }),
      "sharp", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
