// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "burnt bamboo screen" );
    add_adjective( ({ "burnt", "bamboo" }) );
    set_long( "This screen has been constructed by "
        "weaving hundreds of lashes of bamboo together to form a tight "
        "mesh that blocks out any chance of peeking through to see what's "
        "on the other side.  The bamboo, lovely in differing shades of "
        "tan through to chocolate brown, is supported by long poles which "
        "make up the frame and hinge the entire structure together.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(400000);

} /* setup() */
