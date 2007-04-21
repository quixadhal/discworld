inherit "/std/races/reptile_base";

void setup() {
   set_name( "reptile" );
   set_long( "A normal reptile, wiv scales on.\n" );
   set_weight( 1000 );
   set_height( 20 );
   set_desc( "your standard reptile" );

   set_stats( ({ -2, 2, -4, -2, -6 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "chew" );
   thing->remove_attack( "tail" );

   thing->add_attack( "chew", 75, ({ 3 * number, number / 2, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "tail", 75, ({ number, 5, number }),
      "blunt", "unarmed", 0 );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude offensive" );
} /* set_unarmed_attacks() */
