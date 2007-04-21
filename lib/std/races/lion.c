inherit "/std/races/carnivore";

void setup() {
   set_name( "lion" );
   set_long( "A carnivorous mammal.  This is the real thing though, not "
         "your domesticated house cat, but the king of its kind.  It leaps "
         "over the ground in huge bounding strides, power that other cats "
         "can only dream about.\n" );
   set_height( 150 );
   set_weight( 3500 );
   set_desc( "a rather large lion, bounding vigorously towards you" );

   add_bit( "bile duct", "duct", ({ "torso", 1, 0 }) );

   set_stats( ({ 6, 10, -4, 10, -4 }) );
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
   number = sqrt( number );

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "claws" );
   thing->remove_attack( "bite" );

   if( number < 10 )
      number = 10;
  
   thing->add_attack( "claws", 100, ({ number, 10, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "bite", 75, ({ 3 * number, number / 2, number }),
      "pierce", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
