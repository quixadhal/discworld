inherit "/obj/fungus";

void setup() {
  set_adjs( "red stalked" );
  set_type( "mushroom" );
  set_extra( "It seems to be gently smoking." );
  set_messages( ({
    "A small $type$ pushes up through the soil.\n",
    "The $type$ seems to be a bit larger now.\n",
    "The $type$ looks nice and large now.\n",
    "The $type$ swells and suddenly releases a cloud of smoke.\n",
    "There is a flash at the base of the $type$ and with a small roar, it "+
        "rockets into the sky.\n"
  }) );
} /* setup() */

void next_stage() {
  if ( stage == 8 )
    add_eat_effect( "/std/effects/ingested/basic_charging", 20 +
        random( 11 ) );
  ::next_stage();
} /* next_stage() */

int query_charging_power() {
  int denominator, numerator, wholes;
  mapping eat_effects;

  eat_effects = query_eat_effects();
  
  if ( !query_continuous() )
    return eat_effects[ "/std/effects/ingested/basic_charging" ] *
      (int)this_object()->query_weight();
  denominator = query_weight_unit()[ 1 ];
  numerator = eat_effects[ "/std/effects/ingested/basic_charging" ] *
    query_amount() * query_weight_unit()[ 0 ];
  wholes = numerator / denominator;
  numerator %= denominator;
  if ( random( denominator ) < numerator )
    wholes++;
  return wholes;
} /* query_charging_power() */
