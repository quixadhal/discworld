/*
 * $Id$
 */

/* Finger of a golem */

inherit "/std/object";

void setup() {
    set_name( "fingertip" );
    set_short( "broken clay fingertip" );
    add_adjective( ({"broken", "clay"}) );
    set_long( "This looks like a crude fingertip made of clay.  It is broken "
       "in the middle.\n" );
    set_main_plural( "broken clay fingertips" );
    set_weight( 5 );
    set_value( 0 );
    add_property( "no recycling", 1 );
} /* void setup() */


