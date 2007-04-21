/* Vulture race object.  Made by Presto.  Most of this stuff was nicked
from the chicken object, except for the parts nicked from the caterpillar,
and except for the parts Deutha put in.  Well, one or two of these lines are
mine, anyway! :-b */

inherit "/std/races/bird_base";

void setup() {
   set_name( "vulture" );
   set_long( "A largish, rather scruffy-looking bird with an ugly, "
      "featherless head and a big crooked beak.\n" );
   set_desc( "a somewhat evil-looking old vulture\n" );
   set_height( 40 );
   set_weight( 200 );

   set_stats( ({ 0, 14, -4, 2, -6 }) );
} /* setup() */

void new_set_level( mixed *args ) {
   int level;
   object thing;

   if( sizeof(args) < 2 || !args[1] )
      return;

   level = args[ 0 ];
   thing = args[ 1 ];

   thing->add_ac( "blunt", "blunt", level / 2 );
   thing->add_ac( "pierce", "pierce", level );
   thing->add_ac( "sharp", "sharp", level );
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
