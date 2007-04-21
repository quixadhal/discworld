#define POISON_EFFECT "/std/effects/poisons/spider_venom"

inherit "/std/races/arachnid";

void setup() {
   set_name( "spider" );
   set_long( "A small, poisonous, hairy spider.\n" );
   set_desc( "a hairy arachnid" );
   set_height( 5 );
   set_weight( 2 );

   add_bit( "silk gland", "gland", ({ "abdomen", 10,
      ({"/std/bit", 1, 400}) }) );
   add_bit( "poison gland", "gland", ({ "cephalothorax", 5,
      ({"/std/bit", 1, 400}) }) );

   inedible += ({ "poison gland" });

   set_stats( ({ -6, 4, -6, -6, -6 }) );
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
   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );
   thing->remove_attack( "standard" );

   thing->remove_attack( "bite" );
   thing->remove_attack( "chew" );

   thing->add_attack( "bite", 80, ({ 10, 2, 5 }),
      "pierce", "unarmed", ({ "poison", this_object() }) );
   thing->add_attack( "chew", 100, ({ 2, 2, 10 }),
      "sharp", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */

void poison( int damage, object him, object me, string a_type,
             string a_name ) {
   tell_room( me->one_short() + " visciously sinks its fangs into " +
      him->one_short() + " who seethes in agony!\n" );
   tell_object( him, me->one_short() + " sinks its fangs into you!  You "
      "seethe in agony.\n" );

   him->add_effect( POISON_EFFECT, 100 );
} /* poison() */
