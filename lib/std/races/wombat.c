inherit "/std/races/base";

void setup() {
   set_name( "wombat" );
   set_long( "One of several stocky marsupials (family Vombatidae) "
      "resembling small bears.\n" );
   set_height( 40 );
   set_weight( 600 );
   set_desc( "a stocky wombat, looking for something to nibble" );

   bits = ({ 
      "head", "head", ({ 0, 100, 0,
         "left eye", "right eye", "left ear", "right ear", "skull" }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "left ear", "ear", ({ "head", 15, 0 }),
      "right ear", "ear", ({ "head", 15, 0 }),
      "skull", "skull", ({ "head", 50, 0, "teeth", "brain" }),
      "brain", "brain", ({ "skull", 30, 0 }),
      "teeth", "teeth", ({ "skull", 1, ({"/std/bit_set", 1}), "tooth" }),
      "tooth", "tooth", ({ "teeth", 1, ({"/std/bit", 18}) }),
      "torso", "torso", ({ 0, 450, 0,
         "heart", "liver", "left kidney", "right kidney", "spleen",
         "right lung", "left lung", "intestine" }),
      "heart", "heart", ({ "torso", 20, 0 }),
      "liver", "liver", ({ "torso", 20, 0 }),
      "left kidney", "kidney", ({ "torso", 10, 0 }),
      "right kidney", "kidney", ({ "torso", 10, 0 }),
      "spleen", "spleen", ({ "torso", 10, 0 }),
      "right lung", "lung", ({ "torso", 20, 0 }),
      "left lung", "lung", ({ "torso", 20, 0 }),
      "intestine", "gut", ({ "torso", 50, 0 }),
      "genitals", "genitals", ({ 0, 5, 0 }),
      "left front leg", "leg", ({ 0, 150, 0, "left front paw" }),
      "left front paw", "paw", ({ "left front leg", 25, 0 }),
      "right front leg", "leg", ({ 0, 150, 0, "right front paw" }),
      "right front paw", "paw", ({ "right front leg", 25, 0 }),
      "left rear leg", "leg", ({ 0, 150, 0, "left rear paw" }),
      "left rear paw", "paw", ({ "left rear leg", 25, 0 }),
      "right rear leg", "leg", ({ 0, 150, 0, "right rear paw" }),
      "right rear paw", "paw", ({ "right rear leg", 25, 0 }),
      "tail", "tail", ({ 0, 25, 0 }),
      "skin", "skin", ({ 0, 175, 0 })
   });

   inedible = ({ "skull", "teeth", "tail", "skin" });
   unrottable = ({ "skull", "teeth" });
   set_skin( "fur" );

   set_stats( ({ 2, 4, -2, 6, -4 }) );
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
   number = 4 + sqrt( number ) / 2;

   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );

   thing->remove_attack( "chew" );
   thing->remove_attack( "claws" );

   thing->add_attack( "chew", 100, ({ 3 * number, 0, number }),
      "sharp", "unarmed", 0 );
   thing->add_attack( "claws", 50, ({ ( 3 * number ) / 2, number, number }),
      "sharp", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
