inherit "/std/races/insect_base";

void setup() {
   set_desc( "a nasty cockroach, ugh" );
   set_name( "cockroach" );
   set_height( 5 );
   set_weight( 2 );

   add_ac( "blunt", "blunt", 40 );
   add_ac( "sharp", "sharp", 20 );
   add_ac( "pierce", "pierce", 10 );

   set_stats( ({ -6, 4, -4, -6, -6 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   int number;
  
   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "claws" );
   thing->remove_attack( "bite" );

   thing->add_attack( "claws", 100, ({ ( 3 * number ) / 2, 5, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "bite", 50, ({3 * number, 2, number }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude defensive" );
} /* set_unarmed_attacks() */
