inherit "/std/races/lagomorpha_base";

void setup() {
   set_name( "hare" );
   set_long( "This is a hare with long ears.\n" );
   set_height( 50 );
   set_weight( 100 );
   set_desc( "a large and meaty hare" );

   set_stats( ({ -2, 8, -4, -4, -6 }) );
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
   number = sqrt( number ) / 6;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "bite" );

   thing->add_attack( "bite", 75, ({ 0, 3, 5 }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude defensive" );
} /* set_unarmed_attacks() */
