inherit "/std/races/bird_base";

void setup() {
   set_name( "raptor" );
   set_long( "A carnivorous bird (as a hawk or falcon) that feeds wholly on "
      "meat taken by hunting.\n" );
   set_height( 65 );
   set_weight( 200 );
   set_desc( "a sharp-eyed raptor" );

   remove_bit( "gizzard" );

   set_stats( ({ 0, 14, -4, 2, -6 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   level = 2 * level + random( level );

   thing->add_ac( "blunt", "blunt", level );
   thing->add_ac( "pierce", "pierce", level / 4 );
   thing->add_ac( "sharp", "sharp", level / 4 );
} /* new_set_level() */

void set_unarmed_attacks( object thing ) {
   int number;

   number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
   number = sqrt( number ) / 2;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "beak" );
   thing->remove_attack( "claws" );

   thing->add_attack( "beak", 50, ({ number / 2, 2, number / 2 }),
      "pierce", "unarmed", 0 );
   thing->add_attack( "claws", 75, ({ number, 5, number / 2 }),
      "sharp", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */