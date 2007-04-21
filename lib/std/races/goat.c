/*   Goat   *
 *  Gototh  *
 * 22/11/97 */

inherit "/std/races/ruminant";

void setup() {
   set_name( "goat" );
   set_long( "A cloven-hoofed, horned mammal closely related to the "
      "sheep.\n" );
   set_desc( "a cloven-hoofed, horned mammal" );

   set_weight( 800 );
   set_height( 80 );

   remove_bit( "head" );

   add_bit( "head", "head", ({ 0, 135, 0,
      "left eye", "right eye", "nose", "skull", "tongue", "left ear",
      "right ear", "left horn", "right horn" }) );
   add_bit( "left horn", "horn", ({ "head", 5, 0 }) );
   add_bit( "right horn", "horn", ({ "head", 5, 0 }) );

   inedible += ({ "left horn", "right horn" });

   set_stats( ({ 4, 8, 0, 2, -4 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   thing->remove_attack( "hands" );
   thing->remove_attack( "feet" );
   thing->remove_attack( "standard" );

   thing->remove_attack( "horns" );
   thing->remove_attack( "hoof" );

   thing->add_attack( "horns", 60, ({ 21, 8, 11 }),
      "pierce", "unarmed", 0 );
   thing->add_attack( "hoof", 60, ({ 32, 10, 11 }),
      "blunt", "unarmed", 0 );

   thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */

