inherit "/std/races/rodent_base";

void setup() {
   set_name( "mouse" );
   set_long( "Any of numerous small rodents (as of the genus Mus) with "
      "pointed snout, rather small ears, elongated body and slender "
      "tail.\n" );
   set_height( 10 );
   set_weight( 5 );
   set_desc( "a little mouse, scurrying about for food." );

   set_stats( ({ -4, 8, -4, -4, -6 }) );
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
   thing->remove_attack( "claws" );

   thing->add_attack( "bite", 75, ({ 0, 3, number }),
      "pierce", "unarmed", 0 );
   thing->add_attack( "claws", 50, ({ 0, 5, number / 2 }),
      "sharp", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
