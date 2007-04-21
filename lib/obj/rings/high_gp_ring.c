/* Ringo - May 1997 */

#define GP_AWARD 250

inherit "/obj/ring";

void setup() {
    set_name( "ring" );
    set_short( "pink crystal ring" );
    add_adjective( ({ "pink", "crystal" }) );
    set_main_plural( "pink crystal rings" );
    set_long( "The ring is a single piece of $material$, cut and polished "+
      "until its facets sparkle in the light.\n" );
    set_material( "rosy-quartz" );
    set_value( 750 );
    set_value_info( "artifact", 15000 );
    set_weight( 1 );
    set_enchant( 5 );
    add_property( "artifact form", ([
        "ring of power" : 6
      ]) );
    adjust_charges( 1 + random( 11 ) );
    set_level( 75 );
    set_zapper( file_name( this_object() ) );
} /* setup() */

string query_ring_name() { return "ring of power"; }

void zap( object dummy, object person, object ring ) {
    tell_object( person, "You feel slightly more powerful.\n" );
    if ( ( (string)person->query_gp() + GP_AWARD ) >
      (string)person->query_max_gp() ) {
        person->set_gp( person->query_max_gp() );
        return;
    }
    person->adjust_gp( GP_AWARD );
} /* zap() */

int failed_zap( object dummy, object person, object ring ) {
    tell_object( person, "You feel slightly less powerful.\n" );
    person->add_succeeded_mess( ring, "$N $V $D.\n", ({ }) );
    if ( person->query_gp() < ( GP_AWARD / 3 ) ) {
        person->set_gp( 0 );
        return 1;
    }
    person->adjust_gp( 0 - ( GP_AWARD / 3 ) );
    return 1;
} /* failed_zap() */
