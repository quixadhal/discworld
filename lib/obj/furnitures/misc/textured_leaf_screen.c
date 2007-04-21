// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "textured leaf screen" );
    add_adjective( ({ "textured", "leaf" }) );
    set_long( "This beautiful screen has a lovely honey oak frame which "
        "pales in comparison to the panelling it holds.  Three textured rice "
        "paper panels, each divided into six vertical segments, sit "
        "in the hinged frame, each one handpainted in "
        "watercolours with a tan leaf design.  The frame is "
        "double-hinged so it can bend both backwards and forward to suit "
        "its placement, and the rice paper panelling has been ribbed with "
        "matchstick-like strings of wood to give it more support. \n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(290000);

} /* setup() */
