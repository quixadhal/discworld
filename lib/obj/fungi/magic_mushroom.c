inherit "/obj/fungus";

void setup() {
  set_adjs( "checkered" );
  set_type( "mushroom" );
  set_extra( "The checks are nice pastel shades of peach and mauve." );
  set_messages( ({
    "A small $type$ pushes up through the soil.\n",
    "The $type$ seems to be a bit larger now.\n",
    "The $type$ looks nice and large now.\n",
    "The $type$ jumps up and down very quickly, and then burps.\n",
    "You hear a small voice say: Goodbye, cruel world!\nThe $type$ "+
        "falls over and disintegrates.\n"
  }) );
} /* setup() */

void next_stage() {
  if ( stage == 8 )
    add_eat_effect( "/std/effects/ingested/hallucination", 150 +
        random( 150 ) );
  ::next_stage();
} /* next_stage() */
