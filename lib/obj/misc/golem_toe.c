/*
 * $Id$
 */

/* Toe of a golem */

inherit "/std/object";

void setup() {
    set_name( "toe" );
    set_short( "remains of a clay toe" );
    add_adjective( ({"remains", "of", "a", "clay"}) );
    set_long( "This looks part of a toe made of clay.\n" );
    set_main_plural( "remains of clay toes" );
    set_weight( 5 );
    set_value( 0 );
    add_property( "no recycling", 1 );
    add_property( "determinate", "the " );
} /* void setup() */

