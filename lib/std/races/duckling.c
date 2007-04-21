inherit "/std/races/bird_base";

void setup() {
   set_name( "duckling" );
   set_long( "Awwwwwwww it's SUCH a cute little duckling!\n" );
   set_weight( 10 );
   set_height( 15 );
   set_desc( "an cute ickle duckling" );

   add_ac( "feather_blow", "blunt", 10 );
   add_ac( "feather_slash", "sharp", 2 );
   add_ac( "feather_pierce", "pierce", 2 );

   set_stats( ({ 0, 14, -6, -4, -6 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   int number;
  
   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number / 4 ) + ( thing->query_weight() / 20 );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "beak" );

   thing->add_attack( "beak", 50, ({ number / 2, 2, number / 2 }),
      "pierce", "unarmed", 0 );
   thing->add_attack( "feet", 75, ({ number, 5, number / 2 }),
      "blunt", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */

int lives_in_water() { return 1; }
