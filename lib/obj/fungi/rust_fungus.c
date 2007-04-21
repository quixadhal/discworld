inherit "/obj/fungus";

void setup() {
  set_adjs( "rust" );
  set_type( "fungus" );
  set_extra( "It looks like a lump of rust, only soft and a bit squishy." );
  set_messages( ({
    "A small $type$ sprouts on the metal.\n",
    "The $type$ seems to be a bit larger now.\n",
    "The $type$ looks nice and large now.\n",
    "The $type$ silently releases a red dust into the air.\n",
    "The $type$ collapses in on itself and falls apart.\n"
  }) );
} /* setup() */

void next_stage() {
  if ( stage == 8 )
    add_eat_effect( "/std/effects/ingested/strength_boost", 2000 +
        random( 1000 ) );
  ::next_stage();
} /* next_stage() */
