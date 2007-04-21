/* Wolf race object. Maniac, June 1994.
 * Copied to dog race ob by Turrican for now.
 * Modified to use new attacks by Ceres. */

inherit "/std/races/carnivore";

void setup() {
   set_name( "dog" );
   set_long( "A street dog. I wouldn't stay too near, it could bite.\n" );
   set_desc( "a nasty dog" );
   set_height( 50 );
   set_weight( 700 );

   set_stats( ({ 4, 10, -2, 6, -4 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_ac( "blunt", "blunt", 40 );
   thing->add_ac( "sharp", "sharp", 20 );
   thing->add_ac( "pierce", "pierce", 10 );

   thing->add_skill_level( "other.perception", 200 );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "claws" );
   thing->remove_attack( "bite" );

   thing->add_attack( "claws", 100, ({ ( 3 * number ) / 2, 5, number } ),
      "sharp", "unarmed", 0 );
   thing->add_attack( "bite", 50, ({ 3 * number, 2, number }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
