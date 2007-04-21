// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "muslin screen" );
    add_adjective( "muslin" );
    set_long( "This is a simple and traditional double-hinged "
        "screen, its panels filled in with a long sheet of roughly "
        "textured cream-coloured muslin fabric.  A lightly coloured "
        "construction, the material has been gathered at the top "
        "and bottom ends to produce a eye-pleasing loosely pleated "
        "effect which continues down its length.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(225000);

} /* setup() */
