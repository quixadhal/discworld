inherit "/obj/fungus";

void setup() {
  set_adjs( "blue capped" );
  set_type( "mushroom" );
  set_extra( "The shape of it reminds you of someone wearing a nightcap." );
} /* setup() */

void next_stage() {
  if ( stage == 8 )
    add_eat_effect( "/std/effects/ingested/tranquil_sleep", 60 +
        random( 60 ) );
  ::next_stage();
} /* next_stage() */
