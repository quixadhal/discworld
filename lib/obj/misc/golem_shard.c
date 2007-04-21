/*
 * $Id$
 */

/* Clay shard (remains of a golem) */

inherit "/std/object";

void setup() {
    set_name( "shard" );
    set_short( "cracked clay shard" );
    add_adjective( ({"clay"}) );
    set_long( "This is a heavily cracked clay shard." );
    set_main_plural( "cracked clay shards" );
    set_weight( 5 );
    set_value( 0 );
    add_property( "no recycling", 1 );
} /* void setup() */


