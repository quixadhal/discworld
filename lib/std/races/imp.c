inherit "/std/races/demonic";

void setup() {
   set_long( "This is one of the smaller denizens of the Underworld.\n" );
   set_name( "imp" );
   set_height( 50 );
   set_weight( 400 );
   set_desc( "a crafty-looking imp" );

   set_stats( ({ 2, 8, 2, 2, 2 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_skill_level( "other.health", level * 4 );
   thing->add_ac( "blunt", "blunt", level / 4 );
   thing->add_ac( "pierce", "pierce", level / 8 );
   thing->add_ac( "sharp", "sharp", level / 6 );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 6 + sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->add_attack( "hands", 75, ({ 2 * number, 8, number }),
      "blunt", "unarmed", 0 );
   thing->add_attack( "feet", 25, ({ 3 * number, 10, number }),
      "blunt", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
