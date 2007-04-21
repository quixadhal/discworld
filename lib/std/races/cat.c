inherit "/std/races/carnivore";

void setup() {
   set_name( "cat" );
   set_long( "A carnivorous mammal (Felis catus) long domesticated and kept "+
         "by man as a pet or for catching rats and mice.\n" );
   set_height( 20 );
   set_weight( 100 );
   set_desc( "a furry cat, on the hunt for a mouse to play with" );

   set_stats( ({ 0, 10, 2, -2, 2 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   level = 1 + ( 2 * level + random( level ) ) / 10;

   thing->add_ac( "blunt", "blunt", level );
   thing->add_ac( "sharp", "sharp", 2 + level );
   thing->add_ac( "pierce", "pierce", 2 * level );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "bite" );
   thing->remove_attack( "claws" );

   thing->add_attack( "bite", 100, ({ number, 5, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "claws", 150, ({ number / 2, 7, number }),
      "sharp", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
