// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "watercolour landscape screen" );
    add_adjective( ({ "watercolour", "landscape" }) );
    set_long( "This beautiful screen has a lovely honey rosewood frame that "
        "pales in comparison to the panelling it holds.  Three rice "
        "paper panels sit in the hinged frame, each one handpainted in "
        "watercolours and depicting "
        "one third of a Agatean mountain landscape.  The frame is "
        "double-hinged so it can bend both backwards and forward to suit "
        "its placement, and the rice paper panelling has been ribbed with "
        "matchstick-like strings of wood to give it more support. \n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(400000);

} /* setup() */
