/* Dragonkin, Decembar 1997 */

inherit "/std/races/carnivore";

void setup() {
   set_name( "panda" );
   set_desc( "a black and white fluffy panda" );
   set_height( 200 );
   set_weight( 8000 );

   set_stats( ({ 6, 4, -2, 8, -4 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   level = 2 + ( 2 * level + random( level ) ) / 8;

   thing->add_ac( "blunt", "blunt", 20 + level / 3 );
   thing->add_ac( "sharp", "sharp", 50 + level );
   thing->add_ac( "pierce", "pierce", 60 + 2 * level );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = 4 + sqrt( number ) / 2;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "claws" );
   thing->remove_attack( "bite" );

   thing->add_attack( "claws", 100, ({ 2 * number, 10, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "bite", 50, ({ 3 * number, 8, number }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
   thing->tactics( "attitude offensive" );
} /* set_unarmed_attacks() */
