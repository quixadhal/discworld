/*
 * $Id$
 */

/* Eye of a golem */

inherit "/std/object";

void setup() {
    set_name( "triangle" );
    set_short( "clay triangle" );
    add_adjective( ({"clay"}) );
    set_long( "This is a cracked piece of clay.  A hole in the shape of a "
       "triangle is in its middle.\n" );
    set_main_plural( "clay triangles" );
    set_weight( 5 );
    set_value( 0 );
    add_property( "no recycling", 1 );
} /* void setup() */


