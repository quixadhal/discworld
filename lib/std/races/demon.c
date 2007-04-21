inherit "/std/races/demonic";

void setup() {
   set_long( "This is one of the denizens of the Underworld.\n" );
   set_name( "demon" );
   set_height( 250 );
   set_weight( 5000 );
   set_desc( "a nasty-looking demon" );

   add_ac( "fire", "fire", 100 );

   set_stats( ({ 10, 0, -2, 10, 4 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_skill_level( "other.health", level * 2 );
   thing->add_ac( "blunt", "blunt", level / 2 );
   thing->add_ac( "pierce", "pierce", level / 4 );
   thing->add_ac( "sharp", "sharp", level / 3 );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 4 + sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "claws" );
   thing->remove_attack( "bite" );

   thing->add_attack( "claws", 75, ({ 2 * number, 8, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "bite", 25, ({ 3 * number, 10, number }),
      "pierce", "unarmed", 0 );
} /* set_unarmed_attacks() */
