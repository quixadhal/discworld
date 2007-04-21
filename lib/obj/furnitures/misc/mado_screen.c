// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "mado screen" );
    add_adjective( "mado" );
    set_long( "This traditional mado screen has a double-sided "
        "latticework frame made of lacquered red cedar.  Between "
        "the two sides of the frame lies a large sheet of smooth "
        "white rice paper that looks quite delicate and "
        "fragile.  \n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(350000);

} /* setup() */
