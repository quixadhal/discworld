//Una
//Only for use in CWC

inherit "/std/room/furniture/surface";

void setup() {
    set_name( "table" );
    set_short( "pedestal table" );
    add_adjective( "pedestal" );
    set_long("Finished in warm antique hues, this pedestal table is a "
        "roughly triangular object which ends abruptly in a flat plateau "
        "which looks to be the ideal place for placing a few books or "
        "a reading lantern.  Rising along its height are a series of "
        "drawers with brass handles, which narrow in width with the table "
        "as it rises.\n");
    set_allowed_positions( ({ "on" }) );
    set_allowed_room_verbs( ([ "standing" : "stands" ]) );
    set_weight( 250 );
    set_max_weight( 50 );
    set_value( 88000 );
    set_allow_use_internals( 1 );
} /* void setup() */
