inherit "/std/races/reptile_base";

void setup() {
   set_name( "crocodile" );
   set_long( "Any of several large voracious thick-skinned long-bodied "
      "aquatic reptiles (as of the genus Crocodylus) of tropical and "
      "subtropical waters.\n" );
   set_height( 30 );
   set_weight( 5000 );
   set_desc( "a crocodile, sharp of tooth and vicious of temper." );

   add_ac( "blunt", "blunt", 80 );
   add_ac( "pierce", "pierce", 80 );
   add_ac( "sharp", "sharp", 80 );

   set_stats( ({ 6, 4, -4, 8, -4 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "chew" );
   thing->remove_attack( "tail" );

   thing->add_attack( "chew", 75, ({ 3 * number, number, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "tail", 75, ({ number, number, number }),
      "blunt", "unarmed", 0 );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude offensive" );
} /* set_unarmed_attacks() */
