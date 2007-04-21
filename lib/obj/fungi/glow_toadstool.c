inherit "/obj/fungus";

void setup() {
  set_adjs( "yellow spotted" );
  set_type( "toadstool" );
  set_extra( 0 );
} /* setup() */

void next_stage() {
  if ( stage == 8 ) {
    tell_room( environment(), "The yellow spots on the toadstool begin to "+
        "glow.\n" );
    set_light( 5 );
    set_long( query_long() +"The yellow spots are glowing.\n" );
  }
  ::next_stage();
} /* next_stage() */
