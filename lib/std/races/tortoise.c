inherit "/std/races/base";

void setup() {
   set_name( "tortoise" );
   set_long( "A tortoise.\n" );
   set_height( 40 );
   set_weight( 600 );
   set_desc( "a tortoise, slow and steady" );

   bits = ({ 
      "head", "head", ({ 0, 100, 0,
         "left eye", "right eye", "skull", "tongue" }),
      "left eye", "eye", ({ "head", 1, 0 }),
      "right eye", "eye", ({ "head", 1, 0 }),
      "skull", "skull", ({ "head", 50, 0, "brain" }),
      "beak", 0, ({ "skull", 20, 0 }),      
      "brain", "brain", ({ "skull", 30, 0 }),
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
      "left front leg", "leg", ({ 0, 150, 0, "left front claw" }),
      "left front claw", "claw", ({ "left front leg", 25, 0 }),
      "right front leg", "leg", ({ 0, 150, 0, "right front claw" }),
      "right front claw", "claw", ({ "right front leg", 25, 0 }),
      "left rear leg", "leg", ({ 0, 150, 0, "left rear claw" }),
      "left rear claw", "claw", ({ "left rear leg", 25, 0 }),
      "right rear leg", "leg", ({ 0, 150, 0, "right rear claw" }),
      "right rear claw", "claw", ({ "right rear leg", 25, 0 }),
      "tail", "tail", ({ 0, 25, 0 }),
      "shell", 0, ({ 0, 275, 0 })
   });

   inedible = ({ "skull", "beak", "shell" });
   unrottable = ({ "skull", "beak", "shell" });
   set_skin( "skin" );

   set_stats( ({ 2, 4, -2, 6, -4 }) );
} /* setup() */


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
