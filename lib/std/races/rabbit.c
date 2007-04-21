inherit "/std/races/lagomorpha_base";

void setup() {
   set_name( "rabbit" );
   set_long( "A small furry and fluffy rabbit.\n" );
   set_height( 30 );
   set_weight( 25 );
   set_desc( "a cute and fluffy little bunny" );

   set_stats( ({ -3, 8, -4, -5, -6 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_ac( "blunt", "blunt", level );
   thing->add_ac( "sharp", "sharp", 2 + level );
   thing->add_ac( "pierce", "pierce", 2 * level );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number ) / 7;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "bite" );

   thing->add_attack( "bite", 75, ({ 1, 3, 4 }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude defensive" );
} /* set_unarmed_attacks() */
