
inherit "/std/races/rodent_base";

void setup() {
  set_name( "mole" );
  set_long( "Any insectivore of the family Talpidae, which includes moles, "
     "shrew moles, and desmans.  They have no ears and tiny eyes, and live "
     "in small burrows.\n" );
  set_height( 30 );
  set_weight( 10 );
  set_desc( "This is a blind and deaf mole." );

  set_sight( ({ -100, -60, 40, 60 }) );
  set_stats( ({ -2, 2, -6, -2, -6 }) );
} /* setup() */

void set_unarmed_attacks( object thing ) {
  int number;

  number = (int)thing->query_skill_bonus( "fighting.combat.melee.unarmed" );
  number = sqrt( number ) / 6;

  thing->remove_attack( "hands" );
  thing->remove_attack( "feet" );

  thing->remove_attack( "bite" );
  thing->remove_attack( "claws" );

  thing->add_attack( "bite", 75, ({ 0, 3, number }),
    "pierce", "unarmed", 0 );
  thing->add_attack( "claws", 50, ({ 10, 5, number / 2 }),
    "sharp", "unarmed", 0 );

  thing->tactics( "response dodge" );
} /* set_unarmed_attacks() */
