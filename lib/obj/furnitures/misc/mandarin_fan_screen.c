// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "mandarin fan screen" );
    add_adjective( ({ "mandarin", "fan", "hana" }) );
    set_long( "This beautiful screen has a lovely walnut oak frame which "
        "pales in comparison to the panelling it holds.  Three smooth silk "
        "panels, each divided into eighteen square segments, sit "
        "in the hinged frame, each one handpainted in brilliant mandarin, "
        "lilac, cream and peridot watercolours with a mandarin orange "
        "hana fan design.  The frame is double-hinged so it can bend both "
        "backwards and forward to suit its placement.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(395000);

} /* setup() */
