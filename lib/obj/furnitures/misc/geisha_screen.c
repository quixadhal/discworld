// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "rice paper geisha screen" );
    add_adjective( ({ "rice", "paper", "geisha" }) );
    set_long("This is a simple screen that would fit in most traditional "
        "homes.  Quiet geishas relax on "
        "its rice paper panelling, hand-drawn on a bamboo balcony, "
        "they sit and sip tea in peace and solitude or play mandolin-like "
        "instruments which sit silently in their laps.  The frame is made "
        "of a strange combination of walnut and spruce woods, their "
        "differing colours blending well with the shades of the panels.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(320000);

} /* setup() */
