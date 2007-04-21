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
      "right tusk" "left tusk", "trunk" }) );
      
   add_bit( "right tusk", "tusk", ({ "head", 20, ({ "/std/bit", 2 }) }) );
   add_bit( "left tusk", "tusk", ({ "head", 20, ({ "/std/bit", 2 }) }) );
   add_bit( "trunk", "trunk", ({ "head", 30, 0 }) );

   remove_bit( "left front leg" );
   remove_bit( "right front leg" );
   remove_bit( "left rear leg" );
   remove_bit( "right rear leg" );
   remove_bit( "left front hoof" );
   remove_bit( "right front hoof" );
   remove_bit( "left rear hoof" );
   remove_bit( "right rear hoof" );
      
   add_bit( "left front leg", "leg", ({ 0, 150, 0, "left front foot" }) );
   add_bit( "right front leg", "leg", ({ 0, 150, 0, "right front foot" }) );
   add_bit( "left rear leg", "leg", ({ 0, 150, 0, "left rear leg" }) );
   add_bit( "right rear leg", "leg", ({ 0, 150, 0, "right rear leg" }) );
   
   add_bit( "left front foot", "foot", ({ "left front leg", 30, 0 }) );
   add_bit( "right front foot", "foot", ({ "right front leg", 30, 0 }) );
   add_bit( "left rear foot", "foot", ({ "left rear leg", 30, 0 }) );
   add_bit( "right rear foot", "foot", ({ "right rear leg", 30, 0 }) );

   add_bit( "tail", "tail", ({ 0, 15, 0 }) );

   inedible += ({ "tail", "right tusk", "left tusk" });  
   unrottable += ({ "left tusk", "right tusk" });

   add_ac( "blunt", "blunt", ({ 40, 20}) );
   add_ac( "pierce", "pierce", ({ 30, 20 }) );
   add_ac( "sharp", "sharp", ({ 50, 20 }) );
   add_ac( "acid", "acid", 20 );
   add_ac( "fire", "fire", 20 );

   set_stats( ({ 30, -6, -4, 16, -2 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
   thing->remove_attack( "hands" );

   thing->remove_attack( "tusks" );
   thing->remove_attack( "crush" );
   thing->remove_attack( "ram" );

   thing->add_attack( "tusks", 25, ({200, 10, 30}), "pierce", "unarmed", 0 );
   thing->add_attack( "crush", 10, ({300, 10, 50}), "blunt", "unarmed", 0 );

   thing->add_attack_message( "crush", "unarmed", ({
     0, "$N stand$s on $p hind legs and attempt$s to crash down on $I",
      200, "$N stand$s on $p hind legs and crash$es down, striking $I "
       "a glancing blow",
      400, "$N stand$s on $p hind legs and crash$es down on $I, crushing "
            "$P $z",
      5000, "$N stand$s on $p hind legs and crash$es down on $I, crushing "
            "$P $z into a bloody mess" }) );

   thing->add_attack( "ram", 25, ({200, 10, 20}), "blunt", "unarmed", 0 );
   thing->add_attack_message( "ram", "unarmed", ({
     0, "$N charge$s at $I",
      200, "$N charge$s $I and strike$s $I",
      400, "$N charge$s $I and strike$s $P $z hard, making $I stagger "
            "around",
      5000, "$N charge$s $I and strike$s $I to the ground.  $I "
            "stumbles around a little groggily" }) );

   thing->add_attack( "trunk", 50, ({100, 5, 20}), "blunt", "unarmed", 0 );
   thing->add_attack_message( "trunk", "unarmed", ({
     0, "$N swing$s $p trunk at $I",
      200, "$N swing$s $p trunk at $I, hitting $P $z hard",
      400, "$N swing$s $p trunk, which thumps forcefully into $P $z",
      5000, "$N swing$s $p trunk at $I, nearly knocking $I down" }) );
} /* set_unarmed_attacks() */


string *query_target_zones() {
  return ({ "head", "head", "neck",
              "chest", "back", "back", "back", "lower back", 
              "left front leg", "left front leg", "left front foot",
              "stomach", "stomach", "tail",
              "right front leg", "right front leg", "right front foot",
              "left rear leg", "left rear leg", "left rear foot",
              "right rear leg", "right rear leg", "right rear foot"});
}


string map_target_zone(string zone) {
  switch(zone) {
  case "chest":
    return random(4) ? "back" : "chest";
  case "abdomen":
    return random(4) ? "stomach" : "lower back";
  case "arms":
    return (random(2) ? "left " : "right ") + "front leg";
  case "hands":
    return (random(2) ? "left " : "right ") + "front foot";
  case "legs":
    return (random(2) ? "left " : "right ") + "rear leg";
  case "feet":
    return (random(2) ? "left " :"right ") + "rear foot";
  default:
    return zone;
  }
}
