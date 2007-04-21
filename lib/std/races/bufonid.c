inherit "/std/races/humanoid";

void setup() {
   set_name( "bufonid" );
   set_long( "The bufonids are rare species of humanoid toad-like beings.  "
      "It is rumoured that they were created by wizards as part of a "
      "heinous series of experiments of unknown intent and subsequently "
      "escaped from their creators.  Little is known of them, except that "
      "they keep themselves well hidden from humans.\n" );
   set_height( 150 );
   set_weight( 1800 );
   set_desc( "a strange creature, with webbed feet and warty green skin, "
      "that gives you the impression of being a humanoid toad" );

   remove_bit( "left ear" );
   remove_bit( "right ear" );
   remove_bit( "nose" );
   remove_bit( "scalp" );
   remove_bit( "teeth" );

   add_ac( "blunt", "blunt", 15 );
   add_ac( "pierce", "pierce", 12 );
   add_ac( "sharp", "sharp", 10 );

   add_attack( "hands", 75, ({ 40, 5, 10 }) );
   add_attack( "feet", 25, ({ 30, 5, 20 }) );

   set_stats( ({ 2, 2, 0, 2, 0 }) );
} /* setup() */
