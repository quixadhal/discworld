// Una
// For use in Cwc

inherit "/std/room/furniture/basic";

void setup() {
    
    set_name( "screen" );
    set_short( "HungHungese screen" );
    add_adjective( ({ "hunghungese", "HungHungese", "hunghung", 
        "HungHung" }) );
    set_long( "This is a contemporary double-sided screen for the home.  "
        "Framed in a light spruce, the panels of the screen have been "
        "included in the main construction of the frame and form a "
        "checkerboard pattern across its length.  Inside the "
        "checkerboard are long panels of cotton that have depictions "
        "of life in a hectic HungHung market.  There are several "
        "ducks, a few sheep, some dried fish and a lot of people "
        "that look like they are shouting at one another.\n");
    set_allowed_positions( ({ "standing", "sitting" }) );
    set_allowed_room_verbs((["standing" : "stands", "sitting" : "sits" ]) );
    set_weight(150);
    set_value(325000);

} /* setup() */
