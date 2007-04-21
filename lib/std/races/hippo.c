inherit "/std/races/ruminant";

void setup() {
   set_name( "elephant" );
   set_long( "This is an elephant.  A large creature with a thick grey "
      "hide.  He seems to weigh a lot so don't let him step on your "
      "toe.\n" );
   set_height( 280 );
   set_weight( 100000 );
   set_desc( "an elephant that looks back at you with small nice eyes" );

   remove_bit( "nose" );
   remove_bit( "head" );

   add_bit( "head", "head", ({ 0, 135, 0,
      "left eye", "right eye", "skull", "tongue", "left ear", "right ear",
      "tusk", "tusks", "trunk" }) );
   add_bit( "tail", "tail", ({ 0, 15, 0 }) );
   add_bit( "tusk", "tusk", ({ "head", 20, ({"/std/bit_set", 1}),
      "tusks" }) );
   add_bit( "tusks", "tusks", ({ "head", 40, ({"/std/bit", 2}) }) );
   add_bit( "trunk", "trunk", ({ "head", 30, 0 }) );

   inedible += ({ "tail", "tusk", "tusks" });  
   unrottable += ({ "tusk", "tusks" });

   add_ac( "blunt", "blunt", ({ 40, 20}) );
   add_ac( "pierce", "pierce", ({ 30, 20 }) );
   add_ac( "sharp", "sharp", ({ 50, 20 }) );
   add_ac( "acid", "acid", 20 );
   add_ac( "fire", "fire", 20 );

   set_stats( ({ 10, -6, -4, 16, -2 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   thing->remove_attack( "hands" );

   thing->remove_attack( "tusks" );
   thing->remove_attack( "crush" );
   thing->remove_attack( "ram" );

   thing->add_attack( "tusks", 20, ({75, 5, 30}),
      "pierce", "unarmed", 0 );

   thing->add_attack( "crush", 10, ({100, 5, 50}),
      "blunt", "unarmed", 0 );

   thing->add_attack_message( "crush", "unarmed", ({
     0, "$N stand$s on $p hind legs and attempt$s to crash down on $I",
      100, "$N stand$s on $p hind legs and crash$es down stricking $I "
       "a glancing blow ",
      150, "$N stand$s on $p hind legs and crash$es down on $I, crushing "
            "$P $z",
      350, "$N stand$s on $p hind legs and crash$es down on $I, crushing "
            "$P $z into a bloody mess" }) );

   thing->add_attack( "ram", 10, ({50, 3, 20}),
      "blunt", "unarmed", 0 );

   thing->add_attack_message( "ram", "unarmed", ({
     0, "$N charge$s at $I",
      100, "$N charge$s $I and strike$s $I",
      150, "$N charge$s $I and strike$s $P $z hard, making $I stagger "
            "around",
      350, "$N charge$s $I and strike$s $I to the ground.  $I "
            "stumbles around a little groggily" }) );

   thing->add_attack( "trunk", 20, ({50, 3, 20}),
      "blunt", "unarmed", 0 );

   thing->add_attack_message( "trunk", "unarmed", ({
     0, "$N swing$s $p trunk at $I",
      100, "$N swing$s $p trunk at $I, hitting $P $z hard",
      150, "$N swing$s $p trunk, which thumps forcefully into $P $z",
      350, "$N swing$s $p trunk at $I, knocking $I down.  $I "
            "gets up groggily" }) );
} /* set_unarmed_attacks() */
