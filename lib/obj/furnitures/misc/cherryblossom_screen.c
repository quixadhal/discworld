// Only for use within CWC - Una 07.02.02

inherit "/std/room/furniture/basic";

void setup() {

    set_name( "screen" );
    set_short( "cherry blossom shoji screen" );
    add_adjective( ({ "cherry", "blossom", "shoji" }) );
    set_long( "A stunning cherry blossom tree adorns the "
        "silk panelling of this double-sided shoji "
        "screen.  Four panels with black wooden trim fold "
        "out to make this screen stand tall, its blossoms "
        "painted on with the most precise hand, the thickness "
        "of the paint still showing on the silk panels.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(20);
    set_value(400000);

} /* setup() */

	
