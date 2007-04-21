// Gototh, 01/03/99

inherit "/std/races/rodent_base";

void setup() {
   set_name( "bat" );
   set_long( "This is a small flying rodent." );
   set_desc( "a small, flying rodent" );
   set_height( 20 );
   set_weight( 40 );

   remove_bit( "torso" );
   remove_bit( "left front leg" );
   remove_bit( "right front leg" );
   remove_bit( "left rear leg" );
   remove_bit( "right rear leg" );

   add_bit( "torso", "torso", ({ 0, 20, 0,
      "heart", "liver", "left kidney", "right kidney", "spleen",
      "right lung", "left lung", "right wing", "left wing" }) );
   add_bit( "right wing", "wing", ({ "torso", 20, 0 }) );
   add_bit( "left wing", "wing", ({ "torso", 20, 0 }) );
   add_bit( "left leg", "leg", ({ 0, 5, 0 }) );
   add_bit( "right leg", "leg", ({ 0, 5, 0 }) );

   set_sight( ({ -20, 0, 80, 120 }) );
   set_stats( ({ 0, 14, -4, -4, -6 }) );
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

   number = thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number ) / 4;

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
